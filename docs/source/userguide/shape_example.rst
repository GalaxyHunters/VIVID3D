Geometries Example
==============

The package has multiple Geometric objects that you can use to improve your models,
they are detailed in the Geometries Documentation.

.. jupyter-execute::
   :caption: Geometries

    plane = vivid3d.create_plane(position= [0,0,0], size=3)
    box = vivid3d.create_box(position= [0,0,0], size=[1,3,2])
    sphere = vivid3d.create_sphere(radius= 3.9, color='red', alpha=0.8)
    arrow = vivid3d.create_arrow(radius= 3.9, color='red', alpha=0.8)
    grid = vivid3d.create_grid(size=10, num_of_ticks = 5, tick_size = 2)

    geometries = vivid3d.Model([plane, box, sphere, arrow, ellipsoid, grid[0], grid[1]])
    geometries.show()
