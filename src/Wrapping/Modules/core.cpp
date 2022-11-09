#include "cpoint.h"
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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#define PYBIND11_DETAILED_ERROR_MESSAGES

using namespace vivid;
using namespace std;

namespace py = pybind11;
using namespace py::literals;

py::array_t<coord_t> make_cpoint(const coord_t aValue) {
    py::array_t<coord_t> array (3);
    py::buffer_info buffer = array.request();
    auto ptr = static_cast<coord_t *>(buffer.ptr);
    for (int i = 0; i < 3; i++) {
        ptr[i] = aValue;
    }
    return array;
}

PYBIND11_MODULE(core, m) {
    m.doc() = R"mydelimitter(
Core Module
===========

This Module contains the VIVID3D primary classes and functions for creating and exporting 3D models and animations.

)mydelimitter";

    // imports
    py::module_::import(".visual");
    py::module_::import(".utilities").attr("BlobData");

    // if possible remove this
    py::class_<CPoint>(m, "Point")
        .def(py::init<const CPoint &>(),
                "copy constructor for Point",
                py::arg("point"))
        .def(py::init<coord_t, coord_t, coord_t>(),
             "Constructor for point",
             py::arg("x"), py::arg("y"), py::arg("z"))
        .def(py::init<std::vector<coord_t>>(),
                 "Constructor for point",
                 py::arg("3d_vector"));

    py::class_<CModelComponent> (m, "BaseMesh", R"mydelimiter(
Inheritable parent class for : vivid3d.Mesh, vivid3d.PointCloud, vivid3d.Lines

Attributes
----------
label : str
    Optional name of the object, not required to be unique.
n_polygons : number
    # of vertices
n_vertices : number
    # of vertices
material : vivid3d.Material
    The material instance used by 3D renderers
colormap : matplotlib.colors.ColorMap, default: "plasma"
    The Colormap instance used to map scalar data to colors.
    See https://matplotlib.org/stable/gallery/color/colormap_reference for list of all supported colormaps.
opacity : number
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.

Methods
-------
set_color(color: string)
    Set the mesh texture to a single color. See https://htmlcolorcodes.com/color-names/ for list of all available colors.
transform(matrix: ndarray(3,3))
    Transform the Mesh by matrix
rotate(norm_vec: Point3D, radians_angle: float)
    Rotate the Mesh around the norm_vec by radians_angle
scale(scale_vec: Point3D)
    Scale the Mesh by scale_ved
move(direction_vec: Point3D)
    Move the Mesh by direction_vec
show(height=600) -> IPython.display.HTML or None
    Render and view the mesh in a viewer window.
export(output_file: str, file_type='glb')
    Writes BaseMesh to output_file with given file format. See https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types for list of all supported file formats.
export_to_blob(file_type='glb') -> vivid3d.BlobData
    Writes BaseMesh to vivid3d.BlobData with given file format. See https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types for list of all supported file formats.

)mydelimiter")
    .def(py::init<const CModelComponent&> (),
        "Copy Constructor for BaseMesh",
        py::arg("model_component"))
    .def_property("label", &CModelComponent::GetLabel, &CModelComponent::SetLabel)
    .def_property_readonly("n_polygons", &CModelComponent::GetFacesCount, "# of polygons")
    .def_property_readonly("n_vertices", &CModelComponent::GetPointsCount, "# of vertices")
    .def_property("material", &CModelComponent::GetMaterial, &CModelComponent::SetMaterial, "The material instance used by 3D renderers")
    .def_property("colormap", &CModelComponent::GetColorMap, &CModelComponent::SetColorMap, "The Colormap instance used to map scalar data to colors.")
    .def_property("opacity", &CModelComponent::GetOpacity, &CModelComponent::SetOpacity,
        "Opacity (0.0-1.0)")
    .def("set_color", &CModelComponent::SetColor,
         R"mydelimiter(
Set the mesh texture to a single color. 

Parameters
----------
color : str
    See https://htmlcolorcodes.com/color-names/ for list of all available colors.

        )mydelimiter", py::arg("color"))
    .def("transform", py::overload_cast<const array<CPoint, 3>&>(&CModelComponent::TransformMesh),
         "Transform Mesh by transformation matrix", py::arg("matrix"))
    .def("rotate", &CModelComponent::RotateMesh,
R"mydelimiter(
Rotate the Mesh points around a normal vector by an angle in radians

Parameters
----------
norm_vec : Point3D (normalized)
    Unit vector from (0,0,0) of the BaseMesh
radians_angle : float
    The angle to rotate in radians

        )mydelimiter", py::arg("norm_vec"), py::arg("radians_angle"))
    .def("move", &CModelComponent::MoveMesh,
R"mydelimiter(
Moves Mesh in X,Y,Z by direction(X,Y,Z)

Parameters
----------
direction : Point3D
    The vector to move by

        )mydelimiter", py::arg("direction"))
    .def("scale", &CModelComponent::ScaleMesh,
        R"mydelimiter(
Scales Mesh in X,Y,Z by scale(X,Y,Z)

Parameters
----------
scale : Point3D
    The vector to scale by

        )mydelimiter", py::arg("scale"))
    .def("export", &CModelComponent::Export, R"mydelimiter(
        Exports Mesh to output_file with given file_type format. For full list of supported file formats, see: https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types

        Parameters
        -------------
        output_file : str, default: "Vivid3dModel"
            File Directory to export to
        file_type : str, default: 'glb'
            File format to export to

            )mydelimiter",
    py::arg("output_file")="Vivid3dModel", py::arg("file_type") = "glb")
    .def("export_to_blob", &CModelComponent::ExportToBlob, R"mydelimiter(
            Exports Mesh to Blob with given file_type format. For full list of supported file formats, see: https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types

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
    .def("show", [](CModelComponent& arSelf, int aHeight) {
            auto viewer = py::module_::import("vivid3d.viewer");
            return viewer.attr("show")(arSelf, aHeight);
        }, R"mydelimiter(
Render and view the mesh in a viewer window.

Parameters
----------
height : int, default: 600
    height in pixels to open the viewer

Returns
-------
html : str or IPython.display.HTML
    The html with embedded model.

        )mydelimiter", py::arg("height")=600)
    .def("__str__", [](const CModelComponent& arSelf) {
        return "vivid3d.BaseMesh\nlabel: " + arSelf.GetLabel() + "\nVertices: " + to_string(arSelf.GetPointsCount()) + "\nFaces: " + to_string(arSelf.GetFacesCount());
    });

// Main Classes
    py::class_<CMesh, CModelComponent>(m, "Mesh", R"mydelimiter(
Triangular Mesh class that holds polygonal surfaces.


Attributes
----------
label : str
    Optional name of the object, not required to be unique.
n_polygons : number
    # of vertices
n_vertices : number
    # of vertices
material : vivid3d.Material
    The material instance used by 3D renderers
colormap : matplotlib.colors.ColorMap, default: "plasma"
    The Colormap instance used to map scalar data to colors.
    See https://matplotlib.org/stable/gallery/color/colormap_reference for list of all supported colormaps.
opacity : number
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.

Methods
-------
set_color(color: string)
    Set the mesh texture to a single color. See https://htmlcolorcodes.com/color-names/ for list of all available colors.
transform(matrix: ndarray(3,3))
    Transform the Mesh by matrix
rotate(norm_vec: Point3D, radians_angle: float)
    Rotate the Mesh around the norm_vec by radians_angle
scale(scale_vec: Point3D)
    Scale the Mesh by scale_ved
move(direction_vec: Point3D)
    Move the Mesh by direction_vec
show(height=600) -> IPython.display.HTML or None
    Render and view the mesh in a viewer window.
export(output_file: str, file_type='glb')
    Writes BaseMesh to output_file with given file format. See https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types for list of all supported file formats.
export_to_blob(file_type='glb') -> vivid3d.BlobData
    Writes BaseMesh to vivid3d.BlobData with given file format. See https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types for list of all supported file formats.
smooth(n_iterations=5, opacity_weight=0.7, beta_weight=0.0)
    Smooths the surface by HC Laplacian Algorithm. 
    See http://informatikbuero.com/downloads/Improved_Laplacian_Smoothing_of_Noisy_Surface_Meshes.pdf for reference.
reduce(decimation_percent=0.5,max_error=0.1)
    Reduces Poly-Count of smoothed meshes. Prefer to create smaller models as it tends to have a cleaner result.
    See __example__ for down-sampling datasets.

        )mydelimiter")
    .def(py::init<const CMesh &> (),
        "copy constructor for Mesh", py::arg("mesh"))
    .def("reduce", &CMesh::Reduce, R"mydelimiter(
Reduces Poly-Count of smoothed meshes.

Reduces file size while trying to maintain the the shape as much as possible. It's recommended to not over do it.

Parameters
----------
decimation_percent : (0.0-1.0), default: 0.5
    Reduce the model polygon count to this percent.
max_error : (0.0-1.0), default = 0.5
    Maximum error in reduction. Recommended to not touch.

Notes
------
Prefer to create smaller models as it tends to have a cleaner result.
See __example__ for down-sampling datasets.

            )mydelimiter", py::arg("decimation_percent") = 0.5, py::arg("max_error") = 0.5)
        .def("smooth", &CMesh::LaplacianSmooth, R"mydelimiter(
Smooths the surface by HC Laplacian Algorithm. Recommended to use default values.

See http://informatikbuero.com/downloads/Improved_Laplacian_Smoothing_of_Noisy_Surface_Meshes.pdf for reference.

Parameters
----------
n_iterations : number, default: 5
    Number of iterations to smooth by
alpha_weight : (0.0-1.0), default: 0.7
    Percent to move the point by each iteration
beta_weight : (0.0-1.0), default: 0.0
    Percent to hold the point pack to original position. Used for high iteration smoothing, to retain total volume.
    If n_iterations > 20, recommended beta_weight = 0.5

        )mydelimiter", py::arg("n_iterations") = 5, py::arg("alpha_weight") = 0.7, py::arg("beta_weight") = 0.0);

    // TODO: Add color_field support to lines as in points.
    py::class_<CLines, CModelComponent>(m, "Lines", R"mydelimiter(
Lines class that holds 3d lines

Useful for plotting vector fields or trajectories.

Attributes
----------
label : str
    Optional name of the object, not required to be unique.
n_polygons : number
    # of vertices
n_vertices : number
    # of vertices
material : vivid3d.Material
    The material instance used by 3D renderers
colormap : matplotlib.colors.ColorMap, default: "plasma"
    The Colormap instance used to map scalar data to colors.
    See https://matplotlib.org/stable/gallery/color/colormap_reference for list of all supported colormaps.
opacity : number
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.

Methods
-------
set_color(color: string)
    Set the mesh texture to a single color. See https://htmlcolorcodes.com/color-names/ for list of all available colors.
transform(matrix: ndarray(3,3))
    Transform the Mesh by matrix
rotate(norm_vec: Point3D, radians_angle: float)
    Rotate the Mesh around the norm_vec by radians_angle
scale(scale_vec: Point3D)
    Scale the Mesh by scale_ved
move(direction_vec: Point3D)
    Move the Mesh by direction_vec
show(height=600) -> IPython.display.HTML or None
    Render and view the mesh in a viewer window.
export(output_file: str, file_type='glb')
    Writes BaseMesh to output_file with given file format. See https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types for list of all supported file formats.
export_to_blob(file_type='glb') -> vivid3d.BlobData
    Writes BaseMesh to vivid3d.BlobData with given file format. See https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types for list of all supported file formats.
add_line(line: Point3DArray)
    Add a new line

        )mydelimiter")
    .def(py::init<normal_float, const string&>(), R"mydelimiter(
Lines Empty Constructor

Parameters
----------
opacity : (0.0-1.0), default: 1.0
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.
label : str, default: "defaultMaterial"
    Optional name of the object, not required to be unique.

        )mydelimiter", py::arg("opacity")=1., py::arg("label")="")
//    .def(py::init<const vector<CPoint>&, const normal_float, const string& >(), R"mydelimiter(
//Lines Constructor
//
//Parameters
//----------------
//line : Point3DArray
//opacity : normalized float, default: 1.0
//Opacity of Component
//label : str, default: ""
//Label of Component
//        )mydelimiter", py::arg("line"), py::arg("opacity")=1., py::arg("label")="")
////            .def(py::init<const vector<vector<CLines>>&, const coord_t, const string&>(),
////                 "Constructor for Lines",
////                 py::arg("array_of_lines"), py::arg("opacity")=1., py::arg("label")="")
    .def(py::init<const CLines&>(),
         "Copy Constructor for Lines",
         py::arg("Lines"))
    .def("add_line", &CLines::AddLine,
         "Add another line",
         py::arg("line"));
//            .def("add_lines", &CLines::AddLine,
//                 "Add array of lines",
//                 py::arg("points_matrix"));

//TODO: Add VoronoiVolume analysis shit here.
py::class_<CVoronoiVolume>(m, "VoronoiVolume", R"mydelimiter(
VoronoiVolume class for Volumetric analysis and iso-surface extraction.

Methods
-------
iso_surface(mask: array_like(bool, n), label="isosurface", opacity=1)
    Extracts iso-surface by boolean mask of same size as input points, and returns vivid3d.Mesh instance.

        )mydelimiter")
    .def(py::init<const vector<CPoint>&, vector<normal_float>&, normal_float, normal_float, coord_t>(), R"mydelimiter(
Generates 3D Voronoi Volume from input points and scalar color_field.
See https://en.wikipedia.org/wiki/Voronoi_diagram for reference.

Parameters
----------
points : Point3DArray(n,3)
    array_like of 3D Points of shape (n,3)
color_field : array_like(n)[float], default: zeros(n)
    array_like of scalars to use for color_field of shape (n). If left empty will default to array of zeros.
color_field_min : float, default: 0.0
    Values below this will be clamped to this minimum. If null the minimum is automatically calculated
color_field_max : float, default: 0.0
    Values above this will be clamped to this maximum. If null the maximum is automatically calculated
noise_displacement : float, default: 0.0
    Sometimes helps fix edge cases where the algorithm fails. Use sparingly.

        )mydelimiter", py::arg("points"), py::arg("color_field") = vector<normal_float>(0), py::arg("color_field_min") = 0,
        py::arg("color_field_max") = 0, py::arg("noise_displacement") = 0)
    .def(py::init<const CVoronoiVolume &> (),
        "Copy constructor for VoronoiVolume",
        py::arg("voronoi_volume"))
    .def("iso_surface", &CVoronoiVolume::MaskMesh, R"mydelimiter(
Extracts iso-surface by boolean mask of same size as input points.

Only faces between a True and False point will be included in the final mesh. 
For a visual explanation, see https://vivid.readthedocs.io/en/latest/userguide/voronoi.html#the-voronoi-algorithm

Parameters
----------
mask : array_like(n)[bool]
    An (n, 1) shaped boolean array for creating the iso-surface.
    Only faces between a True and False point will be included in the final mesh.
label : str, default: "isosurface"
    Optional name of the object, not required to be unique.
opacity : (0.0-1.0), default: 1.0
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.

Returns
-------
vivid3d.Mesh
    The resulting iso-surface

        )mydelimiter", py::arg("mask"), py::arg("label") = "isosurface", py::arg("opacity") = 1);


    py::class_<CPointCloud, CModelComponent>(m, "PointCloud", R"mydelimiter(
PointCloud class for both surface and volumetric point clouds.

Can be analyzed with 3D Voronoi for iso-surface extraction.


Attributes
----------
label : str
    Optional name of the object, not required to be unique.
n_polygons : number
    # of vertices
n_vertices : number
    # of vertices
material : vivid3d.Material
    The material instance used by 3D renderers
colormap : matplotlib.colors.ColorMap, default: "plasma"
    The Colormap instance used to map scalar data to colors.
    See https://matplotlib.org/stable/gallery/color/colormap_reference for list of all supported colormaps.
opacity : number
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.

Methods
-------
set_color(color: string)
    Set the mesh texture to a single color. See https://htmlcolorcodes.com/color-names/ for list of all available colors.
transform(matrix: ndarray(3,3))
    Transform the Mesh by matrix
rotate(norm_vec: Point3D, radians_angle: float)
    Rotate the Mesh around the norm_vec by radians_angle
scale(scale_vec: Point3D)
    Scale the Mesh by scale_ved
move(direction_vec: Point3D)
    Move the Mesh by direction_vec
show(height=600) -> IPython.display.HTML or None
    Render and view the mesh in a viewer window.
export(output_file: str, file_type='glb')
    Writes BaseMesh to output_file with given file format. See https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types for list of all supported file formats.
export_to_blob(file_type='glb') -> vivid3d.BlobData
    Writes BaseMesh to vivid3d.BlobData with given file format. See https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types for list of all supported file formats.
add_points(points: Point3DArray(n,3), color_field: array_like(n), color_field_min=0, color_field_max=0)
    Append the points to the existing point cloud.
add_noise(max_displacement: float)
    Moves particles randomly by uniform distribution[-max_displacement, max_displacement] along x,y,z
to_volume(noise_displacement=0)
    Generate 3D Volume using the Voronoi algorithm.
    See https://en.wikipedia.org/wiki/Voronoi_diagram for explanation.
        )mydelimiter")
    .def(py::init<normal_float, const std::string&>(),
        "Empty constructor for PointCloud",
        py::arg("opacity") = 1, py::arg("label")= "VIVID_POINT_CLOUD")
    .def(py::init<const std::vector<CPoint>&, const std::string&, normal_float, const std::string&>(),  R"mydelimiter(
PointCloud constructor from points and scalar color_field

Parameters
----------
points : Point3DArray(n,3)
    array_like of 3D Points of shape (n,3)
color : str, default: 'white'
    Color to apply to all the points.
opacity : (0.0-1.0), default: 1.0
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.
label : str, default: 'pointCloud'
    Optional name of the object, not required to be unique.
        )mydelimiter", py::arg("points"), py::arg("color")="white", py::arg("opacity") = 1, py::arg("label")= "pointCloud")
    .def(py::init<const std::vector<CPoint>&, vector<normal_float>&, normal_float, normal_float, normal_float, const std::string&>(), R"mydelimiter(
PointCloud constructor from points and scalar color_field

Parameters
----------
points : Point3DArray(n,3)
    array_like of 3D Points of shape (n,3)
color_field : array_like(n)[float], default: zeros(n)
    array_like of scalars to use for color_field of shape (n). If left empty will default to array of zeros.
color_field_min : float, default: 0.0
    Values below this will be clamped to this minimum. If null the minimum is automatically calculated
color_field_max : float, default: 0.0
    Values above this will be clamped to this maximum. If null the maximum is automatically calculated
opacity : (0.0-1.0), default: 1.0
    Float in the range of 0.0 - 1.0 indicating how transparent the material is. A value of 0.0 indicates fully transparent, 1.0 is fully opaque.
label : str, default: 'pointCloud'
    Optional name of the object, not required to be unique.
            )mydelimiter", py::arg("points"), py::arg("color_field") = vector<normal_float>(0),
        py::arg("color_field_min") = 0, py::arg("color_field_max") = 0, py::arg("opacity") = 1, py::arg("label")= "pointCloud")
    .def(py::init<const CPointCloud&>(),
        "Copy Constructor for Point Cloud",
        py::arg("point_cloud"))
    .def("add_points", &CPointCloud::AddPoints,
        "Add Points to the Point Cloud",
        py::arg("points"), py::arg("color_field") = vector<normal_float>(0), py::arg("color_field_min") = 0, py::arg("color_field_max") = 0)
    .def("add_noise", &CPointCloud::Noisify, R"mydelimiter(
Moves particles randomly by uniform distribution[-max_displacement, max_displacement] along x,y,z axes.

Useful for breaking up grid-like appearance of PointClouds generated with grid based data.

Parameters
----------
max_displacement : float
    Maximum amount to move in along axes. It is recommended to keep below the cell size.

        )mydelimiter", py::arg("max_displacement"))
    .def("to_voronoi", &CPointCloud::CreateVoronoiVolume, R"mydelimiter(
Generate 3D Volume using the Voronoi algorithm.

See https://en.wikipedia.org/wiki/Voronoi_diagram for explanation.

Parameters
----------
noise_displacement : float, default: 0.0
    Sometimes helps fix edge cases where the algorithm fails. Use sparingly.

Returns
-------
volume : vivid3d.VoronoiVolume
    The calculated 3D Cell volume

            )mydelimiter", py::arg("noise_displacement") = 0);

    py::class_<CModel>(m, "Model", R"mydelimiter(
Model class for combining multiple BaseMesh instances into a single scene.

Attributes
----------
meshes : list[vivid3d.BaseMesh]
    List of meshes held
n_meshes : number
    Number of meshes held
n_polygons : number
    Number of total polygons
n_vertices : number
    Number of total vertices

Methods
-------
add_mesh(mesh: vivid3d.BaseMesh)
    Add Mesh, PointCloud, or Lines object to model
add_meshes(mesh: vivid3d.BaseMesh)
    Add Meshes, PointCloud's, or Lines objects to model
export(output_file='', file_type='glb')
    Default exporter supporting most common 3D file types. For full list of supported file formats see: https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types
    Returns vivid3d.BlobData if output_file is empty.
export_to_obj(output_file: str, with_texture=True)
    Backup exporter if the main export() function fails.
show(height=600)
    Render and view the mesh in a viewer window.

    )mydelimiter")
    .def(py::init<> (), "Empty constructor for model")
    .def(py::init<vector<CModelComponent>& >(),
         "constructor for CModel, from meshes, lines, or point clouds",
         py::arg("meshes"))
    .def(py::init<CModelComponent& >(),
         "constructor for CModel, from mesh, line, or point cloud",
         py::arg("mesh"))
    .def("add_meshes", &CModel::AddMeshes, R"mydelimiter(
Add Meshes, PointClouds, or Lines objects to model

Parameters
----------
meshes : array_like[vivid3d.BaseMesh]
    List of vivid3d.Mesh, vivid3d.PointCloud, or vivid3d.Lines

        )mydelimiter", py::arg("meshes"))
    .def("add_mesh", &CModel::AddMesh, R"mydelimiter(
Add Mesh, PointCloud, or Lines object to model

Parameters
----------
mesh : vivid3d.BaseMesh
    Instance of vivid3d.Mesh, vivid3d.PointCloud, or vivid3d.Lines

        )mydelimiter", py::arg("mesh"))
    .def_property_readonly("meshes", &CModel::GetMeshes,
         "Returns the BaseMeshes held")
    .def_property_readonly("n_meshes", &CModel::GetNumMeshes,
        "Number of BaseMeshes held")
    .def_property_readonly("n_polygons", &CModel::GetNumMeshes,
        "Total number of polygons in all Meshes")
    .def_property_readonly("n_vertices", &CModel::GetNumMeshes,
        "Total number of vertices in all Meshes")
    .def("export_to_obj", &CModel::ExportToObj, R"mydelimiter(
Writes Model to output_file with in standard OBJ format.
Backup exporter if the main export() function fails.

Parameters
----------
output_file : str, default: ""

File Directory to export to


        )mydelimiter", py::arg("output_file"), py::arg("with_texture") = 1)
    .def("export", &CModel::Export, R"mydelimiter(
    Exports Model to output_file with given file_type format. For full list of supported file formats, see: https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types

    Parameters
    -------------
    output_file : str, default: "Vivid3dModel"
        File Directory to export to
    file_type : str, default: 'glb'
        File format to export to

        )mydelimiter",
        py::arg("output_file")="Vivid3dModel", py::arg("file_type") = "glb")
    .def("export_to_blob", &CModel::ExportToBlob, R"mydelimiter(
        Exports Model to Blob with given file_type format. For full list of supported file formats, see: https://vivid.readthedocs.io/en/latest/userguide/basics.html#supported-file-types

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
    .def("show", [](CModel &arSelf, int aHeight) {
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
    .def("__str__", [](const CModel &arSelf){ return "vivid3d.Model\nn_meshes: " + to_string(arSelf.GetNumMeshes()) + "\nn_polygons: " + to_string(arSelf.GetNumPolygons()) + "\nn_vertices: " + to_string(arSelf.GetNumVertices()); });
}
