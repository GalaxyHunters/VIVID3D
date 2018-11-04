import ctypes

lib = ctypes.cdll.LoadLibrary("C:\\Users\\User\\source\\repos\\Project1\\Project1\\Model.so")


class Model(object):

    def __init__(self, meshes):
        lib.Model_new.argtypes = [ctypes.c_void_p * 50]
        lib.Model_new.restype = ctypes.c_void_p

        lib.Model_export.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        lib.Model_export.restype = ctypes.c_void_p

        lib.Model_save.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        lib.Model_save.restype = ctypes.c_void_p

        lib.Model_load.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        lib.Model_load.restype = ctypes.c_void_p

        lib.Model_addMesh.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
        lib.Model_addMesh.restype = ctypes.c_void_p

        self.obj = lib.Model_new(meshes)

    def print_model(self, output):
        lib.Model_export(self.obj, output)

    def save(self, output_file):
        lib.Model_save(self.obj, output_file)

    def load(self, input_file):
        return lib.Model_load(self.obj, input_file)

    def add_mesh(self, mesh):
        lib.Model_addMesh(self.obj, mesh)
