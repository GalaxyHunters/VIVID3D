#!/usr/bin/env python3
"""Scalable benchmark of the real VIVID flow:  points -> Voronoi -> iso-surface -> smooth -> reduce (decimate) -> export.

    # one run: 1e6 points, one monolithic Voronoi
    python bench.py run   --python <python with vivid3d> --n 1e6 --label mono-1e6
    # the same scene split into 8 spatial chunks (Voronoi per chunk, with overlapping halos)
    python bench.py run   --python <py> --n 1e6 --chunks 8 --label chunk8-1e6
    # a ladder of sizes; stops at the first failure; fits time/memory growth and predicts bigger sizes
    python bench.py scale --python <py> --sizes 1e3,1e4,1e5,1e6 --predict 1e7,2e7 --mem-cap-gb 8

Every run is a fresh child process (clean memory, no interference). The PARENT samples the child's resident
memory every 0.1 s (the wrapper holds the GIL during long C++ calls, so a sampler thread inside the child would
starve) and kills it when --mem-cap-gb or --timeout is exceeded, so a size that does not fit fails cleanly
instead of freezing the machine. Per-stage peak memory = max sample between the stage's start and end stamps.
Results: <root>/<label>/result.json   (default root ~/work/bench). Only numpy + the vivid3d to test are needed.
"""
import argparse, contextlib, json, math, os, platform, subprocess, sys, time

HERE = os.path.dirname(os.path.abspath(__file__))
STAGE_ORDER = ["scene", "voronoi", "iso_surface", "smooth", "reduce", "model", "export"]


# ---------------------------------------------------------------- memory of another process (no psutil)
if sys.platform == "win32":
    import ctypes
    from ctypes import wintypes

    class _PMC(ctypes.Structure):
        _fields_ = [("cb", wintypes.DWORD), ("PageFaultCount", wintypes.DWORD),
                    ("PeakWorkingSetSize", ctypes.c_size_t), ("WorkingSetSize", ctypes.c_size_t),
                    ("QuotaPeakPagedPoolUsage", ctypes.c_size_t), ("QuotaPagedPoolUsage", ctypes.c_size_t),
                    ("QuotaPeakNonPagedPoolUsage", ctypes.c_size_t), ("QuotaNonPagedPoolUsage", ctypes.c_size_t),
                    ("PagefileUsage", ctypes.c_size_t), ("PeakPagefileUsage", ctypes.c_size_t)]
    _k32, _psapi = ctypes.WinDLL("kernel32", use_last_error=True), ctypes.WinDLL("psapi", use_last_error=True)
    _k32.OpenProcess.restype = wintypes.HANDLE
    _k32.OpenProcess.argtypes = [wintypes.DWORD, wintypes.BOOL, wintypes.DWORD]
    _k32.CloseHandle.argtypes = [wintypes.HANDLE]
    _psapi.GetProcessMemoryInfo.argtypes = [wintypes.HANDLE, ctypes.POINTER(_PMC), wintypes.DWORD]

    def rss_bytes(pid):
        h = _k32.OpenProcess(0x1000 | 0x0010, False, pid)    # QUERY_LIMITED_INFORMATION | VM_READ
        if not h:
            return 0
        try:
            c = _PMC(); c.cb = ctypes.sizeof(c)
            return c.WorkingSetSize if _psapi.GetProcessMemoryInfo(h, ctypes.byref(c), c.cb) else 0
        finally:
            _k32.CloseHandle(h)

    def total_ram_gb():
        class MS(ctypes.Structure):
            _fields_ = [("l", wintypes.DWORD), ("load", wintypes.DWORD), ("tp", ctypes.c_uint64), ("ap", ctypes.c_uint64),
                        ("tpf", ctypes.c_uint64), ("apf", ctypes.c_uint64), ("tv", ctypes.c_uint64), ("av", ctypes.c_uint64), ("x", ctypes.c_uint64)]
        m = MS(); m.l = ctypes.sizeof(m); _k32.GlobalMemoryStatusEx(ctypes.byref(m)); return m.tp / 2**30
else:
    def rss_bytes(pid):
        try:
            with open("/proc/%d/status" % pid) as f:
                for line in f:
                    if line.startswith("VmRSS:"):
                        return int(line.split()[1]) * 1024
        except OSError:
            pass
        return 0

    def total_ram_gb():
        with open("/proc/meminfo") as f:
            return int(f.readline().split()[1]) / 2**20


