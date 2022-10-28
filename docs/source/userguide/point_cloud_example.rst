Point Cloud Example
===================

In this example, we generate a dataset of 3D Points on a uniformly sampled sphere, and pass it to the PointCloud class.

.. jupyter-execute::
    :caption: Uniformly-sampled Point Cloud Sphere
    import vivid3d
    import numpy as np

    def radius(zero):
        u = np.random.normal(0,1)
        v = np.random.normal(0,1)
        w = np.random.normal(0,1)
        norm = (u*u + v*v + w*w)**(0.5)
        x,y,z = u/norm,v/norm,w/norm
        return np.array([x,y,z])

    points = np.apply_along_axis(radius, 0, np.empty(1000))

    pointCloud = vivid3d.PointCloud(points)
    pointCloud.show()