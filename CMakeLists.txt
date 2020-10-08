cmake_minimum_required(VERSION 3.4)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Libs/FBX;${CMAKE_MODULE_PATH}")


set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" -l xml2)

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
#C:/Users/nafta/source/boost_1_74_0
set(BOOST_ROOT "C:/Users/user/Downloads/boost_1_62_0")         #TODO tidy this up, should not have to specify by hand
#set(Boost_DEBUG 1)
find_package(Boost 1.62.0 REQUIRED)
#find_package(LibXml2 REQUIRED)
#message(LibXml Found = ${LibXml2_FOUND})
#message(LibXml2_LIBRARY=${LIBXML2_LIBRARY})
#set(xml2_lib "/home/zohar/Documents/Vivid/Libs/libxml2/libxml2.so")


#include_directories(${LIBXML2_INCLUDE_DIR})

#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#set(Boost_DEBUG 0)




# Optional: set FBX lib path by a specific root
#set(FBX_ROOT   "C:/Program_Files/Autodesk/FBX/FBX_SDK/2020.1.1/include")
find_package(FBX)
message(FBX_FOUND = ${FBX_FOUND})
message(${FBX_LIBRARY_STATIC_RELEASE})
message(${FBX_LIBRARY_STATIC_DEBUG})

#set(PY_ROOT    "C:/Users/Tussbaum/.conda/envs/vivid27_test")
#set(PY_ROOT    "C:/Users/nafta/Miniconda2")
set(PY_ROOT    "C:/Users/user/miniconda2/envs/vivid27_32")

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
#set(PY_ROOT "C:/Users/nafta/Miniconda2")
add_subdirectory(pybind11)

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
target_link_libraries(vivid FBX::fbx)
#target_link_libraries(vivid ${LIBXML2_LIBRARY})
#target_link_libraries(vivid ${FBX_LIBRARY_SHARED_DEBUG} xml2)

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
        src/AnimationBuilder/Animation.cpp src/AnimationBuilder/FbxFunc.cpp
        #shapes
        src/Utils/Shapes.cpp
        #wrapper
        src/wrapper.cpp
        )

#message(${LIBXML2_LIBRARIES})
#add_library(xml2 STATIC ${LIBXML2_LIBRARIES})
target_link_libraries(vivid_py PRIVATE Boost::boost)
#add_library(fbx ${FBX_LIBRARY_STATIC_DEBUG})
#set_target_properties()
#target_link_libraries(vivid_py PRIVATE ${FBX_LIBRARY_STATIC_DEBUG})
target_link_libraries(vivid_py PRIVATE FBX::fbx)
#target_link_libraries(vivid_py PRIVATE ${LibXml2})





#things already tried:
#adding xml2 to the target_link_libraries() line
#adding INTERFACE_LINK_LIBRARIES ${LIBXML2_LIBRARY}