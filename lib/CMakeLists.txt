# Third-party and independent code. Each subdirectory defines its own target; none of them knows
# about Vivid. The two glue files that connect them to Vivid's point/face types are compiled into
# the `vivid` target (defined in the top-level CMakeLists.txt).
add_subdirectory(MyBoost)                                    # Boost::headers
add_subdirectory(ImportExport/lodepng)                       # lodepng
add_subdirectory(MeshDecimation)                             # meshdecimation
add_subdirectory(SurfacingAlgorithms/huji-rich-Elad3DFast)   # voronoi

# glue: Decimate.* adapts meshdecimation, Voronoi.* adapts voronoi, to Vivid's Point and Face types
target_sources(vivid PRIVATE
        Decimate.cpp
        SurfacingAlgorithms/Voronoi.cpp)
target_include_directories(vivid PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/SurfacingAlgorithms)
