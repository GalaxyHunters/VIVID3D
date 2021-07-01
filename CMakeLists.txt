project(vivid)

cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 14)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR})

message(${PROJECT_SOURCE_DIR})


set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/SurfacingAlgorithms/huji-rich-Elad3DFast; ${CMAKE_CURRENT_SOURCE_DIR}/lib/ImportAndExport/FBX;)

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
find_package(VORONOI REQUIRED)
find_package(FBX     REQUIRED)
#find_package(IMPORTANDEXPORT REQUIRED)

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
include_directories(src/ModelBuilder)
include_directories(src/Utils)
include_directories(lib)
include_directories(lib/MeshDecimation)
include_directories(lib/SurfacingAlgorithms)
include_directories(lib/pybind11/include)
include_directories(lib/ImportAndExport)
include_directories(lib/ImportAndExport/lodepng)

add_executable(vivid
        #external
        lib/External.cpp
        lib/SurfacingAlgorithms/Voronoi.cpp
        lib/MeshDecimation/mdMeshDecimator.cpp
        lib/ImportAndExport/DataToImage.cpp
        lib/ImportAndExport/lodepng/lodepng.cpp
        lib/ImportAndExport/ObjExportImport.cpp
        lib/ImportAndExport/FbxExportImport.cpp
        lib/ImportAndExport/Animation.cpp
        #vivid
        src/ModelBuilder/Surface.cpp
        src/ModelBuilder/Mesh.cpp
        src/ModelBuilder/Model.cpp
        src/ModelBuilder/Shapes.cpp
        src/Extra/Analysis.h

        #main
        src/main.cpp
        )


# We use VoronoiLib and not Voronoi::voronoi because voronoi is locally created.
target_link_libraries(vivid VoronoiLib)
target_link_libraries(vivid Boost::boost)
target_link_libraries(vivid ${FBX_LIBRARY_SHARED_DEBUG} xml2) # TODO Y FBX::fbx?


pybind11_add_module(vivid_py
        #external
        lib/External.cpp
        lib/SurfacingAlgorithms/Voronoi.cpp
        lib/MeshDecimation/mdMeshDecimator.cpp
        lib/ImportAndExport/DataToImage.cpp
        lib/ImportAndExport/lodepng/lodepng.cpp
        lib/ImportAndExport/ObjExportImport.cpp
        lib/ImportAndExport/FbxExportImport.cpp
        #vivid
        src/ModelBuilder/Surface.cpp
        src/ModelBuilder/Mesh.cpp
        src/ModelBuilder/Model.cpp
        src/ModelBuilder/Shapes.cpp
        src/Extra/Analysis.h
        lib/ImportAndExport/Animation.cpp

        #wrapper
        src/wrapper.cpp
        )


target_link_libraries(vivid_py PRIVATE VoronoiLib)
target_link_libraries(vivid_py PRIVATE Boost::boost)
target_link_libraries(vivid_py PRIVATE FBX::fbx)
