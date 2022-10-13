#include "TypeCasting.h"
#include "VoronoiVolume.h"
#include "ModelComponent.h"
#include "Mesh.h"
#include "Line.h"
#include "PointCloud.h"
#include "Shapes.h"
#include "Model.h"
#include "Animation.h"
#include "StopMotionAnimation.h"
#include <vector>
#include <array>
#include "Vivify.h"
/*#include "./ImportAndExport/FBXImportExport.h"*/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#define PYBIND11_DETAILED_ERROR_MESSAGES

using namespace vivid;
using namespace std;

namespace py = pybind11;
using namespace py::literals;
constexpr auto DEFAULT_COLOR_SHAPES = "lightgray";

py::array_t<double> make_cpoint(const double aValue) {
    py::array_t<double> array (3);
    py::buffer_info buffer = array.request();
    auto ptr = static_cast<double *>(buffer.ptr);
    for (int i = 0; i < 3; i++) {
        ptr[i] = aValue;
    }
    return array;
}

py::array_t<unsigned char> make_color_t(const unsigned char aValue) {
    py::array_t<unsigned char> array (3);
    py::buffer_info buffer = array.request();
    auto ptr = static_cast<unsigned char *>(buffer.ptr);
    for (int i = 0; i < 3; i++) {
        ptr[i] = aValue;
    }
    return array;
}

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(_vivid, m) {
    m.doc() = R"pbdoc(
        =======================
        vivid3d - Documentation
        =======================

        .. currentmodule:: vivid3d

        Vivify functions
        ================

        .. autosummary::
           :toctree: _generate
           :caption: Vivify functions

           make_model

        Shape functions
        ================

        .. autosummary::
           :toctree: _generate
           :caption: Shape functions

           add
           create_cube
           create_box
           create_sphere
           create_ellipsoid
           create_arrow
           create_grid

        classes
        =======

        .. autosummary::
           :toctree: _generate
           :caption: Classes

           Point
           ColorMap
           Material
           Lines
           PointCloud
           VoronoiVolume
           Mesh
           Model
           Animation
           StopMotionAnimation
	)pbdoc";


    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers
        Some other explanation about the subtract function.
    )pbdoc");
    
    py::class_<CPoint>(m, "Point")
//        .doc() = "VIVID Point Class"
        .def(py::init<const CPoint &>(),
                "copy constructor for Point",
                py::arg("point"))
        .def(py::init<coord_t, coord_t, coord_t>(),
             "Constructor for point",
             py::arg("x"), py::arg("y"), py::arg("z"))
        .def(py::init<std::vector<double>>(),
                 "Constructor for point",
                 py::arg("3d_vector"));

    py::class_<CColorMap>(m, "ColorMap")
        .def(py::init<const string&>(),
            py::arg("name"))
        .def(py::init<vector<array<float, 3>>&, string&>(),
            py::arg("colors"), py::arg("name"))
        .def(py::init<const CColorMap&>(),
             py::arg("ColorMap"))
        .def_property_readonly("name", &CColorMap::GetName)
        .def_property_readonly("colors", &CColorMap::GetColorMap);
    py::class_<PyColorMap, CColorMap>(m, "PyColorMap");

    py::class_<CMaterial>(m, "Material", R"pbdoc(
        Material obj
    )pbdoc")
        .def(py::init<normal_float, normal_float, float, const color_t&, const string& >(),
            py::arg("opacity")=1, py::arg("shininess")=0.1, py::arg("emission_strength")=0, py::arg("emission_color")=make_color_t(0), py::arg("name")="default")
        .def(py::init<normal_float, normal_float, float, const string&, const string&  >(),
            py::arg("opacity")=1, py::arg("shininess")=0.1, py::arg("emission_strength")=0, py::arg("emission_color")="white", py::arg("name")="default")
        .def_property("opacity", &CMaterial::GetOpacity, &CMaterial::SetOpacity,
            "Opacity (0.0-1.0)")
        .def_property("shininess", &CMaterial::GetShininess, &CMaterial::SetShininess,
            "Shininess (0.0-1.0)")
        .def_property("emission_strength", &CMaterial::GetEmissionStrength, &CMaterial::SetEmissionStrength,
            "Emission Strength (0.0-1.0)")
        .def("set_emission_color", py::overload_cast<const string&>(&CMaterial::SetEmissionColor),
            "Set Emission Color",
            py::arg("emission_color_name"));

    py::class_<CModelComponent> (m, "ModelComponent")
        .def(py::init<const CModelComponent&> (),
            "Constructor for ModelComponent",
            py::arg("model_component"))
