import ctypes

lib = ctypes.cdll.LoadLibrary("C:\\Users\\User\\source\\repos\\Project1\\Project1\\Surf.so")


class Surf (object):
    def __init__(self, points, mask, quan):
        lib.Surf_new.argtypes = [ctypes.c_int * 3 * 1000000000, ctypes.c_bool * 1000000000, ctypes.c_float * 10]
        lib.Surf_new.restype = ctypes.c_void_p

        lib.Surf_smooth.argtypes = [ctypes.c_void_p]
        lib.Surf_smooth.restype = ctypes.c_void_p

        lib.Surf_to_mesh.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_float]
        lib.Surf_to_mesh.restype = ctypes.c_void_p

        lib.Surf_exportToObj.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_float]
        lib.Surf_exportToObj.restype = ctypes.c_void_p

        self.obj = lib.Surf_new(points, mask, quan)

    def smooth(self):
        lib.Surf_smooth(self.obj)

    def to_mesh(self, label, alpha):
        return lib.Surf_to_mesh(self.obj, label, alpha)

    def export_to_obj(self, output, label, alpha):
        lib.Surf_exportToObj(self.obj, output, label, alpha)
