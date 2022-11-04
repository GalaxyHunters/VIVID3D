"""
######
Viewer
######

VIVID3D Module for visualizing 3D models in IPython or web browser. Also includes VolumetricViewer class.

"""

from .model_viewer import show
from .volumetric_viewer import VolumetricViewer

__all__ = ['show', "VolumetricViewer"]