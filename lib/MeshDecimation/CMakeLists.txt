# Mesh simplification (third-party, self-contained: only standard headers).
# main.cpp in this directory is a stand-alone demo program and is not built.
add_library(meshdecimation STATIC mdMeshDecimator.cpp)
target_include_directories(meshdecimation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
