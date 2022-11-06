Materials
=========

VIVID3D fully supports PBR (Physically-Based-Rendering) materials. The PBR model is the 3D standard for beautiful 3D models, and is the main rendering model used in Sketchfab.

For ease of use, we chose a select few parameters that we believe are important when rendering scientific models.

The :doc:`Material API<../generated/vivid3d.Material>` allows you to customize the opacity, metalness, roughness, and emissive properties of the Base Mesh.

We built an interactive demo for material editing, which allows you to play with all 4 Material parameters and see how it affects the displayed mesh.

Interactive Demo
----------------
.. raw:: html

    <iframe
        title="Material Editor" width="100%" height="800px"
        src='../_static/material_viewer.html'></iframe>

Good examples for each material would be :

- Gas Volume : dense(low reflectivity, high opacity)/sparse(high reflectivity, low opacity)
- Liquid     : (high reflectivity, medium opacity)
- Stars      : (High Emissive Strength)

Colormaps
=================

VIVID3D supports all matplotlib supported color-maps for visualizing your models.
Additionally, you have the option to set your mesh to a single color.

List of Colormaps
-----------------

.. image:: https://matplotlib.org/stable/_images/sphx_glr_colormap_reference_001.png
.. image:: https://matplotlib.org/stable/_images/sphx_glr_colormap_reference_002.png
.. image:: https://matplotlib.org/stable/_images/sphx_glr_colormap_reference_003.png
.. image:: https://matplotlib.org/stable/_images/sphx_glr_colormap_reference_004.png
.. image:: https://matplotlib.org/stable/_images/sphx_glr_colormap_reference_005.png
.. image:: https://matplotlib.org/stable/_images/sphx_glr_colormap_reference_006.png
.. image:: https://matplotlib.org/stable/_images/sphx_glr_colormap_reference_007.png

List of Colors
--------------

.. image:: https://tchekbo.files.wordpress.com/2021/02/color.png