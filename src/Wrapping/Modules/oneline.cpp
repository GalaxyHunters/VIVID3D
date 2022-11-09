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

PYBIND11_MODULE(oneline, m) {
    m.doc() = R"mydelimitter(
One Liner
==========

Stuff

)mydelimitter";
    py::module_::import(".core");

   m.def("make_model", py::overload_cast<const std::vector<CPoint> &, const std::vector<std::vector<bool>> &, const std::string &,
       std::vector<normal_float> &, normal_float , normal_float , const std::string& ,
       vector<normal_float> &, const std::string&, coord_t>(&vivifyModel), R"mydelimiter(
makes a model from one line, using a vector of masks, each mask will result in a surface, see voronoi user guide for further detail.
if an output path is given the functions writes the model to file.

Parameters
----------
input_points : array[array[float]]
    The input point data in x,y,z form.
mask : array[array[bool]]
    array of masks, Each inner array is a Boolean mask of true and false points, has to be the same size as input_points.
output_path : str, optional
    Path and name for output file
    If given the model will be written to file, otherwise not.
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
    Optional name of the object, not required to be unique.
opacity : array[float], default: []
    Array of Floats in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.
file_type : str, default: "glb"
    File format for export, out of Assimp supported formats
noise_displacement : float, default: 0.001
    The Voronoi algorithm struggles with equidistant point data, a small noise displacement improves algorithm speed

Returns
-------
vivid3d.Model
    Output model.
        
       )mydelimiter",
       py::arg("input_points"),
       py::arg("masks"),
       py::arg("output_path") = "",
       py::arg("color_field") = vector<normal_float>(0),
       py::arg("color_field_min") = 0,
       py::arg("color_field_max") = 0,
       py::arg("label")= "VIVID_MODEL",
       py::arg("opacity") = vector<normal_float>(0),
       py::arg("file_type") = "glb",
       py::arg("noise_displacement") = 0);


   m.def("make_model", py::overload_cast<const std::vector<CPoint> &, const std::vector<normal_float> &, std::vector<normal_float> &,
       const std::string &, std::vector<normal_float> &, normal_float , normal_float , const std::string&,
       std::vector<normal_float> &, const std::string&, coord_t >(&vivifyModel), R"mydelimiter(
makes a model from one line, using surface field and a vector of thresholds, each threshold resulting in a surface.
if an output path is given the functions writes the model to file.

Parameters
----------
input_points : array[array[float]]
    The input point data in x,y,z form.
surface_field : array[float]
    surface field value for each point, has to be same size as input_points.
surface_thresholds : array[float]
    thresholds to create surfaces.
output_path : str, optional
    Path and name for output file
    If given the model will be written to file, otherwise not.
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
    Optional name of the object, not required to be unique.
opacity : array[float], default: []
    Array of Floats in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.
file_type : str, default: "glb"
    File format for export, out of Assimp supported formats
noise_displacement : float, default: 0.001
    The Voronoi algorithm struggles with equidistant point data, a small noise displacement improves algorithm speed

Returns
-------
vivid3d.Model
    Output model.
        
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
       py::arg("file_type") = "glb",
       py::arg("noise_displacement") = 0);
}
