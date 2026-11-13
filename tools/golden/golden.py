#!/usr/bin/env python3
"""Golden-output tool for VIVID3D: measure a build and compare it with a baseline.

  golden.py run     --repo <src> --label <name> [--root ~/work/golden]
  golden.py compare <baseline summary.json> <candidate summary.json>

`run` does, in order: fresh venv -> timed `pip wheel` -> wheel/installed size ->
install into a clean venv -> import checks (+ linked shared libraries) -> flow.py
exports (glb/obj/fbx) -> structural analysis -> <root>/<label>/summary.json.

Comparison is structural, not bit-for-bit: integers must match, floats use a
tolerance, timings and sizes are reported as ratios and never fail the run.
"""
import argparse
import glob
import json
import os
import platform
import shutil
import statistics
import subprocess
import sys
import tempfile
import time
import zipfile

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import analyze  # noqa: E402

def build_deps(repo):
    """Build requirements straight from the project's pyproject.toml (no second copy to keep in sync).
    scikit-build-core adds cmake/ninja on its own only for isolated builds; this tool builds without
    isolation (so the timing is compile time only), hence cmake and ninja are installed explicitly."""
    import re
    text = open(os.path.join(repo, "pyproject.toml"), encoding="utf-8").read()
    block = re.search(r"requires\s*=\s*\[(.*?)\]\s*\n", text, re.S).group(1)
    return re.findall(r'"([^"]+)"', block) + ["cmake>=3.15", "ninja"]


def sh(cmd, **kw):
    return subprocess.run(cmd, check=True, text=True, **kw)


def venv(path):
    shutil.rmtree(path, ignore_errors=True)
    sh([sys.executable, "-m", "venv", path])
    py = os.path.join(path, "bin", "python")
    sh([py, "-m", "pip", "install", "-q", "-U", "pip"])
    return py


def cmd_run(args):
    out = os.path.join(os.path.expanduser(args.root), args.label)
    shutil.rmtree(out, ignore_errors=True)
    os.makedirs(out)
    info = {"label": args.label, "python": platform.python_version(), "cpus": os.cpu_count(),
            "platform": platform.platform()}

    # --- build
    bpy = venv(os.path.join(out, "buildvenv"))
    sh([bpy, "-m", "pip", "install", "-q"] + build_deps(args.repo) + args.build_dep)
    for stale in ("_skbuild", "dist_golden"):
        shutil.rmtree(os.path.join(args.repo, stale), ignore_errors=True)
    wheel_dir = os.path.join(out, "wheel")
    log = os.path.join(out, "build.log")
    env = dict(os.environ)
    if args.cmake_arg:  # picked up by scikit-build-core, e.g. --cmake-arg=-DVIVID_ASSIMP=SYSTEM
        env["CMAKE_ARGS"] = " ".join(args.cmake_arg)

    source = args.repo
    if args.via_sdist:
        # build the source distribution first, then build the wheel FROM it: proves the sdist is complete
        sh([bpy, "-m", "pip", "install", "-q", "build"])
        sdist_dir = os.path.join(out, "sdist")
        sr = subprocess.run([bpy, "-m", "build", "--sdist", "--no-isolation", "--outdir", sdist_dir, args.repo],
                            capture_output=True, text=True, env=env)
        sdists = glob.glob(os.path.join(sdist_dir, "*.tar.gz"))
        if sr.returncode != 0 or not sdists:
            print("SDIST BUILD FAILED:\n" + (sr.stdout + sr.stderr)[-1500:])
            info["sdist"] = {"ok": False}
            return finish(out, info, 1)
        import tarfile
        with tarfile.open(sdists[0]) as tf:
            names = tf.getnames()
        info["sdist"] = {"ok": True, "bytes": os.path.getsize(sdists[0]), "n_files": len(names),
                         "top_dirs": sorted({n.split("/")[1] for n in names if n.count("/") >= 1})}
        source = sdists[0]

    t = time.perf_counter()
    with open(log, "w") as fh:
        rc = subprocess.run([bpy, "-m", "pip", "wheel", "-v", "--no-build-isolation", "--no-deps",
                             "-w", wheel_dir, source], stdout=fh, stderr=subprocess.STDOUT, env=env).returncode
    with open(log, errors="replace") as fh:
        assimp_lines = sorted({l.split("-- ", 1)[1].strip() for l in fh if "-- assimp:" in l})
    info["build"] = {"ok": rc == 0, "seconds": round(time.perf_counter() - t, 1), "log": log,
                     "cmake_args": args.cmake_arg, "assimp": assimp_lines}
    wheels = glob.glob(os.path.join(wheel_dir, "*.whl"))
    if rc != 0 or not wheels:
        print("BUILD FAILED, see", log)
        return finish(out, info, 1)
    whl = wheels[0]
    with zipfile.ZipFile(whl) as z:
        files = z.infolist()
    info["package"] = {"wheel": os.path.basename(whl), "wheel_bytes": os.path.getsize(whl),
                       "unpacked_bytes": sum(f.file_size for f in files), "n_files": len(files),
                       "native_files": {f.filename: f.file_size for f in files
                                        if f.filename.endswith((".so", ".pyd", ".dll", ".dylib"))}}

    # --- install + import
    rpy = venv(os.path.join(out, "runvenv"))
    # the wheel's own install_requires decide what is installed unless --run-dep pins something,
    # e.g. --run-dep=numpy==1.21.6 --run-dep=matplotlib==3.5.3 to test the declared lower bounds
    sh([rpy, "-m", "pip", "install", "-q", whl] + args.run_dep)
    times = []
    for _ in range(5):
        r = sh([rpy, "-c", "import time; t=time.perf_counter(); import vivid3d; "
                "print(time.perf_counter()-t)"], capture_output=True, cwd=tempfile.gettempdir())
        times.append(float(r.stdout.strip()))
    site = sh([rpy, "-c", "import vivid3d,os; print(os.path.dirname(vivid3d.__file__))"],
              capture_output=True, cwd=tempfile.gettempdir()).stdout.strip()
    needed = {}
    for so in glob.glob(os.path.join(site, "**", "*.so*"), recursive=True):
        ldd = subprocess.run(["ldd", so], capture_output=True, text=True).stdout
        needed[os.path.relpath(so, site)] = sorted(l.split()[0] for l in ldd.splitlines() if "=>" in l)
    info["import"] = {"ok": True, "median_seconds": round(statistics.median(times), 3),
                      "location": site, "linked_libs": needed}

    # --- flow + analysis
    exports = os.path.join(out, "exports")
    r = subprocess.run([rpy, os.path.join(HERE, "flow.py"), exports], capture_output=True, text=True,
                       cwd=tempfile.gettempdir())
    info["flow_stdout"] = r.stdout.strip()[-400:]
    info["flow_returncode"] = r.returncode
    if r.returncode != 0:
        info["flow_stderr"] = r.stderr[-1500:]
    fm = os.path.join(exports, "flow_metrics.json")
    info["flow"] = json.load(open(fm)) if os.path.exists(fm) else {}
    info["exports"] = analyze.analyze_dir(exports) if os.path.isdir(exports) else {}

    # functional checks (behaviour the flow does not touch; every check must pass)
    checks_dir = os.path.join(out, "checks")
    cr = subprocess.run([rpy, os.path.join(HERE, "checks.py"), checks_dir], capture_output=True, text=True,
                        cwd=tempfile.gettempdir())
    cj = os.path.join(checks_dir, "checks.json")
    info["checks"] = json.load(open(cj)) if os.path.exists(cj) else {
        "checks.py crashed": {"ok": False, "error": (cr.stderr or cr.stdout)[-300:]}}
    return finish(out, info, 0 if r.returncode == 0 else 2)


