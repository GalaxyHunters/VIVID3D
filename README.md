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

## Highlights
View the [Model Gallery](https://vivid.readthedocs.io/en/latest/gallery.html) to see what VIVID3D is capable of. 

## Documentation
Our Documentation is hosted at [Read The Docs](https://vivid.readthedocs.io/en/latest/). This contains usage guides and examples, our API reference, and a gallery of our favourite models.

# Installation and Usage
## Installation
VIVID3D can be installed from PyPI using pip on `Python >= 3.7`

```
pip install vivid3d
```
This might take a few minutes, and is only tested on linux as of now.

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
