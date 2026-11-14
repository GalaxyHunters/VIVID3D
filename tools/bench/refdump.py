#!/usr/bin/env python3
"""Exact-output check for refactors of the surface code (iso_surface / smooth / reduce / export).

    # 1. with the build BEFORE the change (python that has the old vivid3d):
    python refdump.py dump --python <old py> --out ref_old
    # 2. with the build AFTER the change:
    python refdump.py dump --python <new py> --out ref_new
    # 3. compare:
    python refdump.py compare ref_old ref_new

For seeded synthetic galaxies it exports, for every density mask, the mesh straight out of iso_surface, and again after
smooth() and reduce(), to OBJ, and stores the files plus their SHA-256. `compare` demands byte-identical files:
same vertices, same faces, in the same order. Vertex/face order matters because the decimation is order sensitive.
(Compare builds of the SAME platform; different compilers may differ in the last float bit.)
"""
import argparse, hashlib, json, os, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))


def sha(p):
    return hashlib.sha256(open(p, "rb").read()).hexdigest()


def dump_child(out, sizes, seeds):
    sys.path.insert(0, HERE)
    import numpy as np, scenes, vivid3d
    manifest = {"vivid3d": vivid3d.__version__, "files": {}}
    os.makedirs(out, exist_ok=True)
    for n in sizes:
        for seed in seeds:
            sc = scenes.galaxy(n, seed)
            vol = vivid3d.VoronoiVolume(sc["points"], sc["metric"])
            for mi, mk in enumerate(scenes.masks(sc, (0.05, 0.20, 0.50))):
                m = vol.iso_surface(mk, "m", 1.0)
                tag = "n%d_s%d_m%d" % (n, seed, mi)
                m.export(os.path.join(out, tag + "_iso"), "obj")
                m.smooth(5, 0.7, 0.0)
                m.reduce(0.5, 0.5)
                m.export(os.path.join(out, tag + "_final"), "obj")
                manifest["files"][tag] = dict(polys_final=m.n_polygons, verts_final=m.n_vertices)
    for f in sorted(os.listdir(out)):
        if f.endswith(".obj"):
            manifest["files"].setdefault(f.split("_i")[0].split("_f")[0], {})[f] = sha(os.path.join(out, f))
    json.dump(manifest, open(os.path.join(out, "manifest.json"), "w"), indent=1)
    print("dumped", len([f for f in os.listdir(out) if f.endswith(".obj")]), "obj files to", out)


def compare(a, b):
    A, B = (json.load(open(os.path.join(d, "manifest.json"))) for d in (a, b))
    bad, files = 0, sorted(f for f in os.listdir(a) if f.endswith(".obj"))
    for f in files:
        pa, pb = os.path.join(a, f), os.path.join(b, f)
        if not os.path.exists(pb):
            print("MISSING in new:", f); bad += 1
        elif sha(pa) != sha(pb):
            print("DIFFERENT      :", f, "(%d vs %d bytes)" % (os.path.getsize(pa), os.path.getsize(pb))); bad += 1
    print("%d files compared, %d differences  (old %s, new %s)" % (len(files), bad, A["vivid3d"], B["vivid3d"]))
    return bad


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    d = sub.add_parser("dump"); d.add_argument("--python", default=sys.executable); d.add_argument("--out", required=True)
    d.add_argument("--sizes", default="2000,20000,60000"); d.add_argument("--seeds", default="0,1")
    d.add_argument("--child", action="store_true")
    c = sub.add_parser("compare"); c.add_argument("old"); c.add_argument("new")
    a = ap.parse_args()
    if a.cmd == "compare":
        sys.exit(1 if compare(a.old, a.new) else 0)
    sizes, seeds = [int(float(x)) for x in a.sizes.split(",")], [int(x) for x in a.seeds.split(",")]
    if a.child:
        dump_child(a.out, sizes, seeds)
    else:
        sys.exit(subprocess.call([a.python, os.path.abspath(__file__), "dump", "--child", "--out", a.out, "--sizes", a.sizes, "--seeds", a.seeds]))
