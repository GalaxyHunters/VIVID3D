![image](https://github.com/GalaxyHunters/Vivid/blob/01788a5e3656dbaa048a48215a290dfb7f3dc831/vivid-cover.png?raw=true)

## Vivid
We present VIVID, an innovative method for research using 3D technology.

## Explanation
Vivid uses an implementation of Voronoi algorithm for a 3d space made by Dr. Elad Stiennberg. The package takes simulated data as input and produces an interactive 3D model that is compatible with the 3D industry.  

VIVID was tested in our cosmology research group by producing models from simulations for researchers, supporting new scientific insights about the evolution of galaxies and structures of the Universe.

The code is written in C++ and wrapped in python.

Vivid is currently in the last stages of development, including an installer and the finished wrapper. Soon you will be able to simply install Vivid using anaconda and produce a model using a single function or a few lines of code for more control.

also see https://naftalide.wixsite.com/vivid to view examples of output models.

## Getting started

Downloed the prerequirments
```
pip install --upgrade pip
pip install --upgrade setuptools
```

install the package
```
pip install vivid3d
```


## Creating your first model

Code Example

```Python

import vivid_py as vivid
import numpy as np

surf = vivid.Surface(points, mask, quan)
surf.create_surface()
mesh = surf.to_mesh("This is mt first model", .9)
mesh.laplacian_smooth(10, 0.7, 0)
mesh.export_to_obj(path + "/FirstModel");

```
The mandatory inputs to produce a model are as follows:

- ndArray 3*n float : ModelPoints - holds X Y Z values for data.

- ndArray n boolean : Mask - mask values for ModelPoints. Output model surface is defined as the middle between True and False points.

- string : OutputFile - name and directory for the output file.

optinal arguments:

- Label - float, label for the created surface. defult argument = "Vivid_3D_model"

- Quan - n float ndArray, Defines color per point. default argument = ndArray.ones().

- Vmin - float, minimum value for color map. default argument Quan.min_argument().

- Vmax - float, maximum value for color map. default argument Quan.max_argument().
