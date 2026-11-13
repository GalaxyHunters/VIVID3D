# The 3D Voronoi tessellation library (huji-rich "Elad3DFast") used by Vivid. Only the 3D geometry
# sources below are built; the rest of this directory is research code, tests and examples.
add_library(voronoi STATIC
        source/3D/GeometryCommon/Voronoi3D.cpp
        source/3D/GeometryCommon/Delaunay3D.cpp
        source/3D/GeometryCommon/Face.cpp
        source/3D/GeometryCommon/Intersections.cpp
        source/3D/GeometryCommon/Tessellation3D.cpp
        source/3D/GeometryCommon/Tetrahedron.cpp
        source/3D/GeometryCommon/Vector3D.cpp
        source/3D/GeometryCommon/HilbertOrder3D.cpp
        source/3D/GeometryCommon/Predicates3D.cpp
        source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp
        source/misc/serializable.cpp
        source/misc/universal_error.cpp
        source/misc/int2str.cpp
        source/misc/io3D.cpp
        source/misc/simple_io.cpp
        source/misc/utils.cpp)

# the public headers include Boost, so every user of the library needs it too
target_include_directories(voronoi PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/source/3D/GeometryCommon
        ${CMAKE_CURRENT_SOURCE_DIR}/source/misc)
target_link_libraries(voronoi PUBLIC Boost::headers)
