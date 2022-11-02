Animations
==========

VIVID3D supports two types of animations,

.. list-table::

    * - :doc:`../generated/vivid3d.Animation`
      - list of models each with animation fields of Scale, Rotation, and Movement, all of which are happening simultaneously.
    * - :doc:`../generated/vivid3d.StopMotionAnimation`
      - same as above but each time only one model appears and the change rate is determined by seconds_per_frame

First, we make a few cubes to animate:

.. jupyter-execute::

    import vivid3d
    import numpy as np

    models = []
    colors = ["Red", "Blue", "Green", "Purple", "Yellow", "Cyan", "White", "Black"]
    loc = [[0,0,0], [1,0,0], [2,0,0],[0,0,1],[1,0,1],[2,0,1],[0,0,2],[1,0,2]]
    for i in range(8):
        box = vivid3d.create_cube(position=loc[i], size=0.5, color=colors[i], label=colors[i])
        models.append(vivid3d.Model([box]))

Animated Models
---------------

Now lets add some animation to the cubes:

.. code-block:: python
   :caption: Regular animation

   animation = vivid3d.Animation(models)
   animation.duration = 150 #duration in ticks per second
   animation.ticks_per_second = 30
   
   #set animation to each model by index and animation type
   animation.set_move_animation(0, [3,3,3]) #movement vector, 0 is stay still
   animation.set_move_animation(4, [0,10,3])
   animation.set_move_animation(7, [0,0,3])
   animation.set_rotate_animation(1, [2.3,2.3,2.3]) # in anguler angles
   animation.set_rotate_animation(6, [0, 2.3, 1])
   animation.set_scale_animation(3, [0.5,0.5,0.5]) #1 is basic size, 0 is non-existence
   animation.set_scale_animation(5, [3,3,3])

   #export
   animation.export(path+"/CubeAnimation", "gltf2")

Stop Motion Animation
---------------------

And if we want stop-motion animation we just add this:

.. code-block:: python
   :caption: Stop motion animation

   SMA = vivid3d.StopMotionAnimation(animation, seconds_per_frame= 2)
   SMA.export(path+"/CubeSMAnimation", "gltf2")
