cmake_minimum_required(VERSION 3.4)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

include(GNUInstallDirs)

#project(VoronoiLib VERSION 1.0.0 DESCRIPTION "Library used for running the voronoi algorithm.")
#Checks if the library is already present, if so, skips this part entirely.
include_directories(${PROJECT_SOURCE_DIR}/lib/MyBoost)
IF(NOT TARGET voronoi)
    # Create a new LIBRARY target as a shared library, note that this creates an so file in the cmake-debug output dir.
    # Adding the source code.
    add_library(voronoi SHARED
            # source/3D/GeometryCommon
            ${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon/Voronoi3D.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon/Delaunay3D.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon/Face.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon/Intersections.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon/Tessellation3D.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon/Tetrahedron.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon/Vector3D.cpp

            ${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon/HilbertOrder3D.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon/Predicates3D.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp

            ${CMAKE_CURRENT_SOURCE_DIR}/source/misc/serializable.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/source/misc/universal_error.cpp

            ${CMAKE_CURRENT_SOURCE_DIR}/source/misc/int2str.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/source/misc/io3D.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/source/misc/simple_io.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/source/misc/utils.cpp

#            redacted files?
#            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/predicates.cxx
#            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/RoundGrid3D.cpp
#            source/3D/r3d
#            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/r3d/Intersection3D.cpp
#            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/r3d/r3d.c
#            source/misc
#            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/bisection.cpp
#            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/func_1_var.cpp
#            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/horner.cpp
#            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/mesh_generator.cpp
#            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/mesh_generator3D.cpp
#            #extra
#            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/tessellation/geometry.cpp
            )
    # include_directories(${CMAKE_MODULE_PATH})
    target_include_directories(voronoi PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source/misc>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

    install(DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon
            ${CMAKE_CURRENT_SOURCE_DIR}/source/misc>
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h")

    install(
            TARGETS voronoi
            EXPORT voronoiTargets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

    set(VORONOI_CMAKE_PACKAGE_INSTALL_SUBDIR "lib/cmake/voronoi") #"share/voronoi/cmake"

    install(
            EXPORT voronoiTargets
            NAMESPACE voronoi::
            DESTINATION ${VORONOI_CMAKE_PACKAGE_INSTALL_SUBDIR})

    MESSAGE(CREATED_VORONOI)

ENDIF()