![image](https://github.com/GalaxyHunters/Vivid/blob/01788a5e3656dbaa048a48215a290dfb7f3dc831/vivid-cover.png?raw=true)

# VIVID3D
[![Read the Docs](https://img.shields.io/readthedocs/vivid)](https://vivid.readthedocs.io/en/latest/)
[![PyPI](https://img.shields.io/pypi/v/vivid3d?label=pypi)](https://pypi.org/project/vivid3d/)
[![GitHub](https://img.shields.io/github/license/GalaxyHunters/vivid)](https://github.com/GalaxyHunters/Vivid/blob/master/LICENSE)
[![PyPI - Python Version](https://img.shields.io/pypi/pyversions/vivid3d)](https://pypi.org/project/vivid3d/)
[![GitHub contributors](https://img.shields.io/github/contributors/GalaxyHunters/vivid)](https://github.com/GalaxyHunters/vivid/graphs/contributors/)

We present VIVID3D, an innovative method for research using 3D technology.

# Overview
VIVID3D is an all new python package for simple scientific visualization and creation of 3D models. 
Visualize particle simulations or volumetric scans like never before, with Voronoi Tesselation for realistic volume representation in 3D.

VIVID3D was tested in our cosmology research group by producing models from simulations for researchers, supporting new scientific insights about the evolution of galaxies and structures of the Universe.

## Features
- **Fast execution** - C++ wrapped with [pybind11](https://github.com/pybind/pybind11)
- **Smart Isosurfaces** - Using a [3D Voronoi Algorithm](https://doi.org/10.1088/0067-0049/216/2/35) implementation by Dr. Elad Steinnberg for visualization of volumetric data
- **Support for industry standard file formats** - Using the [Open Asset Import Library](https://github.com/assimp/assimp)
- **Point Clouds**
- **Vector Fields**
- **Interactive 3D Viewer**
- **Scientific Analysis of Meshes**

## What's new in 1.1.0
A modernization release: the build, the dependencies and the packaging were rebuilt, and several long-standing
install and compatibility problems were fixed. Exported models are unchanged: every step was checked against a
regression baseline (glb / obj / fbx, structurally compared) on Linux and Windows.

- **Platforms and Python** - Windows (MSVC) is now supported alongside Linux, and Python 3.10 - 3.14 are tested
  (previously `>= 3.7` was claimed but untested).
- **Modern build** - one `pyproject.toml` built with [scikit-build-core](https://github.com/scikit-build/scikit-build-core)
  replaces `setup.py` / `MANIFEST.in`; the version lives in a single place.
- **Smaller and faster to build** - the compiled package is one native binary: the wheel shrank from 12.8 MB to about
  2 MB, a clean source build takes roughly half as long, and the repository's source went from 217 MB to 80 MB.
- **Dependencies** - assimp 6.0.6 (an installed copy, or a pinned and checksum-verified download) replaces a
  patched, vendored 5.2.5; pybind11 now comes from PyPI instead of a vendored snapshot. The local assimp patches
  were replaced by code in VIVID3D itself, so transparency and materials in glTF exports are preserved.
- **Fixes** - `pip install vivid3d` now installs numpy and matplotlib (a bare install used to fail on import);
  naming a colormap (`mesh.colormap = "viridis"`) works with matplotlib 3.9+ and a memory-safety bug behind it is
  fixed; a compile clash with Python 3.10 is fixed; the library no longer links `libpython` or leaks internal
  symbols into the host process.

Runtime performance and memory work (profiling, mesh decimation, leak fixes) is the next step.

## Highlights
View the [Model Gallery](https://vivid.readthedocs.io/en/latest/gallery.html) to see what VIVID3D is capable of. 

## Documentation
Our Documentation is hosted at [Read The Docs](https://vivid.readthedocs.io/en/latest/). This contains usage guides and examples, our API reference, and a gallery of our favourite models.

# Installation and Usage
## Installation
VIVID3D can be installed from PyPI using pip on `Python >= 3.10`

```
pip install vivid3d
```
Installing builds the C++ core from source, which needs a C++17 compiler (GCC/Clang on Linux and macOS, the Visual Studio 2022 Build Tools with the "Desktop development with C++" workload on Windows) and an internet connection (assimp is downloaded and built); expect a few minutes. It is tested on Linux and Windows with Python 3.10 - 3.14.

## Usage
Learn the fundamentals and advanced model creation examples with the [User Guide](https://vivid.readthedocs.io/en/latest/examples.html).

# About
## Authors
**Project Leads**
- [Tomer Nussbaum](https://github.com/tussbaum)
- [Naftali Deutsch](https://github.com/rslanis)
- [Adam Beili](https://github.com/Beilinson)
- [Zohar Milman](https://github.com/ZoharMilman)

**Special Thanks**
- [Raphael Buzaglo](https://github.com/raphae2118)
- [Oz Weizzman](https://github.com/OzW1234)
- [Alice Temkin](https://github.com/lazy-fox304)

[Contributors](https://github.com/GalaxyHunters/vivid/graphs/contributors/)

## License
VIVID3D is provided under a BSD-3 license that can be found in the [LICENSE](https://github.com/GalaxyHunters/Vivid/blob/master/LICENSE) file. By using, distributing, or contributing to this project, you agree to the terms and conditions of this license.
