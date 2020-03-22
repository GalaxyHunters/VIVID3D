cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 14)

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

project(vivid)

set(BOOST_ROOT "C:/Boost/include")
set(FBX_ROOT   "C:/Program Files/Autodesk/FBX/FBX SDK/2019.0")
#set(PY_ROOT    "C:/Users/Tussbaum/.conda/envs/vivid27_test")
set(PY_ROOT    "C:/Users/Tussbaum/.conda/envs/py27")
#set(PY_ROOT    "C:/ProgramDatas/Miniconda3")


find_package(Boost 1.62.0 REQUIRED COMPONENTS)
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#set(Boost_DEBUG 0)

set(FBX_INCLUDE_DIR ${FBX_ROOT}/include)
set(FBX_LINK_LIBS ${FBX_ROOT}/lib/vs2015/x64/release/libfbxsdk-md.lib)

set(PY_INCLUDE_DIR ${PY_ROOT}/include)
set(PY_LINK_LIBS   ${PY_ROOT}/libs)

message(${FBX_INCLUDE_DIR})
message(${PY_ROOT})

set(PYTHONLIBS_FOUND TRUE)
add_subdirectory(External_pkgs/pybind11)

include_directories(src)
include_directories(External_pkgs/MeshDecimation)
include_directories(External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon)
include_directories(External_pkgs/huji-rich-Elad3DFast/source/misc)
include_directories(External_pkgs/pybind11/include)
include_directories(External_pkgs/lodepng)
#include_directories( ../../../../../../../Program_Files/Autodesk/FBX/FBX_SDK/2019.0/include)

include_directories(${FBX_INCLUDE_DIR})
include_directories(${PY_INCLUDE_DIR})

link_directories(${PY_LINK_LIBS})

add_executable(vivid
        #elad3d
        #source/3D/GeometryCommon
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Voronoi3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Delaunay3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Face.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Intersections.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/predicates.cxx
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Predicates3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/RoundGrid3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tessellation3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/tetgen.cxx
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tetrahedron.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Vector3D.cpp
        #source/3D/r3d
        External_pkgs/huji-rich-Elad3DFast/source/3D/r3d/Intersection3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/r3d/r3d.c
        #source/misc
        External_pkgs/huji-rich-Elad3DFast/source/misc/bisection.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/func_1_var.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/horner.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/int2str.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/io3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/mesh_generator.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/mesh_generator3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/serializable.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/simple_io.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/universal_error.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/utils.cpp
        #extra
        External_pkgs/huji-rich-Elad3DFast/source/tessellation/geometry.cpp
        #vivid
        src/ModelBuilder/Surf.cpp src/ModelBuilder/Mesh.cpp src/ModelBuilder/Model.cpp src/Utils/External.cpp
        #animation
        src/AnimationBuilder/Animation.cpp
        src/Utils/Shapes.cpp
        src/AnimationBuilder/FbxFunc.cpp
#            "../../../../../../../Program Files/Autodesk/FBX/FBX SDK/2019.0/include/fbxsdk.h"
        #external
        External_pkgs/MeshDecimation/mdMeshDecimator.cpp
        External_pkgs/lodepng/lodepng.cpp
        #main
        src/main.cpp
)


target_link_libraries(vivid Boost::boost)
target_link_libraries(vivid ${FBX_LINK_LIBS})

pybind11_add_module(vivid_py
        #elad3d
        #source/3D/GeometryCommon
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Voronoi3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Delaunay3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Face.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Intersections.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/predicates.cxx
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Predicates3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/RoundGrid3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tessellation3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/tetgen.cxx
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tetrahedron.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Vector3D.cpp
        #source/3D/r3d
        External_pkgs/huji-rich-Elad3DFast/source/3D/r3d/Intersection3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/3D/r3d/r3d.c
        #source/misc
        External_pkgs/huji-rich-Elad3DFast/source/misc/bisection.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/func_1_var.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/horner.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/int2str.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/io3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/mesh_generator.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/mesh_generator3D.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/serializable.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/simple_io.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/universal_error.cpp
        External_pkgs/huji-rich-Elad3DFast/source/misc/utils.cpp
        #extra
        External_pkgs/huji-rich-Elad3DFast/source/tessellation/geometry.cpp
        #vivid
        src/ModelBuilder/Surf.cpp src/ModelBuilder/Mesh.cpp src/ModelBuilder/Model.cpp src/Utils/External.cpp
        #external
        External_pkgs/MeshDecimation/mdMeshDecimator.cpp External_pkgs/lodepng/lodepng.cpp
        #animation
        src/AnimationBuilder/Animation.cpp src/Utils/Shapes.cpp src/AnimationBuilder/FbxFunc.cpp
        #wrapper
        src/wrapper.cpp
        )

target_link_libraries(vivid_py PRIVATE  ${FBX_LINK_LIBS} Boost::boost)


