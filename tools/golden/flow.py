"""Deterministic Vivid scene used for golden-output comparison.

Run with the python of the environment under test:  python flow.py <out_dir>
Exports a multi-component model plus single components to glb/obj/fbx and writes
flow_metrics.json (timings, peak RSS, per-export success or error text).
Errors are recorded rather than raised so a broken exporter shows up in the diff.
"""
import json
import os
import resource
import sys
import time

import numpy as np
import vivid3d

FORMATS = ("glb", "obj", "fbx")


def build_scene():
    rng = np.random.default_rng(1234)

    n = 15
    axis = np.linspace(-14, 14, n)
    gx, gy, gz = np.meshgrid(axis, axis, axis, indexing="ij")
    points = np.stack([gx.ravel(), gy.ravel(), gz.ravel()], axis=1).astype(np.float32)
    dist = np.linalg.norm(points, axis=1)
    color = points[:, 2].astype(float).tolist()

    voronoi = vivid3d.VoronoiVolume(points, color)
    # outer shell is translucent on purpose: the opaque-inside-opaque layout of the first
    # version hid the inner ball, so alpha blending was never actually visible or compared.
    outer = voronoi.iso_surface((dist > 9).tolist(), "outer", 0.4)
    inner = voronoi.iso_surface((dist > 5).tolist(), "inner", 0.5)
    outer.smooth()
    outer.material = vivid3d.Material(opacity=0.4, metalness=0.8, roughness=0.3,
                                      emission_strength=2.0, label="metal_emissive")
    inner.set_color("crimson")
    inner.material = vivid3d.Material(opacity=0.5, metalness=0.1, roughness=0.9,
                                      emission_strength=0.0, label="matte_glass")

    sphere = vivid3d.create_sphere([20, 0, 0], 4, 12, 12, "gold", 1.0, "sphere")
    box = vivid3d.create_box([-20, 0, 0], [3, 2, 1], "lightgray", 1.0, "box")

    cloud = vivid3d.PointCloud(rng.uniform(-10, 10, (300, 3)).astype(np.float32),
                               "white", 1.0, "cloud")

    lines = vivid3d.Lines(1.0, "lines")
    for k in range(4):
        t = np.linspace(0, 1, 6)[:, None]
        lines.add_line((np.array([[k, 0, 0]]) + t * np.array([[0, 10, k]])).astype(np.float32))

    return {"outer": outer, "inner": inner, "sphere": sphere, "box": box,
            "cloud": cloud, "lines": lines}


def timed(label, fn, metrics):
    t = time.perf_counter()
    try:
        fn()
        metrics[label] = {"ok": True, "seconds": round(time.perf_counter() - t, 4)}
    except Exception as exc:  # noqa: BLE001 - golden run must not stop on one failure
        metrics[label] = {"ok": False, "error": f"{type(exc).__name__}: {exc}",
                          "seconds": round(time.perf_counter() - t, 4)}


def main(out_dir):
    os.makedirs(out_dir, exist_ok=True)
    metrics = {"vivid3d_file": vivid3d.__file__, "stages": {}}

    t0 = time.perf_counter()
    scene = build_scene()
    metrics["scene_build_seconds"] = round(time.perf_counter() - t0, 4)
    metrics["scene_counts"] = {k: [v.n_vertices, v.n_polygons] for k, v in scene.items()}

    model = vivid3d.Model(list(scene.values()))
    for fmt in FORMATS:
        timed(f"model.{fmt}", lambda f=fmt: model.export(os.path.join(out_dir, "model"), f),
              metrics["stages"])
        for name in ("cloud", "lines", "outer"):
            timed(f"{name}.{fmt}",
                  lambda f=fmt, nm=name: scene[nm].export(os.path.join(out_dir, nm), f),
                  metrics["stages"])

    def blob_glb():
        blob = model.export_to_blob("glb")
        metrics["blob_glb_attrs"] = [a for a in dir(blob) if not a.startswith("_")]
    timed("model.export_to_blob.glb", blob_glb, metrics["stages"])
    timed("model.export_to_obj", lambda: model.export_to_obj(os.path.join(out_dir, "backup")),
          metrics["stages"])

    metrics["total_seconds"] = round(time.perf_counter() - t0, 4)
    metrics["peak_rss_mb"] = round(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss / 1024, 1)
    with open(os.path.join(out_dir, "flow_metrics.json"), "w") as fh:
        json.dump(metrics, fh, indent=2, sort_keys=True)
    failed = [k for k, v in metrics["stages"].items() if not v["ok"]]
    print(f"flow done: {len(metrics['stages']) - len(failed)} ok, {len(failed)} failed {failed}")


if __name__ == "__main__":
    main(sys.argv[1])
