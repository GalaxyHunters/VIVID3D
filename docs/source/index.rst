=============================================
vivid3d - 3D Visualization and animation tool
=============================================

We present VIVID, an innovative method for research using 3D technology.

Alongside theoretical and experimental studies, one of the useful tools for research in the field of physics is the numerical simulation. Powerful as it is, the understanding of simulation may be difficult. The typical way to visually inspect the simulations, 2D projections, allows the researcher to develop intuition and understanding of the simulation. However, not all the details of a given simulation are visible in such a plot. We find that involving 3D tools in scientific research can be profitable, and given the interactive nature of 3D, it can fill the gaps left by the 2D projections. The following developed tool enables new points of view for researchers and facilitates discoveries.

 

VIVID uses a Voronoi code package to represent the simulated data as polygons. The tool proceeds to clear the data resulting in a surface. Later using algorithms of Smooth, decimation and Double Points handling it sharpens the model features, resulting in a clear, easy to study model. By presenting the simulation as fully interactive 3D models, we can make the process of analyzing the simulation faster and efficient, enabling visualizing hidden by 2D physical dynamics — the creation of such models is done in VIVID by just one simple line of code.

 

VIVID was tested in our cosmology research group by producing models from simulations for researchers, supporting new scientific insights about the evolution of galaxies and structures of the Universe.

----

Install
-------


Installing the package is as simple as pip install:

..  code-block:: shell
    
    pip install vivid3d

This might take a few minutes, and is only tested on linux as of now.

----

Content
-------
.. toctree::
   :maxdepth: 3

   examples
   vivid_types 

