
INCLUDE_DIRECTORIES("C:/Users/nafta/source/boost_1_70_0")

add_library(Huji-rich STATIC
        #source/3D/GeometryCommon
        source/3D/GeometryCommon/Voronoi3D.cpp source/3D/GeometryCommon/Delaunay3D.cpp source/3D/GeometryCommon/Face.cpp source/3D/GeometryCommon/HilbertOrder3D.cpp source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp
        source/3D/GeometryCommon/Intersections.cpp source/3D/GeometryCommon/predicates.cxx source/3D/GeometryCommon/Predicates3D.cpp source/3D/GeometryCommon/RoundGrid3D.cpp source/3D/GeometryCommon/Tessellation3D.cpp
        source/3D/GeometryCommon/tetgen.cxx source/3D/GeometryCommon/Tetrahedron.cpp source/3D/GeometryCommon/Vector3D.cpp
        #source/3D/r3d
        source/3D/r3d/Intersection3D.cpp source/3D/r3d/r3d.c
        #source/misc
        source/misc/bisection.cpp source/misc/func_1_var.cpp source/misc/horner.cpp source/misc/int2str.cpp source/misc/io3D.cpp source/misc/mesh_generator.cpp source/misc/mesh_generator3D.cpp
        source/misc/serializable.cpp source/misc/simple_io.cpp source/misc/universal_error.cpp source/misc/utils.cpp
        )