# ---------------------------------------------------------------- the child: runs the flow, stamps the stages
def child(a):
    sys.path.insert(0, HERE)
    import numpy as np
    import scenes
    with open(a.json + ".pid", "w") as f:          # the venv launcher on Windows is a separate parent process
        f.write(str(os.getpid()))
    rec = []
    out = {"stages": rec}
    t_imp = time.time()
    import vivid3d
    out.update(vivid3d=vivid3d.__version__, numpy=np.__version__, import_s=round(time.time() - t_imp, 3))

    def save(status="running"):
        out["status"] = status
        with open(a.json, "w") as f:
            json.dump(out, f)

    @contextlib.contextmanager
    def stage(name, **info):
        t0 = time.time()
        try:
            yield
        finally:
            rec.append(dict(name=name, t0=t0, t1=time.time(), **info))
            save()

    with stage("scene", n=a.n):
        sc = scenes.galaxy(a.n, a.seed)
        pts, metric = sc["points"], sc["metric"]
        masks = scenes.masks(sc, a.fractions)
    out["mask_counts"] = [int(m.sum()) for m in masks]

    # chunks: split along x by quantiles; every chunk also gets a halo (a slab of its neighbours) so surfaces
    # near a cut are computed with their true neighbours. The surface still has seams at the cuts (benchmark only).
    if a.chunks > 1:
        order = np.argsort(pts[:, 0], kind="stable")
        edges = np.linspace(0, a.n, a.chunks + 1).astype(int)
        halo = max(1, int(a.halo * a.n / a.chunks))
        parts = [(order[max(0, edges[i] - halo):min(a.n, edges[i + 1] + halo)]) for i in range(a.chunks)]
    else:
        parts = [None]

    meshes = []
    for ci, idx in enumerate(parts):
        p = pts if idx is None else pts[idx]
        m = metric if idx is None else metric[idx]
        with stage("voronoi", chunk=ci, n=int(len(p))):
            vol = vivid3d.VoronoiVolume(p, m)
        for mi, mk in enumerate(masks):
            mk = mk if idx is None else mk[idx]
            if mk.all() or not mk.any():          # this chunk has no surface for this density cut (the library needs both values)
                continue
            with stage("iso_surface", chunk=ci, mask=mi):
                mesh = vol.iso_surface(mk, "mask%d_c%d" % (mi, ci), 1.0 - 0.3 * mi)
            polys0 = mesh.n_polygons
            with stage("smooth", chunk=ci, mask=mi, polys=polys0):
                mesh.smooth(a.smooth_iters, 0.7, 0.0)
            with stage("reduce", chunk=ci, mask=mi, polys=polys0):
                mesh.reduce(a.reduce_percent, a.reduce_error)
            out.setdefault("meshes", []).append(dict(chunk=ci, mask=mi, polys_before=polys0, polys_after=mesh.n_polygons, verts=mesh.n_vertices))
            meshes.append(mesh)
        del vol                                    # the real flow frees the volume once the surfaces exist
    with stage("model"):
        model = vivid3d.Model(meshes)
    out["model"] = dict(meshes=model.n_meshes, polys=sum(x["polys_after"] for x in out["meshes"]))
    for fmt in a.formats:
        path = os.path.join(a.workdir, "model")
        with stage("export", fmt=fmt):
            model.export(path, fmt)
        f = "%s.%s" % (path, fmt)
        if os.path.exists(f):
            out.setdefault("export_bytes", {})[fmt] = os.path.getsize(f)
            if not a.keep:
                os.remove(f)
    save("ok")