def finish(out, info, code):
    path = os.path.join(out, "summary.json")
    with open(path, "w") as fh:
        json.dump(info, fh, indent=2, sort_keys=True)
    b, p, i, f = info.get("build", {}), info.get("package", {}), info.get("import", {}), info.get("flow", {})
    print(f"summary: {path}")
    print(f"  build   : {'ok' if b.get('ok') else 'FAILED'} in {b.get('seconds')}s on {info['cpus']} cpus")
    if p:
        print(f"  package : wheel {p['wheel_bytes']/1e6:.1f} MB, unpacked {p['unpacked_bytes']/1e6:.1f} MB")
    if i:
        print(f"  import  : median {i['median_seconds']}s")
    if f:
        bad = [k for k, v in f.get("stages", {}).items() if not v["ok"]]
        print(f"  flow    : {info['flow_stdout']}  peak RSS {f.get('peak_rss_mb')} MB")
    if "checks" in info:
        failed = [k for k, v in info["checks"].items() if not v["ok"]]
        print(f"  checks  : {len(info['checks']) - len(failed)} ok, {len(failed)} failed {failed}")
        if failed and code == 0:
            code = 3
    return code


# ----------------------------------------------------------------------------- compare
def close(a, b, rtol, atol):
    return abs(a - b) <= atol + rtol * max(abs(a), abs(b))


def diff(a, b, path, out, rtol, atol):
    if isinstance(a, dict) and isinstance(b, dict):
        for k in sorted(set(a) | set(b)):
            if k not in a:
                out.append(f"{path}/{k}: only in candidate")
            elif k not in b:
                out.append(f"{path}/{k}: only in baseline")
            else:
                diff(a[k], b[k], f"{path}/{k}", out, rtol, atol)
    elif isinstance(a, list) and isinstance(b, list):
        if len(a) != len(b):
            out.append(f"{path}: length {len(a)} -> {len(b)}")
        for n, (x, y) in enumerate(zip(a, b)):
            diff(x, y, f"{path}[{n}]", out, rtol, atol)
    elif isinstance(a, float) or isinstance(b, float):
        if not (isinstance(a, (int, float)) and isinstance(b, (int, float)) and close(a, b, rtol, atol)):
            out.append(f"{path}: {a} -> {b}")
    elif a != b:
        out.append(f"{path}: {a!r} -> {b!r}")


