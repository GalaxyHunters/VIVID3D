import viewer


def show(a):
    # print(self.ExportToGlb())
    #if a.ExportToGlb():
    embedded = viewer.show('model/CubeAnimationTest.gltf')
    # viewer.show(self.ExportToGlb())
    #stub = a.ExportToGlb()
    #return stub
    return embedded


