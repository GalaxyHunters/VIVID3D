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

py::array_t<coord_t> make_cpoint(const coord_t aValue) {
    py::array_t<coord_t> array (3);
    py::buffer_info buffer = array.request();
    auto ptr = static_cast<coord_t *>(buffer.ptr);
    for (int i = 0; i < 3; i++) {
        ptr[i] = aValue;
    }
    return array;
}

PYBIND11_MODULE(utilities, m) {
    m.doc() = R"mydelimitter(
Core Module
===========

This Module contains the VIVID3D primary classes and functions for creating and exporting 3D models and animations.

)mydelimitter";

    py::class_<CBlobData>(m, "BlobData", R"mydelimiter(
Blob files containing exported file data.

Attributes
----------
n_files : number
    Number of files held
names : list[str]
    List of file names
files : list[bytes]
    List of files held

        )mydelimiter")
        .def_readonly("n_files", &CBlobData::mNumFiles, "Number of files held")
        .def_readonly("names", &CBlobData::mNames, "List of file names held")
        .def_property_readonly("files", [](const CBlobData& arSelf) {
            std::vector<py::bytes> bytes;
            for (const auto& file : arSelf.mFiles) {
                bytes.push_back(py::bytes(file));
            }
            return bytes;
        }, "List of files held. All files are binary encoded arrays");


    // submodule Shapes:
    
    py::module_::import(".core");
    m.def("create_plane", &CreatePlaneMesh,R"mydelimiter(
Create a 2D Plane mesh

Parameters
----------
position : Point3D, default: [0,0,0]
    The center of the mesh.
size : float, default: 1.0
    The distance of the vertices from the position.
color : str, default: 'lightgray'
    The color of the resulting mesh.
opacity : (0.0-1.0), default: 1.0
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.
label : str, default: 'plane'
    Optional name of the object, not required to be unique. 

Returns
-------
mesh : vivid3d.Mesh
    The 2D Plane mesh

        )mydelimiter", py::arg("position") = make_cpoint(0), py::arg("size") = 1,
        py::arg("color")=DEFAULT_COLOR_SHAPES, py::arg("opacity") = 1., py::arg("label")="plane");
    m.def("create_cube", &CreateCubeMesh, R"mydelimiter(
Create a Cube mesh

Parameters
----------
position : Point3D, default: [0,0,0]
    The center of the mesh.
size : float, default: 1.0
    The distance of the vertices from the position.
color : str, default: 'lightgray'
    The color of the resulting mesh.
opacity : (0.0-1.0), default: 1.0
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.
label : str, default: 'cube'
    Optional name of the object, not required to be unique.

Returns
-------
mesh : vivid3d.Mesh
    The cube mesh

        )mydelimiter", py::arg("position") = make_cpoint(0), py::arg("size") = 1, py::arg("color")=DEFAULT_COLOR_SHAPES, 
        py::arg("opacity") = 1., py::arg("label")="cube");
    m.def("create_box", &CreateBoxMesh, R"mydelimiter(
Create a Box mesh

Parameters
----------
position : Point3D, default: [0,0,0]
    The center of the mesh.
size : Point3D, default: [1,1,1]
    The distance of the vertices[X,Y,Z] from the position[X,Y,Z].
color : str, default: 'lightgray'
    The color of the resulting mesh.
opacity : (0.0-1.0), default: 1.0
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.
label : str, default: 'box'
    Optional name of the object, not required to be unique.

Returns
-------
mesh : vivid3d.Mesh
    The box mesh

        )mydelimiter", py::arg("position") = make_cpoint(0), py::arg("size") = make_cpoint(1),
          py::arg("color")=DEFAULT_COLOR_SHAPES, py::arg("opacity") = 1., py::arg("label")="box");
    m.def("create_sphere", &CreateSphereMesh, R"mydelimiter(
Create a Sphere mesh

Parameters
----------
position : Point3D, default: [0,0,0]
    The center of the mesh.
radius : float, default: 1.0
    The radius from the position.
num_of_meridians : int, default: 20
    How many horizontal slices to subdivide the sphere mesh.
num_of_parallels : int, default: 20
    How many vertical slices to subdivide the sphere mesh.
color : str, default: 'lightgray'
    The color of the resulting mesh.
opacity : (0.0-1.0), default: 1.0
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.
label : str, default: 'sphere'
    Optional name of the object, not required to be unique.

Returns
-------
mesh : vivid3d.Mesh
    The sphere mesh

        )mydelimiter", py::arg("position")=make_cpoint(0),py::arg("radius")=1,py::arg("num_of_meridians")=20,py::arg("num_of_parallels")=20,
        py::arg("color")=DEFAULT_COLOR_SHAPES, py::arg("opacity")=1., py::arg("label")="sphere");
    m.def("create_ellipsoid", &CreateEllipsoidMesh, R"mydelimiter(
Create an Ellipsoid mesh

Parameters
----------------
position : Point3D, default: [0,0,0]
    The center of the mesh.
radii : Point3D
    The major, middle, and minor axis radii.
num_of_meridians : int, default: 20
    How many horizontal slices to subdivide the sphere mesh.
num_of_parallels : int, default: 20
    How many vertical slices to subdivide the sphere mesh.
major_axis : Point3D
    The unit vector for ellipsoid major axis.
middle_axis : Point3D
    The unit vector for ellipsoid middle axis.
minor_axis : Point3D
    The unit vector for ellipsoid minor axis.
color : str, default: 'lightgray'
    The color of the resulting mesh.
opacity : (0.0-1.0), default: 1.0
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.
label : str, default: 'ellipsoid'
    Optional name of the object, not required to be unique.

Returns
----------------
mesh : vivid3d.Mesh
    The ellipsoid mesh

        )mydelimiter", py::arg("position")=make_cpoint(0),py::arg("radii"),py::arg("num_of_meridians")=20,py::arg("num_of_parallels")=20,
        py::arg("major_axis"), py::arg("middle_axis"), py::arg("minor_axis"),
        py::arg("color")=DEFAULT_COLOR_SHAPES, py::arg("opacity")=1., py::arg("label")="ellipsoid");
    m.def("create_arrow", &CreateArrowMesh, R"mydelimiter(
Create an Arrow mesh

Parameters
----------------
position : Point3D, default: [0,0,0]
    The center of the mesh.
direction : Point3D
    The direction and length of the arrow from the position.
width : float, default: 0.5
    The width of the arrow mesh. 
pointer_chest_ratio : float, default: 0.4
    The pointer to chest ratio.
color : str, default: 'lightgray'
    The color of the resulting mesh.
opacity : (0.0-1.0), default: 1.0
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.
label : str, default: 'arrow'
    Optional name of the object, not required to be unique.

Returns
----------------
mesh : vivid3d.Mesh
    The arrow mesh

        )mydelimiter", py::arg("position")=make_cpoint(0), py::arg("direction"),py::arg("width")=0.5, py::arg("pointer_chest_ratio")=0.4,
        py::arg("color")=DEFAULT_COLOR_SHAPES, py::arg("opacity") = 1, py::arg("label")="arrow");
    m.def("create_grid", &CreateGrid,R"mydelimiter(
Create a Grid mesh, origin is at [0,0,0]

Parameters
----------------
size : float, default: 1.0
    The size of the grid from [0,0,0]
num_of_ticks : int, default: 5
    The number of ticks between each major tick.
tick_size: float, default: 1
    The size of ticks.

Returns
----------------
vivid3d.Lines
    The grid mesh

        )mydelimiter", py::arg("size")=10, py::arg("num_of_ticks")=5, py::arg("tick_size")=1);

    // submodule SurfByFunc:

    // submodule Logging:
    py::enum_<ELogCode>(m, "LogCode")
        .value("LOG_ERROR", LOG_ERROR)
        .value("LOG_WARNING", LOG_WARNING)
        .value("LOG_VIVID", LOG_VIVID)
        .value("LOG_INFO", LOG_INFO)
        .value("LOG_DEBUG", LOG_DEBUG)
        .export_values();

    m.def("config_logger", &ConfigLogging, R"mydelimiter(
Configure the VIVID3D in-built Logging Process

Parameters
----------
write_to_console : bool, default: True
    Print all Logging output to console.
log_level : vivid3d.LogCode, default: LOG_VIVID
    Log all messages above or at this log level.
write_to_file : bool, default: false
    Write all Logging output to file.
log_file_label : str, default: 'VIVID_LOG.txt'
    File name for Logging.

        )mydelimiter", py::arg("write_to_console") = true, py::arg("log_level") = LOG_VIVID, 
            py::arg("write_to_file") = false, py::arg("log_file_name") = "VIVID_LOG.txt");
}
