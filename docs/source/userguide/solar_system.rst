Solar System
============

This tutorial makes use of :doc:`Geometries<./geometries>` and :doc:`Animations<./animations>` to create an animated
model of the Solar System, with the sun and planets represented as :ref:`Spheres</generated/geometries:create_sphere>` and the asteroid belts with :doc:`PointClouds<./point_cloud>`

Data for radii, orbital distances and orbital speeds is obtained from NASA, and scaled appropriately in order to visualize with maximal fidelity.

.. jupyter-execute::
    
    import vivid3d
    import numpy as np
    import pandas as pd

    # Making the Base Data
    names     = ['Sun', 'Mercury', 'Venus', 'Earth', 'Mars', 'Jupyter', 'Saturn', 'Uranus', 'Neptune']
    distances = [0, 1.8, 2, 2.4, 3, 7.4, 12.6, 24.2, 38]
    radii     = [1, 0.05, 0.125, 0.128, 0.07, 0.35, 0.3, 0.255, 0.255]
    colors    = ['orange', 'coral', 'green', 'blue', 'red', 'tan', 'yellow', 'blue', 'skyblue']

    data = pd.DataFrame(list(zip(names, distances, radii, colors)), columns=['name', 'orbit', 'radius', 'color'])
    display(data)
    
We'll define a helper functions to help create the objects, and populate our `solar_system` `Animation` object with them:

.. jupyter-execute::

    planet_material = vivid3d.Material(1, 0, 1) # Planets are mostly rough surfaces with non-conductive (non-metal surfaces)

    def create_solar_object(orbit, radius, color, name):
        sphere = vivid3d.create_sphere([0,0,0], radius, 50, 50, color, name) # creating the planetary object
        a = 2 * math.pi * np.random.normal();
        sphere.move([orbit * math.cos(a), 0, orbit * math.sin(a)]) # placing it randomly along its orbit
        sphere.material = planet_material # setting the material
        return vivid3d.Model(sphere)
    
    # Creating the animation object:
    solar_system = vivid3d.Animation()
    for i, (name, orbit, radius, color) in data.iterrows():
        solar_system.add_model(create_solar_object(orbit, radius, color, name))

    # Our First object, the Sun, is a star and as such should also be emissive:
    solar_system.frames[0].model.meshes[0].material.emission_strength = 5

Our Solar System also has two asteroid belts:
    - The first is located between Mars and Jupyter, and is called `The Asteroid Belt<https://en.wikipedia.org/wiki/Asteroid_belt>`
    - The second is located beyond Neptune, and is called `The Kuiper Belt<https://en.wikipedia.org/wiki/Kuiper_belt>`

We will model the asteroids as a circular PointCloud:

.. jupyter-execute::

    asteroid_material = vivid3d.Material(1, 1, 0.7) # Asteroids are primarily heavy metallic elements, with a rough semi-reflective texture

    def create_asteroid_field(distance, scale, count):
        points = []
        for i in range(count):
            r = np.random.normal(distance, scale) # Modelling the asteroid belts as a normal distribution around an orbital distance
            a = 2 * math.pi * np.random.normal();
            z = np.random.normal(0, 0.15)
            points.append([r * math.cos(a), z, r * math.sin(a)]) # Placing randomly along circle of radius r
        asteroids = vivid3d.PointCloud(points, 'black')
        asteroids.material = asteroid_material
        return vivid3d.Model(asteroids)

    # Asteroid Belt, Kuiper Belt
    solar_system.add_model(create_asteroid_field(5, 0.6, 1_000)) # Populate the inner Asteroid Belt with 1,000 points between the orbits of Mars and Jupyter
    solar_system.add_model(create_asteroid_field(55, 5, 5_000))  # Populate the outer Kuiper Belt with 5,000 points past the orbit of Neptune

Finally, we will animate the solar system by setting the `rotate_animation` property of each frame of our `solar_system` to the appropriate orbital speed:

.. jupyter-execute::

    rotation_speeds = [0, 10, 35, 30, 24, 13, 9.7, 7, 5.4, 2, 1]

    for frame, speed in zip(solar_system.frames, rotation_speeds):
        frame.rotate_animation = [0, speed, 0] # rotating around the y axis

    # Slowing down the framerate
    solar_system.ticks_per_second = 5
    solar_system.show()


