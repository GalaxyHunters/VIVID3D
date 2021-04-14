project(vivid)
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR})

message(${PROJECT_SOURCE_DIR})

cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 14)


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/FBX;${CMAKE_MODULE_PATH}")

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


#set(BOOST_ROOT "D:/Documents/Alpha/Libraries/boost_1_72_0")         #TODO tidy this up, should not have to specify by hand
#set(Boost_DEBUG 1)
find_package(Boost 1.62.0 REQUIRED)
#find_package(LibXml2 REQUIRED)
#message(LibXml Found = ${LibXml2_FOUND})
#message(LibXml2_LIBRARY=${LIBXML2_LIBRARY})
#set(xml2_lib "/home/zohar/Documents/Vivid/lib/libxml2/libxml2.so")


#include_directories(${LIBXML2_INCLUDE_DIR})

#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#set(Boost_DEBUG 0)

# TODO =============================================================> IF SOME PACKAGE IS NOT FOUND, WRITE A MESSAGE TO INSTALL THE LIB (AND VERSIONS? )


# Optional: set FBX lib path by a specific root
#set(FBX_ROOT   "/home/zohar/Documents/FBX") # TODO if used should be FBX_DIR...
find_package(FBX)
message(FBX_FOUND = ${FBX_FOUND}) # TODO

#set(PY_ROOT    "C:/Users/Tussbaum/.conda/envs/vivid27_test")
#set(PY_ROOT    "C:/Users/nafta/Miniconda2")
#set(PY_ROOT    "C:/Users/zorik/Miniconda2")


#set(PY_ROOT    "C:/Users/zorik/Miniconda2")
set(PY_ROOT    "C:/Users/Tussbaum/.conda/envs/py27")
#find_package(PythonLibsNew)
#set(PY_INCLUDE_DIR ${PY_ROOT}/include)
#set(PY_LINK_LIBS   ${PY_ROOT}/libs)
#message(${PY_ROOT})
#

#set(PYBIND11_PYTHON_VERSION 2.7)
#set(bindings_python_version 2.7 REQUIRED)
#add_subdirectory(pybind11 bindings_python_version)
#message(${PYTHON_LIBRARIES})
#set(PY_ROOT "C:/Users/zorik/Miniconda2")
add_subdirectory(pybind11)

include_directories(src) #TODO: (TOMER) I prefer to include headers by name
include_directories(lib/MeshDecimation)
include_directories(lib/huji-rich-Elad3DFast/source/3D/GeometryCommon)
include_directories(lib/huji-rich-Elad3DFast/source/misc)
include_directories(lib/pybind11/include)
include_directories(lib/lodepng)
#include_directories(${FBX_INCLUDE_DIR})



add_executable(vivid
        #elad3d
        #source/3D/GeometryCommon
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Voronoi3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Delaunay3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Face.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Intersections.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/predicates.cxx
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Predicates3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/RoundGrid3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tessellation3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/tetgen.cxx
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tetrahedron.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Vector3D.cpp
        #source/3D/r3d
        lib/huji-rich-Elad3DFast/source/3D/r3d/Intersection3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/r3d/r3d.c
        #source/misc
        lib/huji-rich-Elad3DFast/source/misc/bisection.cpp
        lib/huji-rich-Elad3DFast/source/misc/func_1_var.cpp
        lib/huji-rich-Elad3DFast/source/misc/horner.cpp
        lib/huji-rich-Elad3DFast/source/misc/int2str.cpp
        lib/huji-rich-Elad3DFast/source/misc/io3D.cpp
        lib/huji-rich-Elad3DFast/source/misc/mesh_generator.cpp
        lib/huji-rich-Elad3DFast/source/misc/mesh_generator3D.cpp
        lib/huji-rich-Elad3DFast/source/misc/serializable.cpp
        lib/huji-rich-Elad3DFast/source/misc/simple_io.cpp
        lib/huji-rich-Elad3DFast/source/misc/universal_error.cpp
        lib/huji-rich-Elad3DFast/source/misc/utils.cpp
        #extra
        lib/huji-rich-Elad3DFast/source/tessellation/geometry.cpp
        #vivid
        src/ModelBuilder/Surf.cpp src/ModelBuilder/Mesh.cpp src/ModelBuilder/Model.cpp lib/External.cpp
        #animation
        src/AnimationBuilder/Animation.cpp
        src/Utils/Shapes.cpp
        src/AnimationBuilder/FbxFunc.cpp
        #external
        lib/MeshDecimation/mdMeshDecimator.cpp
        lib/lodepng/lodepng.cpp
        #main
        src/main.cpp
)

target_link_libraries(vivid Boost::boost)
target_link_libraries(vivid ${FBX_LIBRARY_SHARED_DEBUG} xml2)


pybind11_add_module(vivid_py
        #elad3d
        #source/3D/GeometryCommon
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Voronoi3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Delaunay3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Face.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Intersections.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/predicates.cxx
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Predicates3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/RoundGrid3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tessellation3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/tetgen.cxx
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tetrahedron.cpp
        lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Vector3D.cpp
        #source/3D/r3d
        lib/huji-rich-Elad3DFast/source/3D/r3d/Intersection3D.cpp
        lib/huji-rich-Elad3DFast/source/3D/r3d/r3d.c
        #source/misc
        lib/huji-rich-Elad3DFast/source/misc/bisection.cpp
        lib/huji-rich-Elad3DFast/source/misc/func_1_var.cpp
        lib/huji-rich-Elad3DFast/source/misc/horner.cpp
        lib/huji-rich-Elad3DFast/source/misc/int2str.cpp
        lib/huji-rich-Elad3DFast/source/misc/io3D.cpp
        lib/huji-rich-Elad3DFast/source/misc/mesh_generator.cpp
        lib/huji-rich-Elad3DFast/source/misc/mesh_generator3D.cpp
        lib/huji-rich-Elad3DFast/source/misc/serializable.cpp
        lib/huji-rich-Elad3DFast/source/misc/simple_io.cpp
        lib/huji-rich-Elad3DFast/source/misc/universal_error.cpp
        lib/huji-rich-Elad3DFast/source/misc/utils.cpp
        #extra
        lib/huji-rich-Elad3DFast/source/tessellation/geometry.cpp
        #vivid
        src/ModelBuilder/Surf.cpp src/ModelBuilder/Mesh.cpp src/ModelBuilder/Model.cpp lib/External.cpp
        #external
        lib/MeshDecimation/mdMeshDecimator.cpp lib/lodepng/lodepng.cpp
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