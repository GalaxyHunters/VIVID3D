Geometries Example
==============

The package has multiple Geometric objects that you can use to improve your models,
they are detailed in the Geometries Documentation.

.. jupyter-execute::

    import vivid3d
    plane = vivid3d.create_plane(position= [0,0,0], size=3)
    box = vivid3d.create_box(position= [0,0,0], size=[1,3,2])
    sphere = vivid3d.create_sphere(radius= 3.9, color='red', opacity=0.8)
    grid = vivid3d.create_grid(size=10, num_of_ticks = 5, tick_size = 2)

    geometries = vivid3d.Model([box, sphere])
    # geometries.show()

cool