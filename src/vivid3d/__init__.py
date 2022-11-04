"""
VIVID3D
=======

VIVID3D is an all new python package for simple scientific visualization and creation of 3D models.
Visualize particle simulations or volumetric scans like never before, with Voronoi Tesselation for realistic volume
representation in 3D.

Documentation
-------------
For in-depth documentation, user guides, and beautiful models created with VIVID3D, refer to https://vivid.readthedocs.io/en/latest/.

Or use the built-in `help()` function on the class or module of interest.

Core API
========
Meshes and Models
-----------------
**BaseMesh**
    The base VIVID3D class
**Model**
    A collection of BaseMeshes, for advanced visualizations
**PointCloud, Lines, Mesh (base: BaseMesh)**
    Classes for visualizing any form of 3 dimensional data
**VoronoiVolume**
    Our advanced method for visualizing volumetric datasets with iso-surfaces created using this class

Visual
------
**Material**
    Visualize your models with advanced physically-based-rendering (PBR) Materials.
**Colormaps**
    Full inte**gration with matpotlib colormaps : https://matplotlib.org/stable/gallery/color/colormap_reference
**Colors**
    Full support for all 140 CSS colors : https://htmlcolorcodes.com/color-names/

Animations
----------
**Animation**
    Animate your models with easy transformation functions
**StopMotionAnimation**
    Animate your models as stop-motion-animation for time-series data

Utilities
=========
**Geometry**
    Basic geometric objects to improve your models, or test basic VIVID3D features
**BlobData**
    Save models to in-memory blobs
**Logger**
    Configure the logging outputs using `config_logger()`

Viewer
======
**3D Viewer**
    View your models in IPython or web-browser using the `.show()` function on any model or mesh
**2D VolumetricViewer**
    2D 3-axis IPython-only widget for pre-selecting iso-surfaces from numpy 3D matrix
"""

from .viewer import *
from ._core import *
from ._details import __version__
from ._core import __doc__

__all__ = ['_core', 'viewer']