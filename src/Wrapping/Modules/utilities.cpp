#include "TypeCasting.h"
#include "VoronoiVolume.h"
#include "geometry.cpp"
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

    py::module geometry = m.def_submodule("geometry", R"mydelimiter(
        Geometry
        ========

        Submodule meant to provide simple methods for creating basic shapes for enhancing your visualizations
    )mydelimiter");
    defineGeometrySubmodule(geometry);
    
    // submodule SurfByFunc:

    // submodule Logging:
    y::module logging = m.def_submodule("logging", R"mydelimiter(
        Logging
        ========

        Submodule that provides simple interface to VIVID3D's logging process.
    )mydelimiter");
    defineLoggingSubmodule(logging);
}
