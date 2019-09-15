cmake_minimum_required(VERSION 3.4)

# TODO MAKE SURE YOU KNOW THE MEANING:
execute_process(
        COMMAND bash -c "uname -m"
        OUTPUT_VARIABLE TGT_MACHINE
)

project(vivid)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(pybind11)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ROOT "~/boost_1_70_0")
find_package(Boost)

message(Boost_FOUND)

if(Boost_FOUND)
    include_directories(src)
    include_directories(src/MeshDecimation)
    include_directories(src/huji-rich-Elad3DFast/source/3D/GeometryCommon)
    include_directories(src/huji-rich-Elad3DFast/source/misc)
    include_directories(pybind11/include)
    include_directories(C:/Users/nafta/Miniconda2/pkgs/python-2.7.16-hcb6e200_0/include)
    #include_directories(~/boost_1_70_0)

    link_directories(C:/Users/nafta/Miniconda2/pkgs/python-2.7.16-hcb6e200_0/libs)
    add_executable(vivid
            #elad3d
            #source/3D/GeometryCommon
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Voronoi3D.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Delaunay3D.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Face.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Intersections.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/predicates.cxx
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Predicates3D.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/RoundGrid3D.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tessellation3D.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/tetgen.cxx
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tetrahedron.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Vector3D.cpp
            #source/3D/r3d
            src/huji-rich-Elad3DFast/source/3D/r3d/Intersection3D.cpp
            src/huji-rich-Elad3DFast/source/3D/r3d/r3d.c
            #source/misc
            src/huji-rich-Elad3DFast/source/misc/bisection.cpp
            src/huji-rich-Elad3DFast/source/misc/func_1_var.cpp
            src/huji-rich-Elad3DFast/source/misc/horner.cpp
            src/huji-rich-Elad3DFast/source/misc/int2str.cpp
            src/huji-rich-Elad3DFast/source/misc/io3D.cpp
            src/huji-rich-Elad3DFast/source/misc/mesh_generator.cpp
            src/huji-rich-Elad3DFast/source/misc/mesh_generator3D.cpp
            src/huji-rich-Elad3DFast/source/misc/serializable.cpp
            src/huji-rich-Elad3DFast/source/misc/simple_io.cpp
            src/huji-rich-Elad3DFast/source/misc/universal_error.cpp
            src/huji-rich-Elad3DFast/source/misc/utils.cpp
            #extra
            src/huji-rich-Elad3DFast/source/tessellation/geometry.cpp
            #vivid
            src/Surf.cpp src/Mesh.cpp src/Model.cpp src/External.cpp src/MeshDecimation/mdMeshDecimator.cpp
            #main
            src/main.cpp
            )

    target_link_libraries(vivid Boost::boost)

    pybind11_add_module(Vivid_py
            #elad3d
            #source/3D/GeometryCommon
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Voronoi3D.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Delaunay3D.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Face.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Intersections.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/predicates.cxx
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Predicates3D.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/RoundGrid3D.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tessellation3D.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/tetgen.cxx
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tetrahedron.cpp
            src/huji-rich-Elad3DFast/source/3D/GeometryCommon/Vector3D.cpp
            #source/3D/r3d
            src/huji-rich-Elad3DFast/source/3D/r3d/Intersection3D.cpp
            src/huji-rich-Elad3DFast/source/3D/r3d/r3d.c
            #source/misc
            src/huji-rich-Elad3DFast/source/misc/bisection.cpp
            src/huji-rich-Elad3DFast/source/misc/func_1_var.cpp
            src/huji-rich-Elad3DFast/source/misc/horner.cpp
            src/huji-rich-Elad3DFast/source/misc/int2str.cpp
            src/huji-rich-Elad3DFast/source/misc/io3D.cpp
            src/huji-rich-Elad3DFast/source/misc/mesh_generator.cpp
            src/huji-rich-Elad3DFast/source/misc/mesh_generator3D.cpp
            src/huji-rich-Elad3DFast/source/misc/serializable.cpp
            src/huji-rich-Elad3DFast/source/misc/simple_io.cpp
            src/huji-rich-Elad3DFast/source/misc/universal_error.cpp
            src/huji-rich-Elad3DFast/source/misc/utils.cpp
            #extra
            src/huji-rich-Elad3DFast/source/tessellation/geometry.cpp
            #vivid
            src/Surf.cpp src/Mesh.cpp src/Model.cpp src/External.cpp src/MeshDecimation/mdMeshDecimator.cpp
            )
endif()


