import sys

import vivid_py as vivid
import numpy as np
import pickle
import yt

def OpenPickleData(path):
    with open(path, 'rb') as handle:
        data = pickle.load(handle)
        return data

def CheckPickleData(path, mask):
    data = OpenPickleData(path)
    if any(data['mask'][mask]) == True:
        print '--------------------there are points in the mask---------------------'
        return True
    else:
        print '--------------------there are no points in the mask ' + mask + ' of path ' + path + '------------------'
        return False

def RunPickle(path, masks, colors):

    surfs = []
    meshes = []

    for i in range(len(masks)):
        data = OpenPickleData(path)
        mask_status = CheckPickleData(path, masks[i])

        if mask_status == True:
            try:
                mask = data['mask'][masks[i]]
                quan = data['quan']
                x = vivid.CSurf(data['mesh_par'], mask, quan)
                x.SmoothSurf()
                y = vivid.CSurf(x)
                surfs.append(y)
                #surfs[i].SmoothSurf()
                print '--------------------successfully created ' + masks[i] + ' for file ' + path + '----------------------'
            except:
                print '--------------------failed to create ' + masks[i] + ' for file ' + path + '------------------------'
        else:
            print 'unable to create surf due to lack of points in the mask'

        meshes.append(surfs[i].ToMesh("WORK", 1))
        i = i+1
        # faces = meshes[i].GetFaces();
        # for j in range(len(faces)):
        #     faces[j].SetColor(colors[i])

        # meshes[i].Decimation()

    model = vivid.CModel(meshes)
    return model



def CreateAnimFromPkls(paths, masks, colors, interval, outputfile):

    for i in range(len(paths)):
        for j in range(len(masks)):
            mask_status = CheckPickleData(paths[i], masks[j])
            if mask_status == False:
                print 'cant run, some of the surfs cannot be created, look at error log to see which files and masks are uncreatable'
                exit()
    models = []
    for i in range(len(paths)):
        models.append(RunPickle(paths[i], masks, colors))
    print 'starting animation'
    vivid.Animate(models, outputfile, interval)
    print 'animation done'


def ExportPklToObj(inputpath, outputpath, masks, colors):
    model = RunPickle(inputpath, masks, colors)
    print 'starting model export'
    model.ExportToObj(outputpath)
    print 'model exported, find the file in' + outputpath


with open(r'D:\Documents\Alpha\Models\Pickles\special_boys\gal_07_1e-24.pickle', 'rb') as handle:  #gal_07_1e-24_w_color.pickle
    data = pickle.load(handle)

points = data['mesh_par']
mask = data['mask']
quan = data['quan']

print len(mask)
surf = vivid.CSurf(points, mask, quan)
surf.SmoothSurf()
mesh = surf.ToMesh(aLabel = "gal_07", aAlpha= 0.9)
mesh.Decimation()
mesh.ExportToObjTexture(r'D:\Documents\Alpha\Models\Pickles\special_boys\gal_07_1e-24')

# Pickles = ["/home/zohar/Documents/pickles/VELA_v2.Thick.22.a0.230130642653_00001_1e-24_TO_1e-26_GAS.pickle",
#            "/home/zohar/Documents/pickles/VELA_v2.Thick.22.a0.21007296443_00006_1e-24_TO_1e-26_GAS.pickle",
#            "/home/zohar/Documents/pickles/VELA_v2.Thick.07.a0.370373249054_00031_1e-24_TO_1e-26_GAS.pickle",
#            "/home/zohar/Documents/pickles/VELA_v2.Thick.07.a0.34049654007_00013_1e-24_TO_1e-26_GAS.pickle"]
#
#
#
# names = ['VELA_v2.Thick.22.a0.230130642653_00001_1e-24_TO_1e-26_GAS',
#          'VELA_v2.Thick.22.a0.21007296443_00006_1e-24_TO_1e-26_GAS',
#          'VELA_v2.Thick.07.a0.370373249054_00031_1e-24_TO_1e-26_GAS',
#          'VELA_v2.Thick.07.a0.34049654007_00013_1e-24_TO_1e-26_GAS']
#
# data = OpenPickleData(Pickles[0])
# #print data
# #print data['mask']['1e-24']
# print np.array(data['quan'])
#
# vivid.CSurf(data['mesh_par'], np.array(data['mask']['1e-24']), np.array(data['quan']))

#ExportPklToObj(Pickles[0], "/home/zohar/Documents/models/" + names[0], ['1e-24', '1e-25', '1e-26'], 1)

# for f in range(4):
#     pickle = Pickles[f]
#     print pickle
#     i = 0
#     ExportPklToObj(pickle, "/home/zohar/Documents/models/" + names[i], ['1e-24', '1e-25', '1e-26'], 1)
#     i = i + 1