//        .doc("Parent class from which Mesh, Point Cloud, and Lines inherit.")
        .def_property("name", &CModelComponent::GetLabel, &CModelComponent::SetLabel)
        .def_property_readonly("n_vertices", &CModelComponent::GetPointsCount)
        .def_property_readonly("n_polygons", &CModelComponent::GetFacesCount)
        .def_property("opacity", &CModelComponent::GetOpacity, &CModelComponent::SetOpacity,
              "Opacity (0.0-1.0)")
        .def_property("material",  &CModelComponent::GetMaterial, &CModelComponent::SetMaterial)
        .def("set_color", &CModelComponent::SetColor,
             "Set Color", py::arg("color"))
        .def("set_color_map", py::overload_cast<const CColorMap&>(&CModelComponent::SetColorMap),
             "Set Color Map", py::arg("ColorMap"))
        .def("set_color_map", py::overload_cast<const PyColorMap&>(&CModelComponent::SetColorMap),
             "Set Color Map", py::arg("ColorMap"))
        .def("transform", py::overload_cast<const array<CPoint, 3>&>(&CModelComponent::TransformMesh),
             "Transform Model Component by transformation matrix",
             py::arg("matrix"))
        .def("rotate", &CModelComponent::RotateMesh,
            "Rotate the Model Component points around a normal vector by an angle, counterclockwise.",
            py::arg("norm_vec"), py::arg("radians_angle"))
        .def("move", &CModelComponent::MoveMesh,
            "Move Model Component by direction_vec",
            py::arg("direction_vec"))
        .def("scale", &CModelComponent::ScaleMesh,
            "Scale Model Component by scale_vec.",
            py::arg("scale_vec"))
        .def("export", &CModelComponent::Export,
            "writes CMesh to a given file format",
            py::arg("output_file"), py::arg("file_format") = "gltf2")
        .def("__str__", [](const CModelComponent& arMC) {
            return "vivid3d.__vivid.ModelComponent\nName: " + arMC.GetLabel() + "\nVertices: " + to_string(arMC.GetPointsCount()) + "\nFaces: " + to_string(arMC.GetFacesCount());
        });

    py::class_<CVoronoiVolume>(m, "VoronoiVolume")
            .def(py::init<const vector<CPoint>&, vector<normal_float>&, normal_float, normal_float, coord_t>(),
                 "constructor function for surface",
                 py::arg("points"), py::arg("color_field") = vector<normal_float>(0), py::arg("color_field_min") = 0, py::arg("color_field_max") = 0, py::arg("noise_displacement") = 0.001) //color_field basic value = vector<coord_t>(0)
            .def(py::init<const CVoronoiVolume &> (),
                 "copy constructor for Surface",
                 py::arg("surf"))
            .def("create_surface", &CVoronoiVolume::CreateSurface,
                 "Calculate the surface from input data)")
            .def("to_mesh", &CVoronoiVolume::MaskMesh,
                 "Returns an Iso-Surface mesh made by the input mask.",
                 py::arg("mask"), py::arg("label") = "VIVID_3D_MODEL", py::arg("alpha") = 1);

    // Main Classes
    py::class_<CLines, CModelComponent>(m, "Lines", R"pbdoc(
        Lines objects
    )pbdoc")
            .def(py::init<const vector<CPoint>&, const normal_float, const string& >(),
                 "Constructor for Lines",
                 py::arg("line"), py::arg("opacity")=1., py::arg("label")="")
//            .def(py::init<const vector<vector<CLines>>&, const coord_t, const string&>(),
//                 "Constructor for Lines",
//                 py::arg("array_of_lines"), py::arg("opacity")=1., py::arg("label")="")
            .def(py::init<const CLines&>(),
                 "Copy Constructor for Lines",
                 py::arg("Lines"))
            .def("add_line", &CLines::AddLine,
                 "Add another line",
                 py::arg("line"));
