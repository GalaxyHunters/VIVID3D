cmake_minimum_required(VERSION 3.14)
project(Vivid_Windows)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(huji-rich-Elad3DFast)

INCLUDE_DIRECTORIES("C:/Users/nafta/source/boost_1_70_0")
INCLUDE_DIRECTORIES("huji-rich-Elad3DFast/source/3D/GeometryCommon")
set(BOOST_ROOT "C:/Users/nafta/source/boost_1_70_0")


add_executable(Vivid_Windows main.cpp Surf.cpp Mesh.cpp Model.cpp External.cpp MeshDecimation/mdMeshDecimator.cpp)

find_library(HUJI_RICH Huji_rich lib)
target_link_libraries(Vivid_Windows LINK_PUBLIC $(HUJI_RICH))