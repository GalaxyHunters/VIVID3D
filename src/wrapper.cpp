#include "ModelBuilder/Surf.h"
#include "ModelBuilder/Mesh.h"
#include "ModelBuilder/Model.h"
#include "Utils/Shapes.h"
#include "AnimationBuilder/Animation.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(vivid_py, m) {
    py::class_<CSurf>(m, "CSurf")
            .def(py::init<vector<vector<double >>, vector<bool>, vector<cord_t>, cord_t, cord_t>(), "constructor function for surf",
                 py::arg("aInputPoints"), py::arg("aMask"), py::arg("aQuan") = vector<double>(0), py::arg("aVMin") = 0, py::arg("aVMax") = 0) // = vector<double>(0)
            .def(py::init<const CSurf &> (), "copy constructor for CSurf", py::arg("surf"))
            .def("SmoothSurf", &CSurf::SmoothSurf, "A smoothing algorithm for the surface, improves visibility and helps the decimation algorithm in the next stage")
            .def("ToMesh", &CSurf::ToMesh, "returns a mesh obj, a mesh obj can use decimation but will not be able to run smooth",
                 py::arg("aLabel") = "VIVID_3D_MODEL", py::arg("aAlpha") = 1)
            .def("ExportToObj", &CSurf::ExportToObj, "writes the surface to an OBJ file",
                 py::arg("aOutputFile"),  py::arg("aLabel") = "VIVID_3D_MODEL", py::arg("aAlpha") = 1);

    py::class_<CMesh>(m, "CMesh")
            .def(py::init<const CMesh &> (), "copy constructor for CMesh", py::arg("Mesh"))
            .def("Decimation", &CMesh::Decimation,
                 "input values should be between 0 and 1. A Decimation algorithm for the surface, reduces file size while trying to maintain the the shape as much as possible. it's recommended to not over do it.",
                 py::arg("aDecimationPercent") = 0.5, py::arg("aError") = 0.1)
            .def("ExportToObj", &CMesh::ExportToObj, "writes the surface to an OBJ file, color is by metirals",
                 py::arg("aOutputFile"))
            .def("ExportToObjTexture", &CMesh::ExportToObjTexture, "writes the surface to an OBJ file, color is by texture", py::arg("aOutputFile"));

    py::class_<CModel>(m, "CModel")
            .def(py::init<> (), "default constructor for CModel")
            .def(py::init<vector<CMesh> > (), "constructor for CModel, from meshes", py::arg("Meshes"))
            .def(py::init<vector<CSurf>, string, cord_t> (), "constructor for CModel, from surfs", py::arg("Surfs"), py::arg("label") = "VIVID_3D_MODEL", py::arg("alpha") = 1)
            .def("AddMesh", &CModel::AddMesh, "add another mesh to Model", py::arg("Mesh"))
            .def("AddSurf", &CModel::AddSurf, "add another mesh to model, using surf", py::arg("surf"), py::arg("label") = "VIVID_3D_MODEL", py::arg("alpha") = 1)
            .def("ExportToObj", &CModel::ExportToObj, "writes the surfaces to an OBJ file", py::arg("aOutputFile"));

    m.def("Animate", &Animate, "Takes a numpy array of CModels, an output location and an interval and creates a FBX animation containing a model in each frame",
          py::arg("models"),py::arg("interval"), py::arg("outputfile"));
    m.def("RotateAnim", &RotateAnim, "takes a model and creates an animation of it rotating", py::arg("model"), py::arg("length"), py::arg("duration"), py::arg("rotation_axis"), py::arg("outputfile"));


    py::class_<Shapes>(m, "Shapes")
            .def_static("CreateCubeMesh", &Shapes::CreateCubeMesh, "creates a cube mesh", py::arg("sizeX"),  py::arg("sizeY"),  py::arg("sizeZ"),  py::arg("color") = 0.5, py::arg("alpha") = 0.9, py::arg("position") = vector<double>{0, 0, 0});



}
