Lines
=====

In this example, we generate a dataset of random walks starting from (0,0,0), and display them using :doc:`../generated/vivid3d.Lines`.

.. jupyter-execute::

    import vivid3d
    import numpy as np

    # generating data
    def random_walk():
        line = [0,0,0]
        for i in range(10): # random walk in 3D over 10 iterations
            walk = np.random.uniform(-1, 1, 3) # walk by random amount from [-1, 1] along each axis
            line.append(line[-1] + walk)
        return line

    lines = [random_walk() for i in range(10)] # create an array of 10 random walks

    #lines = vivid3d.Lines(lines) # Create a Lines object from the data
    #lines.show()
