cmake_minimum_required(VERSION 3.4)

set(CMAKE_MODULE_PATH "${vivid_SOURCE_DIR}/Libs/FBX;${CMAKE_MODULE_PATH}")



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

#set(BOOST_ROOT "C:/Boost/include")
find_package(Boost 1.62.0 REQUIRED COMPONENTS)
#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#set(Boost_DEBUG 0)


find_package(FBX)
# Optional: set FBX lib path by a specific root
#set(FBX_ROOT   "C:/Program Files/Autodesk/FBX/FBX SDK/2019.0")
#set(FBX_INCLUDE_DIR ${FBX_ROOT}/include)
#set(FBX_LIBRARY ${FBX_ROOT}/lib/vs2015/x64/release/libfbxsdk-md.lib)
#set(FBX_LIBRARY_DEBUG ${FBX_ROOT}/lib/vs2015/x64/debug/libfbxsdk-md.lib)


#set(PY_ROOT    "C:/Users/Tussbaum/.conda/envs/vivid27_test")
#set(PY_ROOT    "C:/ProgramDatas/Miniconda3")

#set(PY_ROOT    "C:/Users/Tussbaum/.conda/envs/py27")
#find_package(PythonLibsNew)
#set(PY_INCLUDE_DIR ${PY_ROOT}/include)
#set(PY_LINK_LIBS   ${PY_ROOT}/libs)
#message(${PY_ROOT})

#set(PYTHONLIBS_FOUND TRUE)

set(PYBIND11_PYTHON_VERSION 2.7)
set(bindings_python_version 2.7 REQUIRED)
add_subdirectory(pybind11 bindings_python_version)
message(${PYTHON_LIBRARIES})

include_directories(src)
include_directories(Libs/MeshDecimation)
include_directories(Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon)
include_directories(Libs/huji-rich-Elad3DFast/source/misc)
include_directories(Libs/pybind11/include)
include_directories(Libs/lodepng)
#include_directories( ../../../../../../../Program_Files/Autodesk/FBX/FBX_SDK/2019.0/include)

# Not sure it is needed:
#include_directories(${PY_INCLUDE_DIR})
#link_directories(${PY_LINK_LIBS})

include_directories(${FBX_INCLUDE_DIR})


add_executable(vivid
        #elad3d
        #source/3D/GeometryCommon
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Voronoi3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Delaunay3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Face.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Intersections.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/predicates.cxx
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Predicates3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/RoundGrid3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tessellation3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/tetgen.cxx
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tetrahedron.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Vector3D.cpp
        #source/3D/r3d
        Libs/huji-rich-Elad3DFast/source/3D/r3d/Intersection3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/r3d/r3d.c
        #source/misc
        Libs/huji-rich-Elad3DFast/source/misc/bisection.cpp
        Libs/huji-rich-Elad3DFast/source/misc/func_1_var.cpp
        Libs/huji-rich-Elad3DFast/source/misc/horner.cpp
        Libs/huji-rich-Elad3DFast/source/misc/int2str.cpp
        Libs/huji-rich-Elad3DFast/source/misc/io3D.cpp
        Libs/huji-rich-Elad3DFast/source/misc/mesh_generator.cpp
        Libs/huji-rich-Elad3DFast/source/misc/mesh_generator3D.cpp
        Libs/huji-rich-Elad3DFast/source/misc/serializable.cpp
        Libs/huji-rich-Elad3DFast/source/misc/simple_io.cpp
        Libs/huji-rich-Elad3DFast/source/misc/universal_error.cpp
        Libs/huji-rich-Elad3DFast/source/misc/utils.cpp
        #extra
        Libs/huji-rich-Elad3DFast/source/tessellation/geometry.cpp
        #vivid
        src/ModelBuilder/Surf.cpp src/ModelBuilder/Mesh.cpp src/ModelBuilder/Model.cpp Libs/External.cpp
        #animation
        src/AnimationBuilder/Animation.cpp
        src/Utils/Shapes.cpp
        src/AnimationBuilder/FbxFunc.cpp
#            "../../../../../../../Program Files/Autodesk/FBX/FBX SDK/2019.0/include/fbxsdk.h"
        #external
        Libs/MeshDecimation/mdMeshDecimator.cpp
        Libs/lodepng/lodepng.cpp
        #main
        src/main.cpp
)


target_link_libraries(vivid Boost::boost)
target_link_libraries(vivid ${FBX_LIBRARY})

pybind11_add_module(vivid_py
        #elad3d
        #source/3D/GeometryCommon
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Voronoi3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Delaunay3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Face.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Intersections.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/predicates.cxx
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Predicates3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/RoundGrid3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tessellation3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/tetgen.cxx
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tetrahedron.cpp
        Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon/Vector3D.cpp
        #source/3D/r3d
        Libs/huji-rich-Elad3DFast/source/3D/r3d/Intersection3D.cpp
        Libs/huji-rich-Elad3DFast/source/3D/r3d/r3d.c
        #source/misc
        Libs/huji-rich-Elad3DFast/source/misc/bisection.cpp
        Libs/huji-rich-Elad3DFast/source/misc/func_1_var.cpp
        Libs/huji-rich-Elad3DFast/source/misc/horner.cpp
        Libs/huji-rich-Elad3DFast/source/misc/int2str.cpp
        Libs/huji-rich-Elad3DFast/source/misc/io3D.cpp
        Libs/huji-rich-Elad3DFast/source/misc/mesh_generator.cpp
        Libs/huji-rich-Elad3DFast/source/misc/mesh_generator3D.cpp
        Libs/huji-rich-Elad3DFast/source/misc/serializable.cpp
        Libs/huji-rich-Elad3DFast/source/misc/simple_io.cpp
        Libs/huji-rich-Elad3DFast/source/misc/universal_error.cpp
        Libs/huji-rich-Elad3DFast/source/misc/utils.cpp
        #extra
        Libs/huji-rich-Elad3DFast/source/tessellation/geometry.cpp
        #vivid
        src/ModelBuilder/Surf.cpp src/ModelBuilder/Mesh.cpp src/ModelBuilder/Model.cpp Libs/External.cpp
        #external
        Libs/MeshDecimation/mdMeshDecimator.cpp Libs/lodepng/lodepng.cpp
        #animation
        src/AnimationBuilder/Animation.cpp src/Utils/Shapes.cpp src/AnimationBuilder/FbxFunc.cpp
        #wrapper
        src/wrapper.cpp
        )

target_link_libraries(vivid_py PRIVATE  ${FBX_LIBRARY} Boost::boost)