# ---------------------------------------------------------------- the parent: spawn, sample memory, enforce limits
def run(a, quiet=False):
    label = a.label or "run"
    d = os.path.join(a.root, label)
    os.makedirs(d, exist_ok=True)
    jpath = os.path.join(d, "child.json")
    for f in (jpath, jpath + ".pid"):
        if os.path.exists(f):
            os.remove(f)
    cmd = [a.python, os.path.abspath(__file__), "_child", "--n", repr(float(a.n)), "--seed", str(a.seed), "--chunks", str(a.chunks),
           "--halo", str(a.halo), "--json", jpath, "--workdir", d, "--formats", ",".join(a.formats),
           "--fractions", ",".join(map(str, a.fractions)), "--smooth-iters", str(a.smooth_iters),
           "--reduce-percent", str(a.reduce_percent), "--reduce-error", str(a.reduce_error)] + (["--keep"] if a.keep else [])
    cap, t_start, samples, status, peak = a.mem_cap_gb * 2**30, time.time(), [], "ok", 0
    with open(os.path.join(d, "child.log"), "w") as log:
        p = subprocess.Popen(cmd, stdout=log, stderr=subprocess.STDOUT)
        target = p.pid
        def kill():
            if sys.platform == "win32":
                subprocess.run(["taskkill", "/F", "/T", "/PID", str(p.pid)], capture_output=True)
            else:
                p.kill()
        while p.poll() is None:
            if target == p.pid and os.path.exists(jpath + ".pid"):
                try:
                    target = int(open(jpath + ".pid").read().strip() or p.pid)
                except ValueError:
                    pass
            r = rss_bytes(target)
            samples.append((time.time(), r))
            peak = max(peak, r)
            if r > cap:
                kill(); status = "KILLED: memory cap %.1f GB" % a.mem_cap_gb; break
            if time.time() - t_start > a.timeout:
                kill(); status = "KILLED: timeout %ds" % a.timeout; break
            time.sleep(0.1)
        p.wait()
    wall = time.time() - t_start
    if status == "ok" and p.returncode != 0:
        status = "FAILED: exit code %s (see %s)" % (p.returncode, os.path.join(d, "child.log"))
    try:
        child_json = json.load(open(jpath))
    except Exception:
        child_json = {"stages": []}
    if status == "ok" and child_json.get("status") != "ok":
        status = "FAILED: child did not finish"
    agg = {}
    for s in child_json["stages"]:
        name = s["name"]
        mem = max([r for t, r in samples if s["t0"] <= t <= s["t1"]] or [0])
        e = agg.setdefault(name, {"seconds": 0.0, "peak_rss_mb": 0.0, "calls": 0})
        e["seconds"] += s["t1"] - s["t0"]; e["calls"] += 1
        e["peak_rss_mb"] = max(e["peak_rss_mb"], mem / 2**20)
    res = dict(label=label, n=int(a.n), seed=a.seed, chunks=a.chunks, status=status, wall_s=round(wall, 2),
               peak_rss_mb=round(peak / 2**20, 1), pipeline_s=round(sum(v["seconds"] for k, v in agg.items() if k != "scene"), 2),
               stages={k: {kk: round(vv, 3) for kk, vv in v.items()} for k, v in agg.items()},
               meshes=child_json.get("meshes"), export_bytes=child_json.get("export_bytes"), import_s=child_json.get("import_s"),
               vivid3d=child_json.get("vivid3d"), host=dict(os=platform.platform(), cpus=os.cpu_count(), ram_gb=round(total_ram_gb(), 1)),
               args=dict(smooth_iters=a.smooth_iters, reduce_percent=a.reduce_percent, reduce_error=a.reduce_error, halo=a.halo))
    json.dump(res, open(os.path.join(d, "result.json"), "w"), indent=1)
    if not quiet:
        report(res)
    return res


def report(r):
    print("%-14s n=%-10s chunks=%-3d %s" % (r["label"], format(r["n"], ","), r["chunks"], r["status"]))
    print("  wall %.1fs  pipeline %.1fs  peak RSS %.0f MB  (vivid3d %s, %s cpus, %s GB RAM)" % (r["wall_s"], r["pipeline_s"], r["peak_rss_mb"], r["vivid3d"], r["host"]["cpus"], r["host"]["ram_gb"]))
    for k in STAGE_ORDER:
        if k in r["stages"]:
            v = r["stages"][k]
            print("  %-12s %9.2fs  peak %8.0f MB  x%d" % (k, v["seconds"], v["peak_rss_mb"], v["calls"]))
    if r.get("meshes"):
        print("  polygons per mask (before -> after reduce):", [(m["polys_before"], m["polys_after"]) for m in r["meshes"]][:6])


