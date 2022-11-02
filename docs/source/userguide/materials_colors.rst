Materials
=========

VIVID3D currently supports heavily simplified PBR (Physically-Based-Rendering) materials.
For ease of use, we chose a select few parameters that we believe are important when rendering scientific models.

The :doc:`Material API<../generated/vivid3d.Material>` allows you to customize the opacity, reflectivity, and emissive properties of the Base Mesh.

1. Opacity affects the transparency of a mesh.

.. image:: https://docs.unrealengine.com/5.0/Images/designing-visuals-rendering-and-graphics/materials/tutorials/transparency/Trans_Opacity_Settings.webp

2. Shininess affects the reflectivity of a mesh.

.. image:: https://www.google.com/url?sa=i&url=https%3A%2F%2Flifeisforu.tistory.com%2F387&psig=AOvVaw0_cFpcNnULUENJDRfJDXkg&ust=1667349146269000&source=images&cd=vfe&ved=0CA0QjRxqFwoTCIizid3di_sCFQAAAAAdAAAAABA1

3. The Emissive properties are used to create self-illuminating meshes.

.. image:: https://i.ytimg.com/vi/Q45HW6hsJzQ/maxresdefault.jpg

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