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
    loc = [[0,0,0], [2,0,0], [4,0,0],[0,0,2],[2,0,2],[4,0,2],[0,0,4],[2,0,4]]
    for i in range(8):
        box = vivid3d.create_cube(position=loc[i], size=0.5, color=colors[i], label=colors[i])
        models.append(vivid3d.Model([box]))

Animated Models
---------------

Now lets add some animation to the cubes:

.. jupyter-execute::
    :raises:

    animation = vivid3d.Animation(models)
    animation.duration = 150 #duration in ticks per second
    animation.ticks_per_second = 30
   
    #set animation to each model by index and animation type
    animation.frames[0].move_animation = [3,3,3] #movement vector, 0 is no movement.
    animation.frames[4].move_animation = [0,10,3]
    animation.frames[7].move_animation = [0,0,3]
    animation.frames[1].rotate_animation = [2.3,2.3,2.3] # in anguler angles
    animation.frames[6].rotate_animation = [0, 2.3, 1]
    animation.frames[3].scale_animation = [0.5,0.5,0.5] #1 is basic size, 0 is non-existence
    animation.frames[5].scale_animation = [3,3,3]

    #export
    animation.show()

Stop Motion Animation
---------------------

And if we want stop-motion animation we just add this:

.. jupyter-execute::
    :raises:

    SMA = vivid3d.StopMotionAnimation(animation, seconds_per_frame= 1.5)
    SMA.show()
