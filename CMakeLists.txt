project(vivid)
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR})

message(${PROJECT_SOURCE_DIR})

cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 14)


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/FBX;${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/lib/huji-rich-Elad3DFast")

# TODO: Choose between Linux\Windows\mac
# TODO: Choose between py3\py27
# TODO: Choose between 32\64
# TODO: debug vs release ( different FBX path)

# TODO: Compile Elads code as a lib, and use it as such (or as subdirectory? )

# Read more at: https://github.com/ttroy50/cmake-examples

execute_process(
        COMMAND bash -c "uname -m"
        OUTPUT_VARIABLE TGT_MACHINEpy
)

find_package(Boost 1.62.0 REQUIRED)

# TODO =============================================================> IF SOME PACKAGE IS NOT FOUND, WRITE A MESSAGE TO INSTALL THE LIB (AND VERSIONS? )

# FindPackage voronoi creates a new library called VoronoiLib from present local cpp files.
find_package(VORONOI)
find_package(FBX)

#set(PY_ROOT    "C:/Users/Tussbaum/.conda/envs/vivid27_test")
#set(PY_ROOT    "C:/Users/nafta/Miniconda2")
#set(PY_ROOT    "C:/Users/zorik/Miniconda2")


#set(PY_ROOT    "C:/Users/zorik/Miniconda2")
#set(PY_ROOT    "C:/Users/Tussbaum/.conda/envs/py27")
#find_package(PythonLibsNew)
#set(PY_INCLUDE_DIR ${PY_ROOT}/include)
#set(PY_LINK_LIBS   ${PY_ROOT}/libs)
#message(${PY_ROOT})
#

#set(PYBIND11_PYTHON_VERSION 2.7)
#set(bindings_python_version 2.7 REQUIRED)
#add_subdirectory(pybind11 bindings_python_version)
#message(${PYTHON_LIBRARIES})
#set(PY_ROOT "C:/Users/zorik/Miniconda2")
add_subdirectory(pybind11)

include_directories(src) #TODO: (TOMER) I prefer to include headers by name
include_directories(lib/MeshDecimation)
include_directories(lib/pybind11/include)
include_directories(lib/lodepng)

add_executable(vivid
        #vivid
        src/ModelBuilder/Surf.cpp src/ModelBuilder/Mesh.cpp src/ModelBuilder/Model.cpp lib/External.cpp
        #animation
        src/AnimationBuilder/Animation.cpp
        src/Utils/Shapes.cpp
        src/AnimationBuilder/FbxFunc.cpp
        #external
        lib/MeshDecimation/mdMeshDecimator.cpp
        lib/lodepng/lodepng.cpp
        #main
        src/main.cpp
        )


# We use VoronoiLib and not Voronoi::voronoi because voronoi is locally created.
target_link_libraries(vivid VoronoiLib)
target_link_libraries(vivid Boost::boost)
target_link_libraries(vivid ${FBX_LIBRARY_SHARED_DEBUG} xml2)


pybind11_add_module(vivid_py
        #vivid
        src/ModelBuilder/Surf.cpp src/ModelBuilder/Mesh.cpp src/ModelBuilder/Model.cpp lib/External.cpp
        #external
        lib/MeshDecimation/mdMeshDecimator.cpp lib/lodepng/lodepng.cpp
        #animation
        src/AnimationBuilder/Animation.cpp src/AnimationBuilder/FbxFunc.cpp
        #shapes
        src/Utils/Shapes.cpp
        #wrapper
        src/wrapper.cpp
        )


target_link_libraries(vivid_py PRIVATE VoronoiLib)
target_link_libraries(vivid_py PRIVATE Boost::boost)
target_link_libraries(vivid_py PRIVATE FBX::fbx)
