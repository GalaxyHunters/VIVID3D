The Basics
===========

BaseMesh
--------

In VIVID3D, we represent 3D data with the :doc:`../generated/vivid3d.BaseMesh` class, that holds the base attributes of any 3D object.

All BaseMeshes contain:

.. list-table::
    :header-rows: 1

    * - Property
      - Explanation
    * - Vertices
      - The cartesian coordinates that signify the vertices of faces of the BaseMesh geometry.
    * - ColorField
      - A scalar field for each vertex that will be used to map to a color.
    * - Polygons
      - Groups of vertices that are displayed by the 3D rendering software.
    * - Material
      - The visual look displayed by the 3D rendering software.
    * - Colormap
      - The colormap to use for the scalar color field.

There are several different BaseMesh classes, used for representing different data:

.. list-table::
    :header-rows: 1

    * - Class
      - Explanation
    * - :doc:`../generated/vivid3d.PointCloud`
      - For representing each vertex as a separate unconnected points
    * - :doc:`../generated/vivid3d.Lines`
      - For representing groups of vertices as connected lines
    * - :doc:`../generated/vivid3d.Mesh`
      - For representing groups of vertices as polygonal faces

Later we will go in depth how to create and manipulate each of these objects.

Model
-----

The :doc:`../generated/vivid3d.Model` class is simply a collection of BaseMeshes, primarily used to export and view meshes in a combined scene.

The creation of a model is as simple as:

.. code-block:: python

    model = vivid3d.Model()

Additionally, a model can be instantiated with a single or list of BaseMeshes

.. code-block:: python

    model = vivid3d.Model(mesh)

.. code-block:: python

    model = vivid3d.Model([mesh1, mesh2, mesh3])

BaseMeshes can be appended to the model:

.. code-block:: python

    model.add_mesh(mesh)

.. code-block:: python

    model.add_meshes([mesh1, mesh2])

The 3D Viewer
=============

Both BaseMeshes and Models can be viewed using the built-in 3D viewer. You can run this viewer both in jupyter notebooks,
where it will open in a IPython widget beneath the cell, or from command line, where it will open in a new webpage.

To view your BaseMesh or Model, simply run:

.. code-block:: python

    mesh.show()

Or:

.. code-block:: python

    model.show()

The Viewer has simple controls for re-orientating in 3D space by rotating or scaling the model.
Animated Models can be played as well in the Viewer.
Finally you can save the model to a compact .glb file with the "Export GLB" button.

Saving Your Models
==================

You can save both BaseMeshes and Models using the export() function. It accepts 2 parameters:

.. list-table::
    :header-rows: 1

    * - Property
      - Explanation
    * - output_file
      - For representing each vertex as a separate unconnected points
    * - file_type
      - For representing groups of vertices as connected lines

Supported File Types
--------------------

VIVID3D supports a wide range of industry standard 3D file formats, usable in any 3D software or platform.

The recommended file format is GLB, intended for minimal file size, integration with nearly every 3D software. Unlike other
file formats, GLB stores all the model data in a single file, making it a much cleaner file format to use than others.

Here is the list of supported 3D file formats:

* `GLB <https://docs.fileformat.com/3d/glb/>`_
* `GLTF <https://docs.fileformat.com/3d/gltf/>`_
* `OBJ <https://docs.fileformat.com/3d/obj/>`_
* `FBX <https://docs.fileformat.com/3d/fbx/>`_

Additional, untested file formats:

* `PLY <https://docs.fileformat.com/3d/ply/>`_
* `3DS <https://docs.fileformat.com/3d/3ds/>`_
* `STL <https://docs.fileformat.com/cad/stl/>`_
* `STLB <https://docs.fileformat.com/cad/stl/#stl-binary-format>`_
* `COLLADA <https://docs.fileformat.com/3d/dae/>`_

Examples
--------

Saving the model to a folder, using the default *file_type = glb*.

This creates a *export.glb* file at the */models/* folder.

.. code-block:: python

    model.export('/models/export')

Similarly, this saves the *file_type = obj* files as: *export.obj, export.mtl, export_texture.png* at the */models/* folder.

.. code-block:: python

    model.export('/models/export', 'obj')

If you do not wish to save directly to a file, you may omit the output_file variable to save the files in a :doc:`../generated/vivid3d.BlobData` instance.

This example writes using the default *file_type = glb* to the blob_data instance.

.. code-block:: python

    blob_data = model.export()