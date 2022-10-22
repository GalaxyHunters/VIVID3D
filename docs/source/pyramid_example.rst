Simple ball example
===================


First, we need model data to test.
In this example, we generate a 3D grid with a Distance attribute for each point.
From this data, we will make a ball

.. code-block:: python
   :caption: creating a grid dataset
   :linenos:

    import vivid3d
    import numpy as np


    #make the data
    BOX_SIZE = 30
    zero = np.array([0,0,0])

    points = []
    color = []
    distance = []

    for i in range(-BOX_SIZE, BOX_SIZE, 2):
        for j in range(-BOX_SIZE, BOX_SIZE, 2):
            for z in range(-BOX_SIZE, BOX_SIZE, 2):
                point = np.array([i, j, z])
                points.append(point)
                distance.append(np.linalg.norm(point-zero))
                color.append(j)

Now, with this data we can call vivid and make a model.

The one liner
-------------

.. code-block:: python
   :caption: The one liner
   :linenos:

    vivid3d.make_model(
        input_points=points, #the 3D points form which faces are made
        surface_field = distance, #surface_field, Value for each input_point to later make a surface 
        surface_threshold = 20, #The threshold value for surface_field, resulting in the surface.
        output_path = path + "/MyFirstModel", #optinal - output path for model
        color_field = color, #optinal - Color value for each input point
        color_field_min = -BOX_SIZE, #optinal - Min value for the color field, default is min(color_field)
        color_field_max = BOX_SIZE, #optinal - Max value for the color field, default is max(color_field)
        label =  "My_First_Model", #optinal - label for the model, some formats support it
        opacity = 0.8, #optinal - opacity factor for the model 0 is See throgh
        file_type = "gltf2" #optinal - file format for the export, basic is "gltf2"
        noise_displacement = 0.0001) #Optional, add noise to the model, improves Voronoi performance, recommend leaving as default

Here we used the distance from 0,0,0 as a field value and with a threshold of 20, we made a surface. 

Similarly, you can input a Mask (bool array) and the surface will be created between true (blue in this 2D example) and false (red).

.. image:: voronoi_diagram.jpg
   :width: 400
   :alt: voronoi_diagrem


More control
------------

For the second method, we will use a mask and utilize some other abilities of the code.

.. code-block:: python
   :caption: The advanced method
   :linenos:

   #we will make two masks for two meshes
   mask1 = np.array(distance) > 25 
   mask2 = np.array(distance) > 15

   voronoi = vivid3d.VoronoiVolume(points, color) #make VoronoiVolume obj
   voronoi.create_surface(); #create the faces by running Voronoi, this might be a heavy function

   #make a mesh and take a surface by mask
   mesh1 = voronoi.to_mesh(mask1, "Mesh", 0.5)
   mesh2 = voronoi.to_mesh(mask2, "Mesh", 1)
   mesh1.smooth(10,0.7,0)
   
   #make a model with our two meshes
   model = vivid3d.Model([mesh1,mesh2])
   
   #lets add a grid to our model
   grid = vivid3d.create_grid(30,15,1)
   model.add_meshes(grid)
   
   #export
   model.export(path+"/MyModel", "gltf2")