//            .def("add_lines", &CLines::AddLine,
//                 "Add array of lines",
//                 py::arg("points_matrix"));

    py::class_<CPointCloud, CModelComponent>(m, "PointCloud", R"pbdoc(
        Point cloud
    )pbdoc")
            .def(py::init<const std::vector<CPoint>&, const std::string&, normal_float, const std::string&>(),
                 "Constructor for Point Cloud",
                 py::arg("points"), py::arg("color")="white", py::arg("opacity") = 1, py::arg("label")= "VIVID_POINT_CLOUD")
            .def(py::init<const std::vector<CPoint>&, vector<normal_float>&, normal_float, normal_float, normal_float, const std::string&>(),
                 "Constructor for Point Cloud",
                 py::arg("points"), py::arg("color_field") = vector<normal_float>(0), py::arg("color_field_min") = 0, py::arg("color_field_max") = 0, py::arg("opacity") = 1, py::arg("label")= "VIVID_POINT_CLOUD")
            .def(py::init<const CPointCloud&>(),
                 "Copy Constructor for Point Cloud",
                 py::arg("point_cloud"))
            .def("add_points", &CPointCloud::AddPoints,
                 "Add Points to the Point Cloud",
                 py::arg("points"), py::arg("color_field") = vector<normal_float>(0), py::arg("color_field_min") = 0, py::arg("color_field_max") = 0)
            .def("generate_mesh", &CPointCloud::CreateVoronoiVolume,
                 "Generate 3D Mesh using Voronoi Algorithm",
                 py::arg("noise_displacement") = 0.001);

    py::class_<CMesh, CModelComponent>(m, "Mesh", R"pbdoc(
        Basic Mesh class
    )pbdoc")
            .def(py::init<const CMesh &> (),
                 "copy constructor for Mesh",
                 py::arg("mesh"))
            .def("reduce", &CMesh::Reduce,
                 "input values should be between 0 and 1. A Reduce algorithm for the surface, reduces file size while trying to maintain the the shape as much as possible. it's recommended to not over do it.",
                 py::arg("decimation_percent") = 0.5, py::arg("error") = 0.1)
