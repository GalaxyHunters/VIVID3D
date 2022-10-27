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
#include "BlobData.h"
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


PYBIND11_MODULE(_core, m) {
    m.doc() = R"mydelimiter(
        =============
        API Reference
        =============

        .. currentmodule:: vivid3d


        Core Api
        ========

        make_model - one liner model function
        -------------------------------------
        .. autosummary::
           :toctree: _generate
           :caption: Vivify functions

           make_model


        Model Objects
        ------------
        .. autosummary::
           :toctree: _generate
           :caption: Model Objects

           Point
           BaseMesh
           Lines
           PointCloud
           VoronoiVolume
           Mesh
           Model
           BlobData

        Animation Objects
        -----------------
        .. autosummary::
           :toctree: _generate
           :caption: Animation Objects

           Animation
           StopMotionAnimation


        Material and Textures
        ---------------------
        .. autosummary::
           :toctree: _generate
           :caption: Material and Textures

           ColorMap
           Material


        Utilities
        =========

        Geometry
        ------
        .. autosummary::
           :toctree: _generate
           :caption: Geometry
           
           create_plane 
           create_cube
           create_box
           create_sphere
           create_ellipsoid
           create_arrow
           create_grid

        Logger
        ------
        .. autosummary::
           :toctree: _generate
           :caption: Logger

           config_logger
           LogCode

	)mydelimiter";

    py::class_<CBlobData>(m, "BlobData", R"mydelimiter(
        Blob files containing exported file data.
        )mydelimiter")
        .def_readonly("n_files", &CBlobData::mNumFiles, "Number of files held in blob.")
        .def_readonly("names", &CBlobData::mNames, "List of file names held in blob.")
        .def_property_readonly("files", [](const CBlobData& arSelf) {
            std::vector<py::bytes> bytes;
            for (const auto& file : arSelf.mFiles) {
                bytes.push_back(py::bytes(file));
            }
            return bytes;
        }, R"mydelimiter(
        List of files held in blob. All files are binary encoded arrays.
        )mydelimiter");

    py::class_<CPoint>(m, "Point")
        .def(py::init<const CPoint &>(),
                "copy constructor for Point",
                py::arg("point"))
        .def(py::init<coord_t, coord_t, coord_t>(),
             "Constructor for point",
             py::arg("x"), py::arg("y"), py::arg("z"))
        .def(py::init<std::vector<double>>(),
                 "Constructor for point",
                 py::arg("3d_vector"));

    py::class_<CColorMap>(m, "ColorMap", R"mydelimiter(
        ColorMap class for mapping point color field scalars to colors

        Attributes
        ------
        name : str, default: "plasma"
        colors : array-like of shape (n,3), default : plasma colormap
        )mydelimiter")
        .def(py::init<>(), "Default Constructor - Plasma Colormap")
        .def(py::init<const string&>(),
            py::arg("name"), "Set color by name")
        .def(py::init<vector<array<float, 3>>&, string&>(),
            py::arg("colors"), py::arg("name"), "Custom colors and name")
        .def(py::init<const CColorMap&>(),
             py::arg("ColorMap"))
        .def_property_readonly("name", &CColorMap::GetName)
        .def_property_readonly("colors", &CColorMap::GetColorMap);
    py::class_<PyColorMap, CColorMap>(m, "PyColorMap");

    py::class_<CMaterial>(m, "Material", R"mydelimiter(
        Material obj
    )mydelimiter")
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

    py::class_<CModelComponent> (m, "BaseMesh", R"mydelimiter(
                Inheritable parent class for : Mesh, Points, Lines

                Attributes
                -------------
                name : str
                n_vertices : number
                    # of vertices
                n_polygons : number
                    # of vertices
                material : vivid3d.Material
                    The material instance used by 3D renderers
                colormap : vivid3d.ColorMap
                    The Colormap instance used to map scalar data to colors
                opacity : number
                    The opacity when rendering, unit value (0.0-1.0)
                )mydelimiter")
        .def(py::init<const CModelComponent&> (),
            "Copy Constructor for ModelComponent",
            py::arg("model_component"))