# def RotateAnim(inputpath, outputpath, masks, colors, time, distance, rotation_axis):
#     model = RunPickle(inputpath, masks, colors)
#     print 'starting animation'
#     vivid.RotateAnim(model, distance, time, rotation_axis, outputpath)
#     print 'animation done, the file is in ' + outputpath


# models = []
# for i in range(100):
#     models.append(vivid.CModel())
#     models[i].AddMesh(vivid.Shapes.CreateCubeMesh(i, i, i))
#
# vivid.Animate(models, 0.05, "D:\\Documents\\Alpha\\Models\\Random\\NewAnimationPY2.fbx")

# inputpaths = [
#               # "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.13013972342_00008_1e-23_TO_25_FIXED.pickle",
#               # "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.18022352457_00007_1e-23_TO_25_FIXED.pickle",
#               "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.190173849463_00003_1e-23_TO_25_FIXED.pickle",
#               "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.21007296443_00006_1e-23_TO_25_FIXED.pickle",
#               "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.140090182424_00005_1e-23_TO_25_FIXED.pickle",
#               "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.150043055415_00007_1e-23_TO_25_FIXED.pickle",
#               "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.160120561719_00008_1e-23_TO_25_FIXED.pickle",
#               "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.170153677464_00004_1e-23_TO_25_FIXED.pickle",
#               "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.200105398893_00005_1e-23_TO_25_FIXED.pickle",
#               "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.220141962171_00007_1e-23_TO_25_FIXED.pickle",
#               "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.230130642653_00001_1e-23_TO_25_FIXED.pickle",
#               "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.240224197507_00004_1e-23_TO_25_FIXED.pickle",
#               # "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.250203311443_00001_1e-23_TO_25_FIXED.pickle",
#               # "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.260174006224_00005_1e-23_TO_25_FIXED.pickle",
#               # "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.270193129778_00007_1e-23_TO_25_FIXED.pickle",
#               #"C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.280246883631_00007_1e-23_TO_25_FIXED.pickle",
#               # "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.290110230446_00005_1e-23_TO_25_FIXED.pickle",
#               # "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.300112098455_00006_1e-23_TO_25_FIXED.pickle",
#               # "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.310047000647_00006_1e-23_TO_25_FIXED.pickle",
#               # "C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Fixed_pickles\VELA_v2.Thick.22.a0.320370465517_00007_1e-23_TO_25_FIXED.pickle",
#     ]




#RunPickle("C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\VELA_v2.Thick.22.a0.190173849463_00003_1e-24_old.pickle", ['1e-24'])
#RotateAnim("C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Gas_pickles_2\VELA_v2.Thick.22.a0.13013972342_00008_1e-24_TO_1e-26_GAS_FIXED.pickle", r'C:\Users\zorik\OneDrive\Documents\Alpha\Models\19_rotate_1e-26.fbx', ['1e-26'], [0.3], 2, 1000, 1)
#CreateAnimFromPkls(inputpaths, ['1e-24'], [0.4], 0.2, r"C:\Users\zorik\OneDrive\Documents\Alpha\Models\gal_22_animation.fbx")
# ExportPklToObj("C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Gas_pickles_2\VELA_v2.Thick.22.a0.190173849463_00003_1e-24_TO_1e-26_GAS_FIXED.pickle", r'C:\Users\zorik\OneDrive\Documents\Alpha\Models\gal_22_19.obj', ['1e-26'], [0.2])
# RotateAnim("C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Gas_pickles_2\VELA_v2.Thick.22.a0.190173849463_00003_1e-24_TO_1e-26_GAS_FIXED.pickle", r'C:\Users\zorik\OneDrive\Documents\Alpha\Models\19_rotate_1e-24.fbx', ['1e-24'], [0.3], 2, 100, 1)



#data = OpenPickleData("C:\Users\zorik\OneDrive\Documents\Alpha\Models\Pickles\Gas_pickles_2\VELA_v2.Thick.22.a0.13013972342_00008_1e-24_TO_1e-26_GAS_FIXED.pickle")
#print data['quan'][0]

# cubes = []
# models = []
# for i in range(100):
#     cubes.append(vivid.Shapes.CreateCubeMesh(i + 1, i + 1, i + 1))
#     models.append(vivid.CModel([cubes[i]]))
# vivid.Animate(models, r'C:\Users\zorik\OneDrive\Documents\Alpha\Models\cube_grow.fbx', 0.05)
# ExportPklsToObj(inputpaths, "C:\\Users\\zorik\\OneDrive\\Documents\\Alpha\\Models\\gal_22\\")