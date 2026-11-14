"""Parse exported 3D files into structural metrics (stdlib only, no assimp).

Metrics describe the *form* of the result (counts, bounding boxes, materials),
not exact bytes, so they tolerate floating-point noise.
"""
import json
import os
import re
import struct
import zlib


def _bbox(points):
    if not points:
        return None
    return [[round(min(p[i] for p in points), 4) for i in range(3)],
            [round(max(p[i] for p in points), 4) for i in range(3)]]


# --------------------------------------------------------------------------- GLB
def analyze_glb(path):
    with open(path, "rb") as fh:
        data = fh.read()
    magic, version, _length = struct.unpack_from("<4sII", data, 0)
    if magic != b"glTF":
        return {"error": "not a GLB"}
    clen, ctype = struct.unpack_from("<II", data, 12)
    doc = json.loads(data[20:20 + clen])
    acc = doc.get("accessors", [])
    out = {"version": version, "extensionsUsed": sorted(doc.get("extensionsUsed", [])),
           "n_nodes": len(doc.get("nodes", [])), "n_meshes": len(doc.get("meshes", [])),
           "n_images": len(doc.get("images", [])), "n_textures": len(doc.get("textures", [])),
           "primitives": []}
    lo, hi = [None] * 3, [None] * 3
    for mesh in doc.get("meshes", []):
        for prim in mesh.get("primitives", []):
            pos = acc[prim["attributes"]["POSITION"]] if "POSITION" in prim["attributes"] else None
            idx = acc[prim["indices"]]["count"] if "indices" in prim else None
            out["primitives"].append({"mode": prim.get("mode", 4), "vertices": pos["count"] if pos else 0,
                                      "indices": idx, "material": prim.get("material"),
                                      "attributes": sorted(prim["attributes"])})
            if pos and "min" in pos:
                lo = [pos["min"][i] if lo[i] is None else min(lo[i], pos["min"][i]) for i in range(3)]
                hi = [pos["max"][i] if hi[i] is None else max(hi[i], pos["max"][i]) for i in range(3)]
    out["bbox"] = [[round(v, 3) for v in lo], [round(v, 3) for v in hi]] if lo[0] is not None else None
    out["primitives"].sort(key=lambda p: (p["mode"], p["vertices"], p["indices"] or 0))
    out["materials"] = sorted(doc.get("materials", []), key=lambda m: json.dumps(m, sort_keys=True))
    return out


# --------------------------------------------------------------------------- OBJ
def analyze_obj(path):
    counts = {"v": 0, "vt": 0, "vn": 0, "f": 0, "p": 0, "l": 0}
    verts, styles, mtllib, usemtl = [], set(), [], set()
    with open(path, errors="replace") as fh:
        for line in fh:
            parts = line.split()
            if not parts:
                continue
            tag = parts[0]
            if tag in counts:
                counts[tag] += 1
            if tag == "v":
                verts.append([float(x) for x in parts[1:4]])
            elif tag in ("f", "p"):
                styles.add((tag, re.sub(r"\d+", "#", parts[1]) if len(parts) > 1 else ""))
            elif tag == "mtllib":
                mtllib.append(parts[1])
            elif tag == "usemtl":
                usemtl.add(parts[1])
    out = {"counts": counts, "bbox": _bbox(verts), "face_styles": sorted(f"{t}:{s}" for t, s in styles),
           "n_usemtl": len(usemtl), "materials": {}}
    for lib in mtllib:
        mtl = os.path.join(os.path.dirname(path), lib)
        if not os.path.exists(mtl):
            continue
        cur = None
        with open(mtl, errors="replace") as fh:
            for line in fh:
                parts = line.split()
                if not parts:
                    continue
                if parts[0] == "newmtl":
                    cur = out["materials"].setdefault(" ".join(parts[1:]), {})
                elif cur is not None:
                    cur[parts[0]] = " ".join(parts[1:])
    return out


