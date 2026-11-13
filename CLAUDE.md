# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

VIVID3D (`vivid3d` on PyPI) is a Python package for scientific 3D visualization (Voronoi-tessellation iso-surfaces, point clouds, lines, animations). The heavy lifting is C++17, exposed to Python through pybind11, built with scikit-build + CMake.

## Build / install / test

- Dev install (compiles C++, slow — assimp and the Voronoi lib are built from source): `pip install -e .` or `python setup.py develop`. Build deps are in `requirements.txt` / `pyproject.toml` (scikit-build, cmake>=3.24, ninja on non-Windows). README states it is only tested on Linux.
- assimp is an external dependency, chosen by the CMake option `VIVID_ASSIMP` (`cmake/VividAssimp.cmake`): `AUTO` (default: use an installed assimp if it is >= `VIVID_ASSIMP_MIN_VERSION` and not in `VIVID_ASSIMP_EXCLUDED_VERSIONS`, else fetch the pinned release), `SYSTEM` (installed only, linked dynamically; point at it with `CMAKE_PREFIX_PATH`), `FETCH` (download and build, linked statically). Pass options through scikit-build with `SKBUILD_CONFIGURE_OPTIONS="-DVIVID_ASSIMP=SYSTEM"`. The tested version is 6.0.6; assimp 6.0.3/6.0.4 (what Ubuntu 26.04 ships) crash on FBX export and are rejected. Vivid works around assimp behaviour in `src/ImportExport/AssimpImportExport.cpp` (`SetSceneFlags`, `PrepareSceneForFormat`) instead of patching assimp. The branch `stable-2026` still contains the old vendored assimp 5.2.5 build.
- The compiled extension is `vivid3d._core`, installed into `src/vivid3d/` (`cmake_install_dir='src/vivid3d'`). C++ changes require rebuilding/reinstalling; pure-Python changes under `src/vivid3d/` do not.
- There is no pytest suite or CI workflow. `tests/test.py` is a manual smoke script (builds a VoronoiVolume, takes iso-surfaces, exports to `test_models/`). Run it with `python tests/test.py`; it needs the extension built.
- Golden-output regression tool, `tools/golden/` (Linux; stdlib-only driver). `python3 tools/golden/golden.py run --repo . --label <name>` does a clean timed `pip wheel`, installs into a fresh venv, checks import and linked libs, runs a deterministic scene (`flow.py`) exporting glb/obj/fbx, and writes `~/work/golden/<name>/summary.json` plus the exported models. `golden.py compare <baseline.json> <candidate.json>` compares *structure* (counts, bboxes, materials; floats use a tolerance) and reports build time, package size and import time as informational ratios. The reference result for the vendored-assimp baseline is `tools/golden/baselines/baseline-vendored.json`. `tools/golden/viewer/index.html` is a side-by-side three.js viewer: serve a folder that contains `golden/<run>/exports/` (e.g. `python -m http.server` from the parent of the repo) and open it with `?a=<run>&b=<run>&file=model.glb`.
- Supported Pythons: 3.10-3.14 (`python_requires` in `setup.py`), all verified with `tools/golden`; numpy >= 1.21.3 and matplotlib >= 3.5 are tested as the lower bounds (`golden.py run --run-dep=numpy==1.21.6 --run-dep=matplotlib==3.5.3`). Keep names in `src/Wrapping/` free of clashes with CPython/pybind11 globals: `wrapper.cpp` has `using namespace vivid;`, and e.g. CPython 3.10 defines a global `CFrame`, so Vivid's is written `vivid::CFrame`. When touching the wrapper, check every supported Python, not just the newest.
- Docs: Sphinx, sources in `docs/source/` (config `docs/source/conf.py`, built by Read the Docs per `.readthedocs.yaml`). Local build: `cd docs && make html`.
- `CMakeLists.txt` only builds the `_core` Python module when `SKBUILD` is defined; a plain CMake configure produces just the shared `vivid` library.

## Architecture

Three layers, all of which usually need to change together when the API changes:

1. **C++ core, `src/`** (namespace `vivid`), compiled into the shared lib `vivid`. Note `CMakeLists.txt` lists every source file explicitly, so **new `.cpp` files must be added there**.
   - `ModelBuilder/`: domain classes. `ModelComponent` is the base for `Mesh`, `Line`, `PointCloud`; `Model` holds a collection of components; `VoronoiVolume`, `MarchingCubesSurface`, `SurfByFunc`, `Shapes` generate geometry; `Animation` / `StopMotionAnimation` build on `Model`; `Vivify.*` is the entry for model construction.
   - `ImportExport/`: OBJ and Assimp (glTF/GLB, etc.) exporters, `BlobData` (in-memory export), colormap-to-PNG.
   - `Textures/`, `Utils/` (logging, constrained numbers, noise), `Extra/Analysis`.
2. **Third-party code, `lib/`**: `SurfacingAlgorithms/huji-rich-Elad3DFast` (the 3D Voronoi implementation, built as target `voronoi`), `ImportExport/lodepng`, `MeshDecimation`, `MyBoost`. Treat as vendored. `pybind11/` at the repo root is vendored too.
3. **Python bindings, `src/Wrapping/wrapper.cpp`** (one ~1200-line `PYBIND11_MODULE(_core, ...)`) plus `TypeCasting.h` for numpy/C++ conversions. Docstrings for the public API live here as raw string literals, so Sphinx docs are generated from them.
4. **Python package, `src/vivid3d/`**: `__init__.py` re-exports `_core` classes and holds the package docstring; `viewer/` has the IPython/web 3D viewer (`model_viewer.py`, `template.html`) and the 2D `VolumetricViewer`; `utils.py`; `_details.py` holds `__version__`, which `setup.py` reads (note the version is also hard-coded in `CMakeLists.txt` `project(... VERSION)`).

Per `.github/CONTRIBUTING.md`: any change to API types, signatures or classes also requires updating the wrapper in `src/Wrapping/` and the docs in `docs/source/` (`docs/source/generated/*.rst` per class, `api_reference.rst`). PRs are made from forks against `master`.
