# import sys

# sys.path.insert(0, "/home/zohar/Documents/Vivid/cmake-build-debug")

import vivid_py as vivid
import numpy as np

mesh_par = []
mask = []
quen = []

box_R = 24
box_size = 20

hight = 10
for i in range(-box_size, box_size, 2):
    for j in range(-box_size, box_size, 2):
        for k in range(-box_size, box_size, 2):
            mesh_par.append([i, j, k])
            if (0 <= k and k <= hight):
                a = hight - k
            else:
                a = 0

            if ((0 <= k and k <= hight) and (-a <= i and i <= a) and (-a <= j and j <= a)):
                mask += [True]
                quen += [10 ** a + 0.0001]
            else:
                mask += [False]
                quen += [0.00001]

mask = np.array(mask)
quen = np.array(quen)  # * (1.0/max(quen))

x = vivid.CSurf(mesh_par, mask, quen)
x.SmoothSurf()
y = vivid.CSurf(x)

test_path = r"D:\Documents\Alpha\Vivid3\test_models"


x = x.ToMesh("hallelujah", 0.8)
x.Decimation(0.6, 0.2)
x.ExportToObjTexture(test_path + r"\test_wrapper_win_texture")
y = y.ToMesh()
y.ExportToObj(test_path + r"\test_wrapper_win")

arr = [x, y]
arr = np.array(arr)
z = vivid.CModel(arr)


print "start arrow"
arrow = vivid.CreateArrowMesh(10, 1, 0.2, [0, 0, 0], [1, 1, 0], 0.6, 0.9, "arrow")
arrow.ExportToObj(test_path + r"\arrow_test_py")
print "arrow done"

print "start ellips"
ellips = vivid.CreateEllipsoidMesh(20, 20, [10, 5, 5], [0, 0, 10], [-1, 2, 0], [2, 1, 0], [0, 0, 1], 0.6, 0.8, "Ellips")
ellips.ExportToObj(test_path + r"\ellips_test_py")
print "ellips done"

print "start sphere"
sphere = vivid.CreateSphereMesh(20, 20, 10, [0, 0, 0], 0.6, 0.8, "sphere")
sphere.ExportToObj(test_path + r"\sphere_test_py")
print "sphere done"

print "start animation no textures"
vivid.RotateAnim(z, 100, 100, 1, test_path + r"\test_wrapper_animation_win")
print "animation with no textures done"


print "start animation textures"
vivid.RotateAnimTextures(z, 1000, 100, 1, test_path + r"\test_wrapper_animation_texture_win")
print "animation with textures done"



