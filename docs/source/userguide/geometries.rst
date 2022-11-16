Geometries
==============

VIVID3D supplies multiple basic geometric objects that you can use to improve your models, or test basic VIVID3D features.

They are detailed in the :doc:`API Reference: Geometry <../generated/vivid3d.Geometries>` section.

.. jupyter-execute::
    :raises:

    import vivid3d

    plane = vivid3d.create_plane(position= [0,0,0], size=3)
    box = vivid3d.create_box(position= [0,0,0], size=[1,3,2])
    sphere = vivid3d.create_sphere(radius= 3.9, color='red', opacity=0.8)
    grid = vivid3d.create_grid(size=10, num_of_ticks = 5, tick_size = 2)

    geometries = vivid3d.Model([plane, box, sphere, grid[0], grid[1]])
    geometries.show()
