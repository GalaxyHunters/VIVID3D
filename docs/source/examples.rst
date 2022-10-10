.. _examples:

Code Examples
#############

.. _simple_example:

Simple example
==============

place holder1 - pyramid

.. _simple_example:

Advanced example
================

place holder - yt enzo galaxy


Shapes example
==============

The package has multiple shape functions that you can use to improve your models,
they are detailed in the Functios Documentation.

Simple code example of adding shapes to an existing model:

.. code-block:: python
   :caption: shape example
   :linenos:
   
   circle = vivid3d.create_cube(position= [0,0,0], size=3)
   cube = vivid3d.create_sphere(radius= 3.9, color='red', alpha=0.8)
   grid = vivid3d.create_grid(size=10, num_of_ticks = 5, tick_size = 2)

   model.add_meshes([cube, sphere, grid[0], grid[1]])

   model.export(Path, "obj") #Export to OBJ file format