//        .doc("Parent class from which Mesh, Point Cloud, and Lines inherit.")
        .def_property("name", &CModelComponent::GetLabel, &CModelComponent::SetLabel)
        .def_property_readonly("n_vertices", &CModelComponent::GetPointsCount)
        .def_property_readonly("n_polygons", &CModelComponent::GetFacesCount)
        .def_property("material",  &CModelComponent::GetMaterial, &CModelComponent::SetMaterial)
        .def_property_readonly("colormap", &CModelComponent::GetColorMap)
        .def_property("opacity", &CModelComponent::GetOpacity, &CModelComponent::SetOpacity,
            "Opacity (0.0-1.0)")
        .def("set_color", &CModelComponent::SetColor,
             "Set Color", py::arg("color"))
        .def("set_colormap", py::overload_cast<const CColorMap&>(&CModelComponent::SetColorMap),
             "Set Color Map", py::arg("ColorMap"))
        .def("set_colormap", py::overload_cast<const PyColorMap&>(&CModelComponent::SetColorMap), R"mydelimiter(
                Set the colormap used for mapping scalar values to colors

                Parameters
                -------------
                colormap : str or matplotlib.colors.Colormap
                    Can be set with any matplotlib registered colormap
            )mydelimiter", py::arg("colormap"))
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
        .def("export", [](CModelComponent arSelf, string output_file, string file_type) {
                if (output_file.empty()) {
                    return arSelf.ExportToBlob(file_type);
                } else {
                    arSelf.Export(output_file, file_type);
                    return CBlobData();
                }
            }, R"mydelimiter(
            Writes Component to output_file with given file format.
            If no output_file provided, this function will return a blob file in the given file format.

            Parameters
            -------------
            output_file : str, default: ""
                File Directory to export to
            file_type : str, default: glb
                File format to export to

            Returns
            -------------
            blob : bytes
                Object containing exported model - only returned if output_file is not provided
            )mydelimiter",
             py::arg("output_file")="", py::arg("file_type") = "glb")
        .def("show", [](CModelComponent& arSelf, int aHeight) {
                auto viewer = py::module_::import("vivid3d.viewer");
                return viewer.attr("show")(arSelf, aHeight);
            }, R"mydelimiter(
            Render and view the mesh in a viewer window. **Requires IPython**

            Params
            -------------
            height : int, default: 600
                height in pixels to open the viewer

            Returns
            -------------
            html : IPython.display.HTML
                Object containing rendered Mesh
            )mydelimiter", py::arg("height")=600)
        .def("__str__", [](const CModelComponent& arSelf) {
            return "vivid3d.ModelComponent\nName: " + arSelf.GetLabel() + "\nVertices: " + to_string(arSelf.GetPointsCount()) + "\nFaces: " + to_string(arSelf.GetFacesCount());
        });

    py::class_<CVoronoiVolume>(m, "VoronoiVolume", R"mydelimiter(
        Inheritable parent class for : Mesh, Points, Lines

        Attributes
        -------------
        name : str
        n_vertices : number
            # of vertices
        n_polygons : number
            # of vertices
        material : vivid3d.Material
            The material instance used by 3D renderers
        colormap : vivid3d.ColorMap
            The Colormap instance used to map scalar data to colors
        opacity : number
            The opacity when rendering, unit value (0.0-1.0)
        )mydelimiter")
        .def(py::init<const vector<CPoint>&, vector<normal_float>&, normal_float, normal_float, coord_t>(),
             "constructor function for surface",
             py::arg("points"), py::arg("color_field") = vector<normal_float>(0), py::arg("color_field_min") = 0, py::arg("color_field_max") = 0, py::arg("noise_displacement") = 0) //color_field basic value = vector<coord_t>(0)
        .def(py::init<const CVoronoiVolume &> (),
             "copy constructor for Surface",
             py::arg("surf"))
        .def("to_mesh", &CVoronoiVolume::MaskMesh,
             "Returns an Iso-Surface mesh made by the input mask.",
             py::arg("mask"), py::arg("label") = "VIVID_3D_MODEL", py::arg("alpha") = 1);

    // Main Classes
    py::class_<CLines, CModelComponent>(m, "Lines", R"mydelimiter(
        Lines object that holds 3D Lines
        )mydelimiter")
        .def(py::init<normal_float, const string&>(), R"mydelimiter(
            Lines Empty Constructor

            Parameters
            -------------
            opacity : normalized float, default: 1.0
                Opacity of Component
            label : str, default: ""
                Label of Component
            )mydelimiter",
             py::arg("opacity")=1., py::arg("label")="")
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

    py::class_<CPointCloud, CModelComponent>(m, "PointCloud", R"mydelimiter(
        Point cloud
    )mydelimiter")
            .def(py::init<normal_float, const std::string&>(),
                 "Empty constructor for PointCloud",
                 py::arg("opacity") = 1, py::arg("label")= "VIVID_POINT_CLOUD")
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

    py::class_<CMesh, CModelComponent>(m, "Mesh", R"mydelimiter(
        Triangular Mesh class that holds polygonal surfaces.
        )mydelimiter")
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

    py::class_<CModel>(m, "Model", R"mydelimiter(
        Model class for working with multiple Meshes
    )mydelimiter")
        .def(py::init<> (), "default constructor for Model")
        .def(py::init<vector<CModelComponent>& >(),
             "constructor for CModel, from meshes, lines, and point clouds",
             py::arg("meshes"))
        .def(py::init<CModelComponent& >(),
             "constructor for CModel, from mesh, line, and point cloud",
             py::arg("mesh"))
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
        .def("export", [](CModel &arSelf, string output_file, string file_type) {
            if (output_file.empty()) {
                return arSelf.ExportToBlob(file_type);
            } else {
                arSelf.Export(output_file, file_type);
                return CBlobData();
            }
        }, R"mydelimiter(
            Writes Component to output_file with given file format.
            If no output_file provided, this function will return a blob file in the given file format.

            Parameters
            -------------
            output_file : str, default: ""
                File Directory to export to
            file_type : str, default: glb
                File format to export to

            Returns
            -------------
            blob : bytes
                Object containing exported model - only returned if output_file is not provided
            )mydelimiter",
             py::arg("output_file")="", py::arg("file_type") = "glb")
        .def("show", [](CModel &arSelf, int aHeight) {
                auto viewer = py::module_::import("vivid3d.viewer");
                return viewer.attr("show")(arSelf, aHeight);
            }, R"mydelimiter(
            Render and view the mesh in a viewer window. **Requires IPython**

            Params
            -------------
            height : int, default: 500
                height in pixels to open the viewer

            Returns
            -------------
            html : IPython.display.HTML
                Object containing rendered Mesh
            )mydelimiter", py::arg("height")=500)
        .def("__repr__", [](const CModel &arSelf){return "vivid3d.Model\nnMeshes " + to_string(arSelf.GetNumMeshes());});

    py::class_<CAnimation> Animation(m,"Animation", R"mydelimiter(
            Class for animations
        )mydelimiter");
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


    py::class_<CStopMotionAnimation>(m,"StopMotionAnimation", Animation, R"mydelimiter(
        Class for stop motion animations
    )mydelimiter")
        .def(py::init<> (), "default constructor for StopMotionAnimation")
        .def(py::init<const CModel &, double> (),
            "constructor for StopMotionAnimation from a single Model",
            py::arg("model"), py::arg("seconds_per_frame") = 2) //py::arg("model"), py::arg("mSecondsPerFrame")
        .def(py::init<const vector<CModel> &, double> (),
            "constructor for StopMotionAnimation from an np array of Models",
            py::arg("models"), py::arg("seconds_per_frame") = 2) //py::arg("models"), py::arg("mSecondsPerFrame")
        .def(py::init<const CAnimation &, double> (),
            "constructor for StopMotionAnimation from animation",
            py::arg("animation"), py::arg("seconds_per_frame") = 2) // py::arg("animation"), py::arg("mSecondsPerFrame")
        .def(py::init<const CStopMotionAnimation &> (),
            "copy constructor for StopMotionAnimation",
            py::arg("animation"))
        .def_property("seconds_per_frame", &CStopMotionAnimation::GetSecondsPerFrame, &CStopMotionAnimation::SetSecondsPerFrame,
             "Seconds per frame");


    //one liners
    m.def("make_model", py::overload_cast<const std::vector<CPoint> &, const std::vector<bool> &, const std::string &,
        std::vector<normal_float> &, normal_float , normal_float , const string& ,
        normal_float, const std::string&, coord_t>(&vivifyMesh), R"mydelimiter(
        one function to rule them all

        makes a model from one line, if an output path is given the functions writes the model to file.

        Parameters
        ----------
        input_points : array[array[float]]
            The input point data in x,y,z form.
        mask : array[bool]
            Boolean mask of true and false points
        output_path : str, default: ""
            Path and name for output file
            If left as None no Module will be written to file.
        color_field : array[float], default: []
            Vector containing the color field of each point
            If left empty color will be basic white
        color_field_min : float, optional
            Set minimum value for color_field, anything below will be set to color_field_min
            If left empty will be set to min(color_field)
        color_field_max : float, optional
            Set max value for color_field, anything below will be set to color_field_max
            If left empty will be set to max(color_field)
        label : str, default: "VIVID_MODEL"
            Label to assign to the model, some file format support it
        opacity : float, default: 1
            Alpha value for the model, 0-1
        file_type : str, default: "gltf2"
            File format for export, out of Assimp supported formats
        noise_displacement : float, default: 0.001
            The Voronoi algorithm struggles with equidistant point data, a small noise displacement improves algorithm speed

        Returns
        -------
        vivid3d.Model
            Output model.

        See Also
        --------
        vivid3d.Model
            Main class holding 3d models data in the vivid3d package, also see examples.
        )mydelimiter",
        py::arg("input_points"),
        py::arg("mask"),
        py::arg("output_path") = "",
        py::arg("color_field") = vector<normal_float>(0),
        py::arg("color_field_min") = 0,
        py::arg("color_field_max") = 0,
        py::arg("label")= "VIVID_MODEL",
        py::arg("opacity") = 1,
        py::arg("file_type") = "gltf2",
        py::arg("noise_displacement") = 0.001);


    m.def("make_model", py::overload_cast<const std::vector<CPoint> &, const std::vector<normal_float> &, normal_float,
        const std::string &, std::vector<normal_float> &, normal_float , normal_float , const string& ,
        normal_float , const std::string&, coord_t>(&vivifyMesh), R"mydelimiter(
        one function to rule them all

        makes a model from one line, if an output path is given the functions writes the model to file.
        )mydelimiter",
        py::arg("input_points"),
        py::arg("surface_field"),
        py::arg("surface_threshold"),
        py::arg("output_path") = "",
        py::arg("color_field") = vector<normal_float>(0),
        py::arg("color_field_min") = 0,
        py::arg("color_field_max") = 0,
        py::arg("label")= "VIVID_MODEL",
        py::arg("opacity") = 1,
        py::arg("file_type") = "gltf2",
        py::arg("noise_displacement") = 0.001);

    m.def("make_model", py::overload_cast<const std::vector<CPoint> &, const std::vector<std::vector<bool>> &, const std::string &,
        std::vector<normal_float> &, normal_float , normal_float , const std::string& ,
        vector<normal_float> &, const std::string&, coord_t>(&vivifyModel), R"mydelimiter(
        one function to rule them all

        makes a model from one line, if an output path is given the functions writes the model to file.
        )mydelimiter",
        py::arg("input_points"),
        py::arg("masks"),
        py::arg("output_path") = "",
        py::arg("color_field") = vector<normal_float>(0),
        py::arg("color_field_min") = 0,
        py::arg("color_field_max") = 0,
        py::arg("label")= "VIVID_MODEL",
        py::arg("opacity") = vector<normal_float>(0),
        py::arg("file_type") = "gltf2",
        py::arg("noise_displacement") = 0.001);


    m.def("make_model", py::overload_cast<const std::vector<CPoint> &, const std::vector<normal_float> &, std::vector<normal_float> &,
        const std::string &, std::vector<normal_float> &, normal_float , normal_float , const std::string&,
        std::vector<normal_float> &, const std::string&, coord_t >(&vivifyModel), R"mydelimiter(
        one function to rule them all

        makes a model from one line, if an output path is given the functions writes the model to file.
        )mydelimiter",
        py::arg("input_points"),
        py::arg("surface_field"),
        py::arg("surface_thresholds"),
        py::arg("output_path") = "",
        py::arg("color_field") = vector<normal_float>(0),
        py::arg("color_field_min") = 0,
        py::arg("color_field_max") = 0,
        py::arg("label")= "VIVID_MODEL",
        py::arg("opacity") = vector<normal_float>(0),
        py::arg("file_type") = "gltf2",
        py::arg("noise_displacement") = 0.001);


    //Shapes:
    m.def("create_plane", &CreatePlaneMesh,
        py::arg("position") = make_cpoint(0), py::arg("size") = 1,
        py::arg("color")=DEFAULT_COLOR_SHAPES, py::arg("alpha") = 1., py::arg("label")="");
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
