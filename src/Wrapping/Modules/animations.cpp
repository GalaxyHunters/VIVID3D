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


PYBIND11_MODULE(animations, m) {
    m.doc() = R"mydelimitter(
Visual
======

This Module contains the VIVID3D primary classes and functions for creating and exporting 3D models and animations.

)mydelimitter";
    py::module_::import(".core");

    py::class_<CFrame>(m, "Frame", R"mydelimiter(
        Basic Frame struct, capable of move, rotate and scale.

        Attributes
        ----------
        model : vivid3d.Model
            frame model
        move_animation : Point3D
            movement animation for this frame
        rotate_animation : Point3D
            rotate animation for this frame
        scale_animation : Point3D
            scale animation for this frame

        )mydelimiter")
        .def_readwrite("move_animation", &CFrame::mMoveAnimation)
        .def_readwrite("rotate_animation", &CFrame::mRotateAnimation)
        .def_readwrite("scale_animation", &CFrame::mScaleAnimation)
        .def_readwrite("model", &CFrame::mModel);

    py::class_<CAnimation> Animation(m,"Animation", R"mydelimiter(
        Basic Animation class, capable of move, rotate and scale.
        
        Attributes
        ----------
        frames : list[vivid3d.Frame]
            List of frames held, each model holds his own animation.
        duration : float
            Animation duration in ticks
        ticks_per_second  : float
            ticks per second
        
        Methods
        -------
        add_model(model: vivid3d.model)
            add a model to the animation
        add_models(models: list[vivid3d.model])
            add a list of models to the animation
        export(output_file: str, file_format = 'glb')
            export the animation to file, file formats: glb, gltf, fbx
        export_to_blob(file_format = 'glb')
            Writes Model to Blob with given file_type format.

        )mydelimiter");
    Animation.def(py::init<> (), "default constructor for Animation")
    .def(py::init<const CModel &> (),R"mydelimiter(
            constructor for Animation from a single Model.

            Parameters
            ----------
            model : vivid3d.Model,
                model to initialize the animation with
        )mydelimiter", py::arg("model"))
    .def(py::init<const vector<CModel> &> (),R"mydelimiter(
            constructor for Animation from a list of Models.

            Parameters
            ----------
            models : list[vivid3d.Model],
                models to initialize the animation with
        )mydelimiter", py::arg("models"))
    .def(py::init<const CAnimation &> (),
        R"mydelimiter(
            copy constructor for animation

            Parameters
            ----------
            animation : vivid3d.Animation,
                animation to initialize the Animation
        )mydelimiter", py::arg("animation"))
    .def_property("duration", &CAnimation::GetDuration, &CAnimation::SetDuration, "duration(in ticks)")
    .def_property("ticks_per_second", &CAnimation::GetTicksPerSecond, &CAnimation::SetTicksPerSecond, "ticks")
    .def_property("frames", &CAnimation::GetFrames, &CAnimation::SetFrames,"animation frames")
    .def("add_model", &CAnimation::AddModel,R"mydelimiter(
            Add a model to the animation.

            Parameters
            ----------
            model : vivid3d.Model,
                model to add
        )mydelimiter", py::arg("model"))
    .def("add_models", &CAnimation::AddModels,R"mydelimiter(
            Add a list of models to the animation.

            Parameters
            ----------
            models : list[vivid3d.Model],
                models to add
        )mydelimiter", py::arg("models"))
    .def("show", [](CAnimation &arSelf, int aHeight) {
            auto viewer = py::module_::import("vivid3d.viewer");
            return viewer.attr("show")(arSelf, aHeight);
        }, R"mydelimiter(
            Render and view the model in a viewer window.

            Parameters
            ----------
            height : int, default: 600
                height in pixels to open the viewer

            Returns
            -------
            html : str or IPython.display.HTML
                The HTML with embedded model

        )mydelimiter", py::arg("height")=600)
    .def("export_to_blob", &CAnimation::ExportToBlob, R"mydelimiter(
        Exports Animation to Blob with given file_type format. For full list of supported file formats, see: https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types

        Parameters
        ----------
        file_type : str, default: 'glb'
            File format to export to

        Returns
        -------
        blob : vivid3d.BlobData
            Object containing exported model file blobs
            )mydelimiter",
    py::arg("file_type") = "glb")
    .def("export", &CAnimation::Export,
         R"mydelimiter(
    Exports Animation to output_file with given file_type format. For full list of supported file formats, see: https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types

    Parameters
    -------------
    output_file : str, default: "Vivid3dModel"
        File Directory to export to
    file_type : str, default: 'glb'
        File format to export to

        )mydelimiter",
         py::arg("output_file"), py::arg("file_format") = "glb");


    py::class_<CStopMotionAnimation>(m,"StopMotionAnimation", Animation, R"mydelimiter(
        Stop motion animation class, inherits from vivid3d.animation but present the models frame after frame

        Attributes
        ----------
        frames : list[vivid3d.Frame]
            List of frames held, each model holds his own animation.
        seconds_per_frame : float
            How long should each frame last
        ticks_per_second  : float
            ticks per second

        Methods
        -------
        add_model(model: vivid3d.model)
            add a model to the animation
        add_models(models: list[vivid3d.model])
            add a list of models to the animation
        export(output_file: str, file_format = 'glb')
            export the animation to file, file formats: glb, gltf, fbx
        export_to_blob(file_format = 'glb')
            Writes Model to Blob with given file_type format.


    )mydelimiter")
        .def(py::init<> (), "default constructor for StopMotionAnimation")
        .def(py::init<const CModel &, coord_t> (),
        R"mydelimiter(
            constructor for StopMotionAnimation from a single Model.

            Parameters
            ----------
            model : vivid3d.Model,
                model to initialize the animation with
            seconds_per_frame : float, default: 2
                seconds for frame
        )mydelimiter",
            py::arg("model"), py::arg("seconds_per_frame") = 2) 
        .def(py::init<const vector<CModel> &, coord_t> (),
        R"mydelimiter(
            constructor for StopMotionAnimation from a list of Models.

            Parameters
            ----------
            models : list[vivid3d.Model],
                models to initialize the animation with
            seconds_per_frame : float, default: 2
                seconds for frame
        )mydelimiter",
            py::arg("models"), py::arg("seconds_per_frame") = 2) 
        .def(py::init<const CAnimation &, coord_t> (),
        R"mydelimiter(
            constructor for StopMotionAnimation from animation

            Parameters
            ----------
            animation : vivid3d.Animation,
                animation to initialize the StopMotionAnimation
            seconds_per_frame : float, default: 2
                seconds for frame
        )mydelimiter",
            py::arg("animation"), py::arg("seconds_per_frame") = 2) 
        .def(py::init<const CStopMotionAnimation &> (),
        R"mydelimiter(
            copy constructor for StopMotionAnimation

            Parameters
            ----------
            StopMotionAnimation : vivid3d.StopMotionAnimation,
                StopMotionAnimation to initialize the StopMotionAnimation
        )mydelimiter",
            py::arg("animation"))
        .def_property("seconds_per_frame", &CStopMotionAnimation::GetSecondsPerFrame, &CStopMotionAnimation::SetSecondsPerFrame,
             "Seconds per frame");
}