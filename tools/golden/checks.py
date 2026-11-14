"""Functional checks run by golden.py with the python of the environment under test.

The golden *flow* compares exported files against a baseline; these checks cover behaviour the
flow does not touch (and that a baseline cannot describe, e.g. features that were broken in it).
Every check must pass. Output: <out_dir>/checks.json  {name: {"ok": bool, "error": str?}}
"""
import contextlib
import io
import json
import os
import sys

import numpy as np
import vivid3d


def check_colormap_by_name(name):
    def run(out):
        sphere = vivid3d.create_sphere()
        sphere.colormap = name                       # str -> matplotlib colormap lookup in C++
        sphere.export(os.path.join(out, f"cmap_{name}"), "glb")
        assert os.path.getsize(os.path.join(out, f"cmap_{name}.glb")) > 1000
    return run


def check_colormap_by_object(out):
    import matplotlib
    sphere = vivid3d.create_sphere()
    sphere.colormap = matplotlib.colormaps["plasma"]   # a Colormap object, not a name


def check_unknown_colormap_raises(out):
    sphere = vivid3d.create_sphere()
    try:
        sphere.colormap = "definitely-not-a-colormap"
    except Exception:
        return
    raise AssertionError("an unknown colormap name was accepted silently")


def check_log_goes_through_python_stdout(out):
    points = np.random.default_rng(1).uniform(-5, 5, (200, 3)).astype(np.float32)
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        vivid3d.VoronoiVolume(points, points[:, 2].tolist())
    assert "VIVID" in buf.getvalue(), "log output was not captured by contextlib.redirect_stdout"


def check_export_to_blob(out):
    blob = vivid3d.create_box().export_to_blob("glb")
    assert blob is not None


CHECKS = {
    "colormap_by_name_viridis (ListedColormap)": check_colormap_by_name("viridis"),
    "colormap_by_name_jet (LinearSegmentedColormap)": check_colormap_by_name("jet"),
    "colormap_by_object": check_colormap_by_object,
    "colormap_unknown_name_raises": check_unknown_colormap_raises,
    "log_goes_through_python_stdout": check_log_goes_through_python_stdout,
    "export_to_blob": check_export_to_blob,
}


def main(out_dir):
    os.makedirs(out_dir, exist_ok=True)
    results = {}
    for name, fn in CHECKS.items():
        try:
            fn(out_dir)
            results[name] = {"ok": True}
        except BaseException as exc:  # noqa: BLE001 - record any failure, never stop the run
            results[name] = {"ok": False, "error": f"{type(exc).__name__}: {exc}"[:300]}
    with open(os.path.join(out_dir, "checks.json"), "w") as fh:
        json.dump(results, fh, indent=2, sort_keys=True)
    bad = [k for k, v in results.items() if not v["ok"]]
    print(f"checks done: {len(results) - len(bad)} ok, {len(bad)} failed {bad}")


if __name__ == "__main__":
    main(sys.argv[1])
