#include "Typecasting.h"
#include "./ModelBuilder/Surface.h"
#include "./ModelBuilder/ModelComponent.h"
#include "./ModelBuilder/Mesh.h"
#include "./ModelBuilder/Line.h"
#include "./ModelBuilder/PointCloud.h"
#include "./ModelBuilder/Shapes.h"
#include "./ImportAndExport/FBXImportExport.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

using namespace vivid;
using namespace std;

namespace py = pybind11;

PYBIND11_MODULE(vivid_py, m) {
    m.doc() = "VIVID: Creating 3D animations in one line of code";
    // add model component base functions
//    py::class_<CModelComponent> (m, "ModelComponent")
//        .def(py::init<const CModelComponent&> (),
//                "Constructor for ModelComponent",
//                py::arg("model_component"))
//        //TODO: Add transform mesh functions
////        .doc("Parent class from which Mesh, Point Cloud, and Lines inherit.")
////        .def("transform", &CModelComponent::TransformMesh,
////             "Transform Model Component points by transformation function",
////             py::arg("function"))
////        .def("transform", &CModelComponent::TransformMesh,
////             "Transform Model Component points by transformation matrix",
////             py::arg("matrix"))
//        .def("RotateMesh", &CModelComponent::RotateMesh,
//             "Rotate the Model Component points around a normal vector by an angle, counterclockwise.",
//             py::arg("norm_vec"), py::arg("radians_angle"))
//        .def("MoveMesh", &CModelComponent::MoveMesh,
//             "Add direction_vec to Model Component points.",
//             py::arg("direction_vec"))
//        .def("ScaleMesh", &CModelComponent::ScaleMesh,
//             "Scale Model Component points by multiplying by scale_vec.",
//             py::arg("scale_vec"));
    py::class_<CModelComponent> model_component (m, "ModelComponent");
    py::class_<CPoint>(m, "Point")
//        .doc() = "VIVID Point Class"
//        .def(py::init<const CPoint &>(),
//                "copy constructor for Point",
//                py::arg("point"))
            .def(py::init<coord_t, coord_t, coord_t>(),
                 "Constructor for point",
                 py::arg("x"), py::arg("y"), py::arg("z"))
            .def(py::init<std::vector<double>>(),
                 "Constructor for point",
                 py::arg("3d_vector"));

    py::class_<CSurface>(m, "Surface")
            .def(py::init<const vector<vector<double>>&, const vector<bool>&, vector<coord_t>&, coord_t, coord_t>(),
                 "constructor function for surface",
                 py::arg("points"), py::arg("mask"), py::arg("quan") = vector<coord_t>(0), py::arg("quan_min") = 0, py::arg("quan_max") = 0)
            .def(py::init<const CSurface &> (),
                 "copy constructor for Surface",
                 py::arg("surf"))
            .def("create_surface", &CSurface::CreateSurface,
                 R"-(
             .. py:function:: create_surface()
             :return: Calculate the surface from input data)-")
            .def("smooth", &CSurface::Smooth,
                 "A smoothing algorithm -for the surface, improves visibility and helps the decimation algorithm in the next stage", py::arg("super_smooth"), py::arg("smooth_factor"))
            .def("to_mesh", &CSurface::ToMesh,
                 "returns a mesh obj, a mesh obj can use decimation but will not be able to run smooth",
                 py::arg("label") = "VIVID_3D_MODEL", py::arg("alpha") = 1);

    py::class_<CLines>(m, "Lines", model_component)
            .def(py::init<const vector<CPoint>&, const coord_t, const string& >(),
                 "Constructor for Lines",
                 py::arg("points"), py::arg("alpha")=1., py::arg("label")="")
            .def(py::init<const vector<vector<CPoint>>&, const coord_t, const string&>(),
                 "Constructor for Lines",
                 py::arg("points_matrix"), py::arg("alpha")=1., py::arg("label")="")
            .def(py::init<const CLines&>(),
                 "Copy Constructor for Lines",
                 py::arg("Lines"))
            .def("add_line", &CLines::AddLine,
                 "Add another line",
                 py::arg("points"))
            .def("add_lines", &CLines::AddLine,
                 "Add array of lines",
                 py::arg("points_matrix"));

    py::class_<CPointCloud>(m, "PointCloud", model_component)
            .def(py::init<const vector<CPoint>&, const coord_t, vector<coord_t>&, const string& >(),
                 "Constructor for Point Cloud",
                 py::arg("points"), py::arg("quan"), py::arg("alpha"), py::arg("label"))
            .def(py::init<const CPointCloud&>(),
                 "Copy Constructor for Point Cloud",
                 py::arg("point_cloud"))
            .def("add_points", &CPointCloud::AddPoints,
                 "Add Points to the Point Cloud",
                 py::arg("points"), py::arg("quan"));

    py::class_<CMesh>(m, "Mesh", model_component)
            .def(py::init<const CMesh &> (),
                 "copy constructor for Mesh",
                 py::arg("mesh"))
            .def("reduce", &CMesh::Reduce,
                 "input values should be between 0 and 1. A Reduce algorithm for the surface, reduces file size while trying to maintain the the shape as much as possible. it's recommended to not over do it.",
                 py::arg("decimation_percent") = 0.5, py::arg("error") = 0.1)
            .def("export_to_obj", &CMesh::ExportToObj,
                 "writes the surface to an OBJ file, by materials or textures",
                 py::arg("output_file"), py::arg("with_texture") = 1)
            .def("laplacian_smooth", &CMesh::LaplacianSmooth,
                 "Smooths the surface by HC Laplacian Algorithm.",
                 py::arg("num_of_iterations"), py::arg("alpha_weight"), py::arg("beta_weight"));

    py::class_<CModel>(m, "Model")
            .def(py::init<> (), "default constructor for CModel")
            .def(py::init<vector<CModelComponent>& >(),
                 "constructor for CModel, from meshes, lines, and point clouds",
                 py::arg("meshes"))
            .def(py::init<vector<CSurface>&, string, coord_t> (),
                 "constructor for CModel, from surfs",
                 py::arg("surfaces"), py::arg("label") = "VIVID_3D_MODEL", py::arg("alpha") = 1)
            .def("add_meshes", &CModel::AddMesh,
                 "add more meshes, lines, or point clouds to Model",
                 py::arg("meshes"))
            .def("add_mesh", &CModel::AddMesh,
                 "add another mesh, lines, or point clouds to Model",
                 py::arg("mesh"))
            .def("add_surf", &CModel::AddSurf,
                 "add another surf to model",
                 py::arg("surf"), py::arg("label") = "VIVID_3D_MODEL", py::arg("alpha") = 1)
            .def("get_meshes", &CModel::GetMeshes,
                 "Returns the list of meshes held by model")
            .def("export_to_obj", &CModel::ExportToObj,
                 "writes the surface to an OBJ file, by materials or textures",
                 py::arg("output_file"), py::arg("with_texture") = 1);

    //Animations:
//    m.def("animation", &Animate,
//          "Takes a numpy array of CModels, an output location and an interval and creates a FBX animation containing a model in each frame",
//          py::arg("models"),py::arg("interval"), py::arg("output_file"));
//    m.def("rotating_animation", &RotateAnim,
//          "takes a model and creates an animation of it rotating",
//          py::arg("model"), py::arg("length"), py::arg("duration"), py::arg("rotation_axis"), py::arg("output_file"));
//    m.def("animation_textures", &AnimateTextures,
//          "Takes a numpy array of CModels, an output location and an interval and creates a FBX animation containing a model in each frame",
//          py::arg("models"), py::arg("interval"),py::arg("output_file"));
//    m.def("rotating_animation_textures", &RotateAnimTextures,
//          "takes a model and creates an animation of it rotating",
//          py::arg("model"), py::arg("length"), py::arg("duration"), py::arg("rotation_axis"), py::arg("output_file"));

    //Shapes:
    m.def("create_cube", &CreateCubeMesh,
          "Creates a cube mesh",
          py::arg("position") = CPoint(0,0,0), py::arg("size") = 1,
          py::arg("color") = 0.5, py::arg("alpha") = 1., py::arg("label")="");
    m.def("create_box", &CreateBoxMesh,
          "Creates a box mesh",
          py::arg("position") = CPoint(0, 0, 0), py::arg("size") = CPoint(1,1,1),
          py::arg("color") = 0.5, py::arg("alpha") = 1., py::arg("label")="");
    m.def("create_sphere", &CreateSphereMesh,
          "Creates a sphere",
          py::arg("position")=CPoint(0,0,0),py::arg("radius")=1,py::arg("num_of_meridians")=20,py::arg("num_of_parallels")=20,
          py::arg("color")=0.5, py::arg("alpha")=1., py::arg("label")="");
    m.def("create_ellipsoid", &CreateEllipsoidMesh,
          "Creates ellipsoid mesh",
          py::arg("position")=CPoint(0,0,0),py::arg("radii"),py::arg("num_of_meridians")=20,py::arg("num_of_parallels")=20,
          py::arg("major_axis"), py::arg("middle_axis"), py::arg("minor_axis"),
          py::arg("color")=0.5, py::arg("alpha")=1., py::arg("label")="");
    m.def("create_arrow", &CreateArrowMesh,
          "Creates an arrow mesh",
          py::arg("position")=CPoint(0,0,0), py::arg("direction"),py::arg("width")=0.5, py::arg("pointer_chest_ratio")=0.4,
          py::arg("color")=0.5, py::arg("alpha") = 1, py::arg("label")="");
    m.def("create_grid", &CreateGrid,
          "Creates a grid",
          py::arg("size")=10, py::arg("num_of_ticks")=5, py::arg("tick_size")=1);

    //SurfByFunc:
}