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

PYBIND11_MODULE(visual, m) {
    m.doc() = R"mydelimitter(
Visual
======

This Module contains the VIVID3D primary classes and functions for creating and exporting 3D models and animations.

)mydelimitter";

    py::class_<CMaterial>(m, "Material", R"mydelimiter(
Simplified PBR Material for physically based rendering of Meshes

Attributes
----------
opacity : (0.0-1.0), default: 1.0
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.
metalness : (0.0-1.0), default: 0.5
    Float in the range of 0.0 - 1.0 indicating the reflectiveness quality of the material, where 0 is completely non-reflective and 1 is completely reflective. This variable is affected by the roughness parameter as well.
roughness : (0.0-1.0), default: 0.5
    Float in the range of 0.0 - 1.0 indicating the surface texture quality of the material, where 0 is completely smooth and 1 is completely rough. This variable affects the look of the reflection of a surface.
emission_strength : Positive Float, default: 0.0
    Positive Float indicating the self-illumination strength of the material.
label : str, default: "defaultMaterial"
    Optional name of the object, not required to be unique.

    )mydelimiter")
        .def(py::init<normal_float, normal_float, normal_float, float, const string&  >(),
            py::arg("opacity")=1, py::arg("metalness")=0.5, py::arg("roughness")=0.5, py::arg("emission_strength")=0, py::arg("label")="defaultMaterial")
        .def_property("label", &CMaterial::GetLabel, &CMaterial::SetLabel, R"mydelimiter(
            label : str
                Optional name of the object, not required to be unique.

                )mydelimiter")
        .def_property("opacity", &CMaterial::GetOpacity, &CMaterial::SetOpacity, R"mydelimiter(
            opacity (0.0-1.0)
                Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.

                )mydelimiter")
        .def_property("metalness", &CMaterial::GetMetalness, &CMaterial::SetMetalness, R"mydelimiter(
            roughness : (0.0-1.0)
                Float in the range of 0.0 - 1.0 indicating the surface texture quality of the material, where 0 is completely smooth and 1 is completely rough. This variable affects the look of the reflection of a surface.
   
                )mydelimiter")
        .def_property("roughness", &CMaterial::GetRoughness, &CMaterial::SetRoughness, R"mydelimiter(
            metalness : (0.0-1.0)
                Float in the range of 0.0 - 1.0 indicating the reflectiveness quality of the material, where 0 is completely non-reflective and 1 is completely reflective. This variable is affected by the roughness parameter as well.
                
                )mydelimiter")
        .def_property("emission_strength", &CMaterial::GetEmissionStrength, &CMaterial::SetEmissionStrength, R"mydelimiter(
            emission_strength : Positive Float
                Positive Float indicating the self-illumination strength of the material.
    
                )mydelimiter");

    // dictionary with list of colormaps
    // dictionary with list of colors
}
