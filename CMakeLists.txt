cmake_minimum_required(VERSION 3.4)

# TODO MAKE SURE YOU KNOW THE MEANING:
execute_process(
        COMMAND bash -c "uname -m"
        OUTPUT_VARIABLE TGT_MACHINE
)

project(vivid)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(External_pkgs/pybind11)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ROOT "~/boost_1_70_0")
find_package(Boost)

message(Boost_FOUND)

if(Boost_FOUND)
    include_directories(src)
    include_directories(External_pkgs/MeshDecimation)
    include_directories(External_pkgs/huji-rich-Elad3DFast/source/3D/GeometryCommon)
    include_directories(External_pkgs/huji-rich-Elad3DFast/source/misc)
    include_directories(External_pkgs/pybind11/include)
    include_directories(C:/Users/nafta/Miniconda2/pkgs/python-2.7.16-hcb6e200_0/include)

    link_directories(C:/Users/nafta/Miniconda2/pkgs/python-2.7.16-hcb6e200_0/libs)
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
            src/Surf.cpp src/Mesh.cpp src/Model.cpp src/External.cpp External_pkgs/MeshDecimation/mdMeshDecimator.cpp
            #main
            src/main.cpp
            )

    target_link_libraries(vivid Boost::boost)

    pybind11_add_module(Vivid_py
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
            src/Surf.cpp src/Mesh.cpp src/Model.cpp src/External.cpp External_pkgs/MeshDecimation/mdMeshDecimator.cpp
            #wrapper
            src/wrapper.cpp
            )
endif()


