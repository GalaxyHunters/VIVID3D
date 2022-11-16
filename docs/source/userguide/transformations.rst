Transformations
===============

All VIVID3D :doc:`BaseMesh<../generated/vivid3d.BaseMesh>` derived classes contain methods for basic transformations.

For the purposes of this tutorial we will use a cube primitive, the original will be left in its starting position and shown in blue, while the transformed cube will be shown in red.

.. jupyter-execute::
  
  import vivid3d
  
  starting_cube = vivid3d.create_cube(color='blue', opacity=0.4)
  
Move
----

.. jupyter-execute::
  
  transformed_cube = vivid3d.create_cube(color='red', opacity=0.4)
  transformed_cube.move([2, 1, 3])
  vivid3d.Model([starting_cube, transformed_cube]).show()

Scale
-------

.. jupyter-execute::
  
  transformed_cube = vivid3d.create_cube(color='red', opacity=0.4)
  transformed_cube.scale([1.5, 0.5, 2])
  vivid3d.Model([starting_cube, transformed_cube]).show()

Rotate
--------

.. jupyter-execute::
  
  transformed_cube = vivid3d.create_cube(color='red', opacity=0.4)
  transformed_cube.rotate([1, 0, 0], 0.785)
  vivid3d.Model([starting_cube, transformed_cube]).show()
  
