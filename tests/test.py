import vivid3d
import numpy as np

#make the data
BOX_SIZE = 30

#make 3d grid
arr = np.arange(-BOX_SIZE, BOX_SIZE, 2)
ones = np.ones((len(arr), len(arr), len(arr)))
arrX = arr.reshape(-1, 1, 1) * ones
arrY = arr.reshape(1, -1, 1) * ones
arrZ = arr.reshape(1, 1, -1) * ones
# set points
points = np.array(list(zip(arrX.reshape(-1), arrY.reshape(-1), arrZ.reshape(-1))))
# set color field by Z value
color = arrZ.reshape(-1)
# Get distance from zero to each point (We will make our surface with this)
distance = np.linalg.norm(points - np.zeros((1, 3)), axis=1).reshape(-1)


#we will make two masks for two meshes
mask1 = np.array(distance) > 25
mask2 = np.array(distance) > 15

#create the faces by running Voronoi, this might be a heavy function
voronoi = vivid3d.VoronoiVolume(points, color) #make VoronoiVolume obj

#make a mesh and take a surface by mask
mesh1 = voronoi.iso_surface(mask1, "Mesh", 0.5)
mesh2 = voronoi.iso_surface(mask2, "Mesh", 1)
mesh1.smooth()
mesh1.export("test_models/testexport", "obj")
