

cmake_minimum_required(VERSION 3.4)

#project(VoronoiLib VERSION 1.0.0 DESCRIPTION "Library used for running the voronoi algorithm.")
#Checks if the library is already present, if so, skips this part entirely.
IF(NOT TARGET VoronoiLib)


    # Create a new LIBRARY target as a shared library, note that this creates an so file in the cmake-debug output dir.
    # Adding the source code.
    add_library(VoronoiLib SHARED
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
                )


    # Telling cmake where to find headers for the code.
    target_include_directories(VoronoiLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Libs/huji-rich-Elad3DFast/source/3D/GeometryCommon)
    target_include_directories(VoronoiLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Libs/huji-rich-Elad3DFast/source/misc)

    MESSAGE(CREATED_VORONOI)

ENDIF()