def fit_and_predict(results, targets):
    """power-law fit  y = a * n^b  per stage on the successful runs with n >= 1e4, then predictions."""
    ok = [r for r in results if r["status"] == "ok" and r["n"] >= 1e4]
    if len(ok) < 2:
        print("\n(not enough successful runs with n >= 1e4 to fit growth)"); return
    def fit(xs, ys):
        lx, ly = [math.log(x) for x in xs], [math.log(max(y, 1e-9)) for y in ys]
        mx, my = sum(lx) / len(lx), sum(ly) / len(ly)
        b = sum((x - mx) * (y - my) for x, y in zip(lx, ly)) / max(sum((x - mx) ** 2 for x in lx), 1e-12)
        return math.exp(my - b * mx), b
    print("\nGrowth fit on n >= 1e4 (y = a * n^b; b=1 is linear, b>1 worse than linear):")
    rows = [("pipeline time (s)", [r["pipeline_s"] for r in ok]), ("peak memory (MB)", [r["peak_rss_mb"] for r in ok])]
    rows += [("  %s (s)" % k, [r["stages"].get(k, {}).get("seconds", 0) for r in ok]) for k in STAGE_ORDER[1:] if any(k in r["stages"] for r in ok)]
    xs = [r["n"] for r in ok]
    for name, ys in rows:
        a_, b_ = fit(xs, ys)
        pred = "  ".join("%s: %s" % (format(t, ".0e"), ("%.1f GB" % (a_ * t ** b_ / 1024)) if "memory" in name else ("%.0f s = %.1f min" % (a_ * t ** b_, a_ * t ** b_ / 60))) for t in targets)
        print("  %-26s exponent %.2f   -> %s" % (name, b_, pred))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    def common(p):
        p.add_argument("--python", default=sys.executable, help="python that has the vivid3d to measure")
        p.add_argument("--seed", type=int, default=0)
        p.add_argument("--chunks", type=int, default=1, help="K spatial chunks, Voronoi per chunk (1 = one monolithic Voronoi)")
        p.add_argument("--halo", type=float, default=0.05, help="chunk overlap as a fraction of the chunk size")
        p.add_argument("--formats", type=lambda s: [x for x in s.split(",") if x], default=["glb"])
        p.add_argument("--fractions", type=lambda s: [float(x) for x in s.split(",")], default=[0.05, 0.20, 0.50], help="density cut per mask")
        p.add_argument("--smooth-iters", type=int, default=5)
        p.add_argument("--reduce-percent", type=float, default=0.5)
        p.add_argument("--reduce-error", type=float, default=0.5)
        p.add_argument("--keep", action="store_true", help="keep the exported model files")
        p.add_argument("--root", default=os.path.join(os.path.expanduser("~"), "work", "bench"))
        p.add_argument("--mem-cap-gb", type=float, default=0.8 * total_ram_gb(), help="kill the run above this resident memory")
        p.add_argument("--timeout", type=int, default=3 * 3600)
    r = sub.add_parser("run"); common(r); r.add_argument("--n", type=float, required=True); r.add_argument("--label")
    s = sub.add_parser("scale"); common(s)
    s.add_argument("--sizes", default="1e3,1e4,1e5,1e6"); s.add_argument("--predict", default="1e7,2e7"); s.add_argument("--tag", default="scale")
    c = sub.add_parser("_child"); c.add_argument("--n", type=float); c.add_argument("--seed", type=int); c.add_argument("--chunks", type=int)
    c.add_argument("--halo", type=float); c.add_argument("--json"); c.add_argument("--workdir"); c.add_argument("--keep", action="store_true")
    c.add_argument("--formats", type=lambda s: [x for x in s.split(",") if x]); c.add_argument("--fractions", type=lambda s: [float(x) for x in s.split(",")])
    c.add_argument("--smooth-iters", type=int); c.add_argument("--reduce-percent", type=float); c.add_argument("--reduce-error", type=float)
    a = ap.parse_args()
    if a.cmd == "_child":
        a.n = int(a.n); return child(a)
    if a.cmd == "run":
        return run(a)
    results = []
    for n in [float(x) for x in a.sizes.split(",")]:
        a.n, a.label = n, "%s-c%d-%s" % (a.tag, a.chunks, format(n, ".0e").replace("+", ""))
        res = run(a); results.append(res)
        if res["status"] != "ok":
            print("stopping the ladder at n=%s" % format(n, ".0e")); break
    fit_and_predict(results, [float(x) for x in a.predict.split(",")])


if __name__ == "__main__":
    main()
