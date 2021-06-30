#include "ModelBuilder/Surface.h"
#include "ModelBuilder/Mesh.h"
#include "Utils/Shapes.h"
#include "ModelBuilder/Animation.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace vivid;

namespace py = pybind11;

PYBIND11_MODULE(vivid_py, m) {
    py::class_<CSurface>(m, "CSurface")
            .def(py::init<vector<vector<double >>, vector<bool>, vector<coord_t>, coord_t, coord_t>(), "constructor function for surf",
                 py::arg("aInputPoints"), py::arg("aMask"), py::arg("aQuan") = vector<double>(0), py::arg("aVMin") = 0, py::arg("aVMax") = 0) // = vector<double>(0)
            .def(py::init<const CSurface &> (), "copy constructor for CSurface", py::arg("surf"))
            .def("SmoothSurf", &CSurface::SmoothSurf, "A smoothing algorithm for the surface, improves visibility and helps the decimation algorithm in the next stage")
            .def("ToMesh", &CSurface::ToMesh, "returns a mesh obj, a mesh obj can use decimation but will not be able to run smooth",
                 py::arg("aLabel") = "VIVID_3D_MODEL", py::arg("aAlpha") = 1)
            .def("ExportToObj", &CSurface::ExportToObj, "writes the surface to an OBJ file",
                 py::arg("aOutputFile"), py::arg("aLabel") = "VIVID_3D_MODEL", py::arg("aAlpha") = 1);

    py::class_<CMesh>(m, "CMesh")
            .def(py::init<const CMesh &> (), "copy constructor for CMesh", py::arg("Mesh"))
            .def("Decimation", &CMesh::Decimation,
                 "input values should be between 0 and 1. A Decimation algorithm for the surface, reduces file size while trying to maintain the the shape as much as possible. it's recommended to not over do it.",
                 py::arg("aDecimationPercent") = 0.5, py::arg("aError") = 0.1)
            .def("ExportToObj", &CMesh::ExportToObj,  "writes the surface to an OBJ file, by materials or textures", py::arg("aOutputFile"), py::arg("WithTexture") = 1); //TODO make sure it sent as True to the bool param
    py::class_<CModel>(m, "CModel")
            .def(py::init<> (), "default constructor for CModel")
            .def(py::init<vector<CMesh> > (), "constructor for CModel, from meshes", py::arg("Meshes"))
            .def(py::init<vector<CSurface>, string, coord_t> (), "constructor for CModel, from surfs", py::arg("Surfs"), py::arg("label") = "VIVID_3D_MODEL", py::arg("alpha") = 1)
            .def("AddMesh", &CModel::AddMesh, "add another mesh to Model", py::arg("Mesh"))
            .def("AddSurf", &CModel::AddSurf, "add another mesh to model, using surf", py::arg("surf"), py::arg("label") = "VIVID_3D_MODEL", py::arg("alpha") = 1)
            .def("ExportToObj", &CModel::ExportToObj,  "writes the surface to an OBJ file, by materials or textures", py::arg("aOutputFile"), py::arg("WithTexture") = 1); //TODO make sure it sent as True to the bool param
    m.def("Animate", &Animate, "Takes a numpy array of CModels, an output location and an interval and creates a FBX animation containing a model in each frame",
          py::arg("Models"),py::arg("Interval"), py::arg("OutputFile"));
    m.def("RotateAnim", &RotateAnim, "takes a model and creates an animation of it rotating", py::arg("Model"), py::arg("Length"), py::arg("Duration"), py::arg("RotationAxis"), py::arg("OutputFile"));
    m.def("AnimateTextures", &AnimateTextures, "Takes a numpy array of CModels, an output location and an interval and creates a FBX animation containing a model in each frame",py::arg("Models"), py::arg("Interval"), py::arg("OutputFile"));
    m.def("RotateAnimTextures", &RotateAnimTextures, "takes a model and creates an animation of it rotating", py::arg("Model"), py::arg("Length"), py::arg("Duration"), py::arg("RotationAxis"), py::arg("OutputFile"));

    m.def("CreateBoxMesh", &CreateBoxMesh, "Creates a box mesh", py::arg("sizeX"),  py::arg("sizeY"),  py::arg("sizeZ"),  py::arg("color") = 0.5, py::arg("alpha") = 0.9, py::arg("position") = vector<double>{0, 0, 0});
    m.def("CreateSphereMesh", &CreateSphereMesh,"Creates a sphere", py::arg("NumOfMeridians"), py::arg("NumOfParallels"), py::arg("Radius"), py::arg("CenterPoint"),py::arg("Color"), py::arg("Alpha"), py::arg("Label"));
    m.def("CreateEllipsoidMesh", &CreateEllipsoidMesh, "Creates ellipsoid mesh", py::arg("NumOfMeridians"), py::arg("NumOfParallels"), py::arg("Radii"), py::arg("CenterPoint"), py::arg("MajorAxis"), py::arg("MiddleAxis"), py::arg("MinorAxis"), py::arg("Color"), py::arg("Alpha"), py::arg("Label"));
    m.def("CreateArrowMesh", &CreateArrowMesh, "Creates an arrow mesh",  py::arg("Width"), py::arg("PointerToChestRatio"), py::arg("BasePoint"), py::arg("DirectionVector"), py::arg("Color"), py::arg("Alpha") = 1, py::arg("Label")="");
}