# --------------------------------------------------------------------------- FBX (binary)
def _read_props(buf, pos, count):
    props = []
    for _ in range(count):
        code = chr(buf[pos]); pos += 1
        if code in "YCIFDL":
            fmt = {"Y": "<h", "C": "<?", "I": "<i", "F": "<f", "D": "<d", "L": "<q"}[code]
            props.append(struct.unpack_from(fmt, buf, pos)[0]); pos += struct.calcsize(fmt)
        elif code in "fdlib":
            n, enc, clen = struct.unpack_from("<III", buf, pos); pos += 12
            raw = buf[pos:pos + clen]; pos += clen
            if enc == 1:
                raw = zlib.decompress(raw)
            size = {"f": 4, "d": 8, "l": 8, "i": 4, "b": 1}[code]
            props.append(("array", code, n, raw if n * size == len(raw) else b""))
        elif code in "SR":
            n = struct.unpack_from("<I", buf, pos)[0]; pos += 4
            raw = buf[pos:pos + n]; pos += n
            props.append(raw.decode("latin1") if code == "S" else raw)
        else:
            raise ValueError(f"unknown FBX property type {code!r}")
    return props, pos


def _read_nodes(buf, pos, end, wide):
    nodes = []
    head = 25 if wide else 13
    while pos < end:
        fmt = "<QQQ" if wide else "<III"
        end_off, nprops, plen = struct.unpack_from(fmt, buf, pos)
        hsize = struct.calcsize(fmt)
        if end_off == 0:
            return nodes, pos + head
        nlen = buf[pos + hsize]
        name = buf[pos + hsize + 1: pos + hsize + 1 + nlen].decode("latin1")
        ppos = pos + hsize + 1 + nlen
        props, after = _read_props(buf, ppos, nprops)
        children = []
        if after < end_off:
            children, _ = _read_nodes(buf, after, end_off, wide)
        nodes.append({"name": name, "props": props, "children": children})
        pos = end_off
    return nodes, pos


def _arr(prop):
    _, code, n, raw = prop
    return list(struct.unpack(f"<{n}{code}", raw)) if raw else []


def analyze_fbx(path):
    with open(path, "rb") as fh:
        buf = fh.read()
    if not buf.startswith(b"Kaydara FBX Binary"):
        return {"error": "not binary FBX", "size": len(buf)}
    version = struct.unpack_from("<I", buf, 23)[0]
    nodes, _ = _read_nodes(buf, 27, len(buf), version >= 7500)
    top = {n["name"]: n for n in nodes}
    out = {"version": version, "geometries": [], "materials": [], "object_types": {}}
    for child in top.get("Objects", {}).get("children", []):
        out["object_types"][child["name"]] = out["object_types"].get(child["name"], 0) + 1
        if child["name"] == "Geometry":
            sub = {c["name"]: c for c in child["children"]}
            v = _arr(sub["Vertices"]["props"][0]) if "Vertices" in sub else []
            pts = [v[i:i + 3] for i in range(0, len(v), 3)]
            pvi = _arr(sub["PolygonVertexIndex"]["props"][0]) if "PolygonVertexIndex" in sub else []
            out["geometries"].append({"vertices": len(pts), "polygon_indices": len(pvi),
                                      "polygons": sum(1 for i in pvi if i < 0), "bbox": _bbox(pts),
                                      "layers": sorted(c["name"] for c in child["children"] if c["name"].startswith("Layer"))})
        if child["name"] == "Material":
            mat = {"name": str(child["props"][1]).split("\x00")[0] if len(child["props"]) > 1 else ""}
            for c in child["children"]:
                if c["name"] == "Properties70":
                    for p in c["children"]:
                        if p["props"] and len(p["props"]) >= 5:
                            mat[str(p["props"][0])] = [round(x, 4) if isinstance(x, float) else x for x in p["props"][4:]]
            out["materials"].append(mat)
    out["geometries"].sort(key=lambda g: (g["vertices"], g["polygons"]))
    out["materials"].sort(key=lambda m: m["name"])
    return out


ANALYZERS = {".glb": analyze_glb, ".obj": analyze_obj, ".fbx": analyze_fbx}


def analyze_dir(directory):
    results = {}
    for name in sorted(os.listdir(directory)):
        ext = os.path.splitext(name)[1].lower()
        if ext in ANALYZERS:
            path = os.path.join(directory, name)
            try:
                res = ANALYZERS[ext](path)
            except Exception as exc:  # noqa: BLE001
                res = {"error": f"{type(exc).__name__}: {exc}"}
            res["size_bytes"] = os.path.getsize(path)
            results[name] = res
    return results
