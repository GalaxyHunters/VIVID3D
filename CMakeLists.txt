project(vivid)

cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR})

message(${PROJECT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/SurfacingAlgorithms/huji-rich-Elad3DFast; ${CMAKE_CURRENT_SOURCE_DIR}/lib/ImportAndExport/FBX;)

# TODO: Choose between Linux\Windows\mac
# TODO: Choose between py3\py27
# TODO: Choose between 32\64
# TODO: debug vs release ( different FBX path)

# TODO: Compile Elads code as a lib, and use it as such (or as subdirectory? )

# Read more at: https://github.com/ttroy50/cmake-examples

execute_process(
        COMMAND bash -c "uname -m"
        OUTPUT_VARIABLE TGT_MACHINEpy
)

#set(Boost_INCLUDE_DIR "/home/naf/projects/boost_1_80_0")
find_package( Boost 1.58.0 REQUIRED)
# TODO =============================================================> IF SOME PACKAGE IS NOT FOUND, WRITE A MESSAGE TO INSTALL THE LIB (AND VERSIONS? )

# FindPackage voronoi creates a new library called VoronoiLib from present local cpp files.
find_package(VORONOI REQUIRED)
#find_package(FBX     REQUIRED)
#find_package(IMPORTANDEXPORT REQUIRED)

#set(PY_ROOT    "C:/Users/Tussbaum/.conda/envs/vivid27_test")
#set(PY_ROOT    "C:/Users/nafta/Miniconda2")
#set(PY_ROOT    "C:/Users/zorik/Miniconda2")


#set(PY_ROOT    "C:/Users/zorik/Miniconda2")
#set(PY_ROOT    "C:/Users/Tussbaum/.conda/envs/py27")
#find_package(PythonLibsNew)
#set(PY_INCLUDE_DIR ${PY_ROOT}/include)
#set(PY_LINK_LIBS   ${PY_ROOT}/libs)
#message(${PY_ROOT})
#

#set(PYBIND11_PYTHON_VERSION 2.7)
#set(bindings_python_version 2.7 REQUIRED)
#add_subdirectory(pybind11 bindings_python_version)
#message(${PYTHON_LIBRARIES})
#set (DPYBIND11_PYTHON_VERSION 3.8.5)

#find_package(PythonLibs 3.9 REQUIRED)
#find_package(Python 3.9 COMPONENTS Interpreter Development REQUIRED)

add_subdirectory(pybind11)
add_subdirectory(lib/assimp)
set(PYBIND11_FINDPYTHON ON)
set(PYBIND11_PYTHON_VERSION 3.9)



include_directories(src) #TODO: (TOMER) I prefer to include headers by name
include_directories(src/ModelBuilder)
include_directories(src/Utils)
include_directories(lib)
include_directories(lib/MeshDecimation)
include_directories(lib/SurfacingAlgorithms)
include_directories(lib/pybind11/include)
include_directories(lib/ImportAndExport)
include_directories(lib/ImportAndExport/lodepng)


add_executable(vivid
        #external
        lib/Decimate.cpp
        lib/SurfacingAlgorithms/Voronoi.cpp
        lib/MeshDecimation/mdMeshDecimator.cpp
        lib/ImportAndExport/DataToImage.cpp
        lib/ImportAndExport/lodepng/lodepng.cpp
        lib/ImportAndExport/ObjImportExport.cpp
        lib/ImportAndExport/AssimpImportExport.cpp
        #lib/ImportAndExport/FbxCode.cpp
        #lib/ImportAndExport/FBXImportExport.cpp
        #vivid
        src/ModelBuilder/Model.cpp
        src/ModelBuilder/ModelComponent.cpp
        src/ModelBuilder/Mesh.cpp
        src/ModelBuilder/Line.cpp
        src/ModelBuilder/PointCloud.cpp

        src/ModelBuilder/Surface.cpp
        src/ModelBuilder/VoronoiSurface.cpp
        src/ModelBuilder/MarchingCubesSurface.cpp

        src/ModelBuilder/Shapes.cpp

        src/ModelBuilder/Animation.cpp
        src/ModelBuilder/StopMotionAnimation.cpp

        src/Extra/Analysis.cpp
        src/ModelBuilder/SurfByFunc.cpp
        src/ModelBuilder/SurfByFunc.h
        src/Utils/ProgressBar.h
        src/Utils/ColorDefs.h
        src/Utils/Errors.h
        src/Utils/Plasma.h
        src/Wrapping/TypeCasting.h
        src/Extra/VoronoiSmoothing.cpp

        #main
        src/main.cpp)



# We use VoronoiLib and not Voronoi::voronoi because voronoi is locally created.
target_link_libraries(vivid VoronoiLib)
target_link_libraries(vivid assimp)
target_include_directories(vivid PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(vivid Boost::boost)
#target_link_libraries(vivid ${FBX_LIBRARY_SHARED_DEBUG} xml2) # TODO Y FBX::fbx?

include_directories(lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon)
include_directories(lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/)

project(vivid_py)

pybind11_add_module(vivid_py
        #voronoi
        # source/3D/GeometryCommon
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Voronoi3D.cpp
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Delaunay3D.cpp
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Face.cpp
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Intersections.cpp
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tessellation3D.cpp
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tetrahedron.cpp
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Vector3D.cpp

        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D.cpp
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Predicates3D.cpp
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp
        #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/predicates.cxx
        #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/RoundGrid3D.cpp

        # source/3D/r3d
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/serializable.cpp
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/universal_error.cpp

        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/int2str.cpp
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/io3D.cpp
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/simple_io.cpp
        lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/utils.cpp
        #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/r3d/Intersection3D.cpp
        #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/r3d/r3d.c
        #                #source/misc
        #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/bisection.cpp
        #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/func_1_var.cpp
        #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/horner.cpp
        #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/mesh_generator.cpp
        #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/mesh_generator3D.cpp
        #                #extra
        #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/tessellation/geometry.cpp
        #external
        lib/Decimate.cpp
        lib/SurfacingAlgorithms/Voronoi.cpp
        lib/MeshDecimation/mdMeshDecimator.cpp
        lib/ImportAndExport/DataToImage.cpp
        lib/ImportAndExport/lodepng/lodepng.cpp
        lib/ImportAndExport/ObjImportExport.cpp
        #lib/ImportAndExport/FbxCode.cpp
        #vivid
        src/ModelBuilder/Surface.cpp
        src/ModelBuilder/Mesh.cpp
        src/ModelBuilder/Line.cpp
        src/ModelBuilder/PointCloud.cpp
        src/ModelBuilder/Model.cpp
        src/ModelBuilder/Shapes.cpp
        src/ModelBuilder/Animation.cpp
        src/Extra/Analysis.h
        #lib/ImportAndExport/FBXImportExport.cpp

        src/ModelBuilder/SurfByFunc.cpp
        src/ModelBuilder/SurfByFunc.h
        src/Utils/ProgressBar.h
        src/Utils/ColorDefs.h
        src/Utils/Errors.h
        src/Utils/Plasma.h
        src/Extra/VoronoiSmoothing.cpp

        #wrapper
        src/Wrapping/wrapper.cpp
        )

target_link_libraries(vivid_py PRIVATE assimp)
target_link_libraries(vivid_py PRIVATE Boost::boost)
#target_link_libraries(vivid_py PRIVATE FBX::fbx)
