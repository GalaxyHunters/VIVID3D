import ctypes

lib = ctypes.cdll.LoadLibrary("C:\\Users\\User\\source\\repos\\Project1\\Project1\\Mesh.so")


class Mesh(object):
    def __init__(self):
        lib.Mesh_new.argtypes = [ctypes.c_void_p]
        lib.Mesh_new.restype = ctypes.c_void_p

        lib.Mesh_simplify.argtypes = [ctypes.c_void_p, ctypes.c_float, ctypes.c_float]
        lib.Mesh_simplify.restype = ctypes.c_void_p

        lib.Mesh_export.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        lib.Mesh_export.restype = ctypes.c_void_p

        lib.Mesh_getLabel.argtypes = [ctypes.c_void_p]
        lib.Mesh_getLabel.restype = ctypes.c_char_p

        lib.Mesh_getAlpha.argtypes = [ctypes.c_void_p]
        lib.Mesh_getAlpha.restype = ctypes.c_float

        lib.Mesh_setLabel.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        lib.Mesh_setLabel.restype = ctypes.c_void_p

        lib.Mesh_setAlpha.argtypes = [ctypes.c_void_p, ctypes.c_float]
        lib.Mesh_setAlpha.restype = ctypes.c_void_p

        self.obj = lib.Mesh_new()

    def simplify(self, verticle_percent, error):
        lib.Mesh_simplify(self.obj, verticle_percent, error)

    def print_mesh(self, output):
        lib.Mesh_export(self.obj, output)

    def get_label(self):
        return lib.Mesh_getLabel(self.obj)

    def get_alpha(self):
        return lib.Mesh_getAlpha(self.obj)

    def set_label(self, label):
        lib.Mesh_setLabel(self.obj, label)

    def set_alpha(self, alpha):
        lib.Mesh_setAlpha(self.obj, alpha)