//            .def("export_to_obj", &CMesh::ExportToObj,
//                 "writes the surface to an OBJ file, by materials or textures",
//                 py::arg("output_file"), py::arg("with_texture") = 1)
            .def("smooth", &CMesh::LaplacianSmooth,
                 "Smooths the surface by HC Laplacian Algorithm.",
                 py::arg("num_of_iterations"), py::arg("alpha_weight"), py::arg("beta_weight"));

    py::class_<CModel>(m, "Model", R"pbdoc(
        Model class for working with multiple Meshes
    )pbdoc")
            .def(py::init<> (), "default constructor for Model")
            .def(py::init<vector<CModelComponent>& >(),
                 "constructor for CModel, from meshes, lines, and point clouds",
                 py::arg("meshes"))
            .def("add_meshes", &CModel::AddMeshes,
                 "add more meshes, lines, or point clouds to Model",
                 py::arg("meshes"))
            .def("add_mesh", &CModel::AddMesh,
                 "add another mesh, lines, or point clouds to Model",
                 py::arg("mesh"))
            .def_property_readonly("meshes", &CModel::GetMeshes,
                 "Returns the list of meshes held by model")
            .def_property_readonly("n_meshes", &CModel::GetNumMeshes,
                "Returns the number of meshes held by model")
            .def("export_to_obj", &CModel::ExportToObj,
                 "writes the surface to an OBJ file, by materials or textures",
                 py::arg("output_file"), py::arg("with_texture") = 1)
            .def("export", &CModel::Export,
                 "writes CModel to a given file format",
                 py::arg("output_file"), py::arg("file_format") = "gltf2")
            .def("__repr__", [](const CModel &a){return "<vivid3d.Model>";});

    py::class_<CAnimation> Animation(m,"Animation", R"pbdoc(
        Class for animations
    )pbdoc");
        Animation.def(py::init<> (), "default constructor for Animation")
            .def(py::init<const CModel &> (),
                 "constructor for Animation from a single Model",
                 py::arg("model"))
            .def(py::init<const vector<CModel> &> (),
                 "constructor for Animation from an np array of Models",
                 py::arg("models"))
            .def(py::init<const CAnimation &> (),
                 "copy constructor for animation",
                 py::arg("animation"))
            .def_property("duration", &CAnimation::GetDuration, &CAnimation::SetDuration, "duration(in ticks)")
            .def_property("ticks_per_second", &CAnimation::GetTicksPerSecond, &CAnimation::SetTicksPerSecond, "ticks")
            .def("get_models", &CAnimation::GetModels,
                 "getter function for models in the animation")
            .def("add_models", static_cast<void (CAnimation::*)(const CModel &)>(&CAnimation::AddModels),"add a model to animation", py::arg("models"))
            .def("add_models", static_cast<void (CAnimation::*)(const vector<CModel> &)>(&CAnimation::AddModels),"add models to animation", py::arg("models"))
            .def("set_move_animation", &CAnimation::SetMoveAnim,
                 "set movement animation for index model",
                 py::arg("index"), py::arg("move_vector"))
            .def("set_rotate_animation", &CAnimation::SetRotateAnim,
                 "set rotate animation for index model",
                 py::arg("index"), py::arg("anguler_rotate"))
            .def("set_scale_animation", &CAnimation::SetScaleAnim,
                 "set scale animation for index model",
                 py::arg("index"), py::arg("scale_vector"))
            .def("get_move_animation", &CAnimation::GetMoveAnim,
                 "get movement animation for given model index",
                 py::arg("index"))
            .def("get_rotate_animation", &CAnimation::GetRotateAnim,
                 "get rotate animation for given model index",
                 py::arg("index"))
            .def("get_scale_animation", &CAnimation::GetScaleAnim,
                 "get scale animation for given model index",
                 py::arg("index"))
            .def("export", &CAnimation::Export,
                 "Exports animation to selected file format",
                 py::arg("output_file"), py::arg("file_format") = "gltf2");


    py::class_<CStopMotionAnimation>(m,"StopMotionAnimation", Animation, R"pbdoc(
        Class for stop motion animations
    )pbdoc")
        .def(py::init<> (), "default constructor for StopMotionAnimation")
        .def(py::init<const CModel &, double> (),
            "constructor for StopMotionAnimation from a single Model",
            py::arg("model"), py::arg("mSecondsPerFrame")) //py::arg("model"), py::arg("mSecondsPerFrame")
        .def(py::init<const vector<CModel> &, double> (),
            "constructor for StopMotionAnimation from an np array of Models",
            py::arg("models"), py::arg("mSecondsPerFrame")) //py::arg("models"), py::arg("mSecondsPerFrame")
        .def(py::init<const CAnimation &, double> (),
            "constructor for StopMotionAnimation from animation",
            py::arg("animation"), py::arg("mSecondsPerFrame")) // py::arg("animation"), py::arg("mSecondsPerFrame")
        .def(py::init<const CStopMotionAnimation &> (),
            "copy constructor for StopMotionAnimation",
            py::arg("animation"))
        .def_property("seconds_per_frame", &CStopMotionAnimation::GetSecondsPerFrame, &CStopMotionAnimation::SetSecondsPerFrame,
             "Seconds per frame");


    //one liners
    m.def("make_model", py::overload_cast<const std::vector<CPoint> &, const std::vector<bool> &, const std::string &,
        std::vector<normal_float> &, normal_float , normal_float , const string& ,
        normal_float , coord_t>(&vivifyMesh), R"pbdoc(
            one function to rule them all

            makes a model from one line, if an output path is given the functions writes the model to file.
        )pbdoc",
        py::arg("input_points"),
        py::arg("mask"),
        py::arg("output_path") = "",
        py::arg("ColorField") = vector<normal_float>(0),
        py::arg("color_field_min") = 0,
        py::arg("color_field_max") = 0,
        py::arg("label")= "VIVID_MODEL",
        py::arg("opacity") = 1,
        py::arg("noise_displacement") = 0.001);


    m.def("make_model", py::overload_cast<const std::vector<CPoint> &, const std::vector<normal_float> &, normal_float,
        const std::string &, std::vector<normal_float> &, normal_float , normal_float , const string& ,
        normal_float , coord_t>(&vivifyMesh), R"pbdoc(
            one function to rule them all

            makes a model from one line, if an output path is given the functions writes the model to file.
        )pbdoc",
        py::arg("input_points"),
        py::arg("surface_field"),
        py::arg("surface_threshold"),
        py::arg("output_path") = "",
        py::arg("ColorField") = vector<normal_float>(0),
        py::arg("color_field_min") = 0,
        py::arg("color_field_max") = 0,
        py::arg("label")= "VIVID_MODEL",
        py::arg("opacity") = 1,
        py::arg("noise_displacement") = 0.001);

    m.def("make_model", py::overload_cast<const std::vector<CPoint> &, const std::vector<std::vector<bool>> &, const std::string &,
        std::vector<normal_float> &, normal_float , normal_float , const std::string& ,
        vector<normal_float> &, coord_t>(&vivifyModel), R"pbdoc(
            one function to rule them all

            makes a model from one line, if an output path is given the functions writes the model to file.
        )pbdoc",
        py::arg("input_points"),
        py::arg("masks"),
        py::arg("output_path") = "",
        py::arg("ColorField") = vector<normal_float>(0),
        py::arg("color_field_min") = 0,
        py::arg("color_field_max") = 0,
        py::arg("label")= "VIVID_MODEL",
        py::arg("opacity") = vector<normal_float>(0),
        py::arg("noise_displacement") = 0.001);


    m.def("make_model", py::overload_cast<const std::vector<CPoint> &, const std::vector<normal_float> &, std::vector<normal_float> &,
        const std::string &, std::vector<normal_float> &, normal_float , normal_float , const std::string&,
        std::vector<normal_float> &, coord_t >(&vivifyModel), R"pbdoc(
            one function to rule them all

            makes a model from one line, if an output path is given the functions writes the model to file.
        )pbdoc",
        py::arg("input_points"),
        py::arg("surface_field"),
        py::arg("surface_thresholds"),
        py::arg("output_path") = "",
        py::arg("ColorField") = vector<normal_float>(0),
        py::arg("color_field_min") = 0,
        py::arg("color_field_max") = 0,
        py::arg("label")= "VIVID_MODEL",
        py::arg("opacity") = vector<normal_float>(0),
        py::arg("noise_displacement") = 0.001);


    //Shapes:
    //m.def("create_plane")
    m.def("create_cube", &CreateCubeMesh,
          "Creates a cube mesh",
          py::arg("position") = make_cpoint(0), py::arg("size") = 1,
          py::arg("color")=DEFAULT_COLOR_SHAPES, py::arg("alpha") = 1., py::arg("label")="");
    m.def("create_box", &CreateBoxMesh,
          "Creates a box mesh",
          py::arg("position") = make_cpoint(0), py::arg("size") = make_cpoint(1),
          py::arg("color")=DEFAULT_COLOR_SHAPES, py::arg("alpha") = 1., py::arg("label")="");
    m.def("create_sphere", &CreateSphereMesh,
          "Creates a sphere",
          py::arg("position")=make_cpoint(0),py::arg("radius")=1,py::arg("num_of_meridians")=20,py::arg("num_of_parallels")=20,
          py::arg("color")=DEFAULT_COLOR_SHAPES, py::arg("alpha")=1., py::arg("label")="");
    m.def("create_ellipsoid", &CreateEllipsoidMesh,
          "Creates ellipsoid mesh",
          py::arg("position")=make_cpoint(0),py::arg("radii"),py::arg("num_of_meridians")=20,py::arg("num_of_parallels")=20,
          py::arg("major_axis"), py::arg("middle_axis"), py::arg("minor_axis"),
          py::arg("color")=DEFAULT_COLOR_SHAPES, py::arg("alpha")=1., py::arg("label")="");
    m.def("create_arrow", &CreateArrowMesh,
          "Creates an arrow mesh",
          py::arg("position")=make_cpoint(0), py::arg("direction"),py::arg("width")=0.5, py::arg("pointer_chest_ratio")=0.4,
          py::arg("color")=DEFAULT_COLOR_SHAPES, py::arg("alpha") = 1, py::arg("label")="");
    m.def("create_grid", &CreateGrid,
          "Creates a grid",
          py::arg("size")=10, py::arg("num_of_ticks")=5, py::arg("tick_size")=1);

    //SurfByFunc:

    // Utils:
    py::enum_<ELogCode>(m, "LogCode")
        .value("LOG_ERROR", LOG_ERROR)
        .value("LOG_WARNING", LOG_WARNING)
        .value("LOG_VIVID", LOG_VIVID)
        .value("LOG_INFO", LOG_INFO)
        .value("LOG_DEBUG", LOG_DEBUG)
        .export_values();

    m.def("config_logger", &ConfigLogging,
          "Configure the VIVID in-built Logging Process",
          py::arg("write_to_console") = true, py::arg("log_level") = LOG_VIVID, py::arg("write_to_file") = false, py::arg("log_file_name") = "VIVID_LOG.txt");
}
