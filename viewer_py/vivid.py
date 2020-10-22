from vivid_py import *
import viewer


def show(a):
    # print(self.ExportToGlb())
    #if a.ExportToGlb():
    embedded = viewer.show('model.glb')
    # viewer.show(self.ExportToGlb())
    #stub = a.ExportToGlb()
    #return stub
    return embedded
setattr(CModel,'show',show)