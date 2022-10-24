YT Enzo galaxy example
======================

In this example, we will make a model of the Enzo-isolated galaxy

We start by getting the data and sorting it:

.. code-block:: python
   :caption: setting up data
   :linenos:

    import yt   # Import packages
    import numpy as np
    import vivid3d


    ds = yt.load_sample("IsolatedGalaxy")   # Load example data from yt

    sphere = ds.sphere(center="max", radius=(47, "kpc"))   # Create sphere object 

    dens = np.array(sphere[("gas", "density")]) # Take densities of gas at every point of the sphere
    temp = np.log10(np.array(sphere[("gas", "temperature")])) # Take the temperature of gas at every point of the sphere and normalize

    pos_x = sphere[("index", "x")].in_units('kpc') # Create list of x grid values
    pos_y = sphere[("index", "y")].in_units('kpc') # Create list of y grid values
    pos_z = sphere[("index", "z")].in_units('kpc') # Create list of z grid values

    points = np.array(list(zip(pos_x, pos_y, pos_z)))

    # List of densities we want to create meshes for
    densities = [1e-24, 1e-25, 1e-26, 1e-27, 5e-28]

    #make masks and opacity rates
    masks = []
    opacities = []
    alpha_jumps = 0.8/len(densities)
    for i in range(len(densities)): 
      masks.append(dens > densities[i])
      opacities.append(1-alpha_jumps*i)

Once we have the data we can use vivid3d to compute our model.

This process is expensive ram-wise and will take a while.


one liner
---------
.. code-block:: python
   :caption: one line function
   :linenos:

    vivid3d.make_model(input_points=points, masks = masks, output_path = path + "Enzo_Galaxy.obj",
        color_field = temp, color_field_min = np.log10(10000), color_field_max = color_field_max=max(temp),
        opacity = opacities, file_type = "obj", noise_displacement = 0)

step by step
------------
.. code-block:: python
   :caption: making a model step by step
   :linenos:

    #make volume and run voronoi
    volume = vivid3d.VoronoiVolume(points, temp, color_field_min=np.log10(10000), color_field_max=max(temp) ,noise_displacement=0)
    volume.create_surface() #a very expensive action 

    #get matplotlib color map
    clm = matplotlib.cm.gnuplot2

    model = vivid3d.Model()
    for i in range(len(masks)):
      try:
        mesh = volume.to_mesh(masks[i], "Enzo_"+str(densities[i]), opacities[i])
        mesh.smooth(10,0.7,0) #smooth the mesh
        mesh.set_color_map(clm)
        model.add_mesh(mesh)
      finally:
        pass

    model.export(path+"Enzo_Galaxy.obj", "obj")



