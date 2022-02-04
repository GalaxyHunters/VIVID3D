![image](https://github.com/GalaxyHunters/Vivid/blob/01788a5e3656dbaa048a48215a290dfb7f3dc831/vivid-cover.png?raw=true)

## Vivid
We present VIVID, an innovative method for research using 3D technology.

## Explanation
Vivid uses an implementation of Voronoi algorithm for a 3d space made by Dr. Elad Stiennberg. The package takes simulated data as input and produces an interactive 3D model that is compatible with the 3D industry.  

VIVID was tested in our cosmology research group by producing models from simulations for researchers, supporting new scientific insights about the evolution of galaxies and structures of the Universe.

The code is written in C++ and wrapped in python code for comptorbality.

Vivid is currently in the last stages of development, including an installer and the finished wrapper. Soon you will be able to simply install Vivid using anaconda and produce a model using a single function or a few lines of code for more control.

## How To Use
In order to produce a model the following inputs are reqired:

Points - 3*n float ndArray, X Y Z values accurding to which, the model will be created.

Mask - n boolean ndArray, mask values for the inputed points that represent which points shuld be within the surface and which ones shukld be outside(the surface will be created betwwen the two).

OutputFile - string, name and diroctory for the output file.

optinal arguments:

Label - float, label for the created surface. defult argument = "Vivid_3D_model"

Quan - n float ndArray, corresponding color values for the input points. default argument = ndArray.ones().

Vmin - float, minimum value for color map. default argument Quan.min_argument().

Vmax - float, maximum value for color map. default argument Quan.max_argument().

decimation values #TODO

also see https://naftalide.wixsite.com/vivid to view examples of output models.
