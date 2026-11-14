# Geometry and model building: meshes, lines, point clouds, shapes, Voronoi/marching-cubes surfaces, animation.
target_sources(vivid PRIVATE
        Vivify.cpp
        Model.cpp
        ModelComponent.cpp
        Mesh.cpp
        Line.cpp
        PointCloud.cpp
        VoronoiVolume.cpp
        MarchingCubesSurface.cpp
        Shapes.cpp
        Animation.cpp
        StopMotionAnimation.cpp
        SurfByFunc.cpp)
target_include_directories(vivid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
