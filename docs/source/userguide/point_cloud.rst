Point Cloud
===================

In this example, we generate a dataset of 3D Points on a uniformly sampled sphere, and display it with a :doc:`../generated/vivid3d.PointCloud` object.

By default, a BaseMesh that does not have the color_field provided will generate a color_field of *np.zeros(n, 1)*, and will
set the default color to *'white'*.

.. jupyter-execute::

    import vivid3d
    import numpy as np

    # generating data
    points = []
    for i in range(1000): # sample random point on sphere
        u = np.random.normal(0,1)
        v = np.random.normal(0,1)
        w = np.random.normal(0,1)
        norm = (u*u + v*v + w*w)**(0.5)
        x,y,z = u/norm,v/norm,w/norm
        points.append([x,y,z])

    point_cloud = vivid3d.PointCloud(points) # Create a PointCloud from the data
    point_cloud.show()

Just like all other BaseMeshes, PointClouds can be given a optional color_field array that maps each point to a colormap.

.. jupyter-execute::

    # Creating an array of 1000 scalars with random value from uniform distribution [0, 1]
    colors = np.random.rand(1000)

    point_cloud_colored = vivid3d.PointCloud(points, colors) # PointCloud with color_field
    point_cloud_colored.show()
