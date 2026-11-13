# The 3D Voronoi tessellation library (huji-rich "Elad3DFast") used by Vivid. Only the 3D geometry
# sources below are built; the rest of this directory is research code, tests and examples.
cmake_minimum_required(VERSION 3.15)

include(GNUInstallDirs)

add_library(voronoi SHARED
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
        ${CMAKE_CURRENT_SOURCE_DIR}/source/misc/utils.cpp)

target_include_directories(voronoi
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon
            ${CMAKE_CURRENT_SOURCE_DIR}/source/misc
        PRIVATE
            ${PROJECT_SOURCE_DIR}/lib/MyBoost)

install(TARGETS voronoi LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
