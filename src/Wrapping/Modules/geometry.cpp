#include "TypeCasting.h"
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

void defineGeometrySubmodule(const py::module& m) {
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

}