import Vivid_py as vivid
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
            if (0<=k and k<=hight):
                a = hight-k
            else:
                a=0

            if ((0<=k and k<=hight) and (-a<=i and i <=a) and (-a<=j and j <=a)):
                mask += [True]
                quen += [10**a+0.0001]
            else:
                mask +=[False]
                quen += [0.00001]

mask = np.array(mask)
quen = np.array(quen) #* (1.0/max(quen))

x = vivid.CSurf(mesh_par, mask, quen)
x.SmoothSurf()
y = vivid.CSurf(x)

x = x.ToMesh("hallelujah", 0.8)
x.Decimation(0.6, 0.2)
x.ExportToObj(r"..\test_models\test_wrapper_win")
y = y.ToMesh()
y.ExportToObj(r"..\test_models\test_wrapper_win")

arr = [x, y]
arr = np.array(arr)
z = vivid.CModel(arr)