def ratio(a, b):
    return f"{a} -> {b} ({b / a:.2f}x)" if a else f"{a} -> {b}"


def cmd_compare(args):
    A, B = (json.load(open(p)) for p in (args.baseline, args.candidate))
    fails = []
    print("== informational (never fails) ==")
    print("  build seconds :", ratio(A["build"]["seconds"], B["build"]["seconds"]),
          "(cpus %s -> %s)" % (A["cpus"], B["cpus"]))
    print("  assimp source :", A["build"].get("assimp") or "n/a (pre-option baseline)", "->",
          B["build"].get("assimp") or "n/a", "| cmake args:", B["build"].get("cmake_args") or [])
    for key in ("wheel_bytes", "unpacked_bytes", "n_files"):
        print(f"  package {key:14}:", ratio(A["package"][key], B["package"][key]))
    print("  import seconds:", ratio(A["import"]["median_seconds"], B["import"]["median_seconds"]))
    print("  peak RSS MB   :", ratio(A["flow"].get("peak_rss_mb", 0), B["flow"].get("peak_rss_mb", 0)))
    print("  total flow s  :", ratio(A["flow"].get("total_seconds", 0), B["flow"].get("total_seconds", 0)))
    for lib in sorted(set(A["package"]["native_files"]) | set(B["package"]["native_files"])):
        print(f"  native {lib}:", A["package"]["native_files"].get(lib), "->", B["package"]["native_files"].get(lib))
    la = {k: v for k, v in A["import"]["linked_libs"].items()}
    lb = {k: v for k, v in B["import"]["linked_libs"].items()}
    for so in sorted(set(la) | set(lb)):
        added = sorted(set(lb.get(so, [])) - set(la.get(so, [])))
        gone = sorted(set(la.get(so, [])) - set(lb.get(so, [])))
        if added or gone:
            print(f"  linked libs {so}: +{added} -{gone}")

    print("\n== structural checks ==")
    if not B["build"]["ok"]:
        fails.append("candidate build failed")
    if not B["import"]["ok"]:
        fails.append("candidate import failed")
    for stage, v in A["flow"].get("stages", {}).items():
        w = B["flow"].get("stages", {}).get(stage)
        if w is None or v["ok"] != w["ok"]:
            fails.append(f"flow stage {stage}: ok {v['ok']} -> {w and w['ok']}")
    # functional checks are absolute (they have no baseline): each must pass in the candidate
    for name, res in B.get("checks", {}).items():
        if not res["ok"]:
            fails.append(f"check '{name}' failed: {res.get('error')}")
    if "checks" not in B:
        print("  note: candidate has no functional checks (run with a newer tools/golden)")
    ea, eb = A["exports"], B["exports"]
    for name in sorted(set(ea) | set(eb)):
        if name not in ea or name not in eb:
            fails.append(f"export {name}: only in {'candidate' if name in eb else 'baseline'}")
            continue
        x, y = dict(ea[name]), dict(eb[name])
        sx, sy = x.pop("size_bytes"), y.pop("size_bytes")
        found = []
        diff(x, y, name, found, args.rtol, args.atol)
        fails += found
        if sx and abs(sx - sy) / sx > 0.05:
            print(f"  note: {name} size {sx} -> {sy} bytes")
    diff(A["flow"].get("scene_counts"), B["flow"].get("scene_counts"), "scene_counts", fails, args.rtol, args.atol)

    for f in fails:
        print("  FAIL", f)
    print(f"\n{'PASS' if not fails else 'FAIL'}: {len(fails)} structural difference(s)")
    return 1 if fails else 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("run")
    r.add_argument("--repo", required=True)
    r.add_argument("--label", required=True)
    r.add_argument("--root", default="~/work/golden")
    r.add_argument("--cmake-arg", action="append", default=[], help="extra CMake option, repeatable")
    r.add_argument("--via-sdist", action="store_true",
                   help="build an sdist first and build the wheel from it (verifies the sdist is complete)")
    r.add_argument("--build-dep", action="append", default=[],
                   help="extra/pinned build requirement appended to the pyproject list, e.g. pybind11==3.0.1")
    r.add_argument("--run-dep", action="append", default=[],
                   help="pin a runtime dependency for the test venv, repeatable (default: whatever the wheel requires)")
    c = sub.add_parser("compare")
    c.add_argument("baseline")
    c.add_argument("candidate")
    c.add_argument("--rtol", type=float, default=1e-3)
    c.add_argument("--atol", type=float, default=1e-3)
    a = ap.parse_args()
    sys.exit(cmd_run(a) if a.cmd == "run" else cmd_compare(a))


if __name__ == "__main__":
    main()
