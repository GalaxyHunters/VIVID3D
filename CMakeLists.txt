# Vivid: the C++ core library and the `_core` Python extension module.
#
# Normally built through pip (scikit-build-core, see pyproject.toml). Direct use:
#   cmake -S . -B build                            # core libraries + Python extension (needs pybind11)
#   cmake -S . -B build -DVIVID_BUILD_PYTHON=OFF   # C++ libraries only
#
# 3.15: FetchContent_MakeAvailable needs 3.14, and 3.15 is what current distro/CI CMake packages
# provide at minimum (Ubuntu 20.04: 3.16). pip builds get a recent CMake through scikit-build-core.
cmake_minimum_required(VERSION 3.15)

# The version has a single source of truth: src/vivid3d/_details.py (pyproject.toml reads it too).
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/src/vivid3d/_details.py" _vivid_version_line REGEX "^__version__ *=")
string(REGEX MATCH "\"([0-9]+\\.[0-9]+\\.[0-9]+)\"" _vivid_version_match "${_vivid_version_line}")
if(NOT _vivid_version_match)
    message(FATAL_ERROR "Cannot read __version__ (\"X.Y.Z\") from src/vivid3d/_details.py")
endif()
project(vivid VERSION ${CMAKE_MATCH_1} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# A hand-run `cmake` without a build type compiles with no optimisation at all, which is very slow for a
# numerical library. (pip builds already pass Release.)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(VIVID_BUILD_PYTHON "Build the _core Python extension module" ON)

set(PROJECT_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(VIVID_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

include(GNUInstallDirs)

# --- Dependencies -------------------------------------------------------------------------------
add_subdirectory(${PROJECT_LIB_DIR}/SurfacingAlgorithms/huji-rich-Elad3DFast)   # target: voronoi
# assimp: installed copy or a pinned release download, see cmake/VividAssimp.cmake (target: assimp::assimp)
include(${PROJECT_SOURCE_DIR}/cmake/VividAssimp.cmake)

# --- The Vivid core library ---------------------------------------------------------------------
add_library(vivid SHARED
        # model building
        "${VIVID_SRC_DIR}/ModelBuilder/Vivify.cpp"
        "${VIVID_SRC_DIR}/ModelBuilder/Model.cpp"
        "${VIVID_SRC_DIR}/ModelBuilder/ModelComponent.cpp"
        "${VIVID_SRC_DIR}/ModelBuilder/Mesh.cpp"
        "${VIVID_SRC_DIR}/ModelBuilder/Line.cpp"
        "${VIVID_SRC_DIR}/ModelBuilder/PointCloud.cpp"
        "${VIVID_SRC_DIR}/ModelBuilder/VoronoiVolume.cpp"
        "${VIVID_SRC_DIR}/ModelBuilder/MarchingCubesSurface.cpp"
        "${VIVID_SRC_DIR}/ModelBuilder/Shapes.cpp"
        "${VIVID_SRC_DIR}/ModelBuilder/Animation.cpp"
        "${VIVID_SRC_DIR}/ModelBuilder/StopMotionAnimation.cpp"
        "${VIVID_SRC_DIR}/ModelBuilder/SurfByFunc.cpp"

        # import / export
        "${VIVID_SRC_DIR}/ImportExport/AssimpImportExport.cpp"
        "${VIVID_SRC_DIR}/ImportExport/ObjImportExport.cpp"
        "${VIVID_SRC_DIR}/ImportExport/BlobData.cpp"
        "${VIVID_SRC_DIR}/ImportExport/ColorMapToPng.cpp"

        # textures, utilities, analysis
        "${VIVID_SRC_DIR}/Textures/ColorDefs.cpp"
        "${VIVID_SRC_DIR}/Utils/Logging/ProgressBar.cpp"
        "${VIVID_SRC_DIR}/Utils/Logging/LogFile.cpp"
        "${VIVID_SRC_DIR}/Utils/ConstrainedNumbers.cpp"
        "${VIVID_SRC_DIR}/Utils/Noisify.cpp"
        "${VIVID_SRC_DIR}/Extra/Analysis.cpp"

        # third-party code compiled directly into the library
        "${PROJECT_LIB_DIR}/Decimate.cpp"
        "${PROJECT_LIB_DIR}/SurfacingAlgorithms/Voronoi.cpp"
        "${PROJECT_LIB_DIR}/MeshDecimation/mdMeshDecimator.cpp"
        "${PROJECT_LIB_DIR}/ImportExport/lodepng/lodepng.cpp")

# The sources include each other by bare file name, so these directories are on the include path
# of the library and of everything that links it (the Python module).
target_include_directories(vivid PUBLIC
        ${VIVID_SRC_DIR}/ImportExport
        ${VIVID_SRC_DIR}/ModelBuilder
        ${VIVID_SRC_DIR}/Utils
        ${VIVID_SRC_DIR}/Textures
        ${VIVID_SRC_DIR}/Utils/Logging
        ${VIVID_SRC_DIR}/Wrapping
        ${PROJECT_LIB_DIR}
        ${PROJECT_LIB_DIR}/MeshDecimation
        ${PROJECT_LIB_DIR}/SurfacingAlgorithms
        ${PROJECT_LIB_DIR}/MyBoost
        ${PROJECT_LIB_DIR}/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon
        ${PROJECT_LIB_DIR}/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc
        ${PROJECT_LIB_DIR}/ImportExport/lodepng
        ${PROJECT_LIB_DIR}/ImportExport)

# assimp is linked dynamically when an installed copy is used, statically when it was fetched
target_link_libraries(vivid voronoi assimp::assimp)

# libvivid and libvoronoi are installed side by side: let libvivid find its neighbours itself instead
# of relying on the loader inheriting the RPATH of whoever loaded it (_core).
if(APPLE)
    set_target_properties(vivid PROPERTIES INSTALL_RPATH "@loader_path")
else()
    set_target_properties(vivid PROPERTIES INSTALL_RPATH "$ORIGIN")
endif()

install(TARGETS vivid LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# --- The Python extension module ----------------------------------------------------------------
if(VIVID_BUILD_PYTHON)
    # pybind11 is an external build dependency (pyproject.toml: build-system.requires). Ask the
    # interpreter that is building where its pybind11 lives, so this works with any build frontend.
    set(PYBIND11_FINDPYTHON ON)
    find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
    execute_process(
        COMMAND "${Python_EXECUTABLE}" -m pybind11 --cmakedir
        OUTPUT_VARIABLE _vivid_pybind11_dir OUTPUT_STRIP_TRAILING_WHITESPACE RESULT_VARIABLE _vivid_pybind11_rc)
    if(_vivid_pybind11_rc EQUAL 0)
        list(APPEND CMAKE_PREFIX_PATH "${_vivid_pybind11_dir}")
    endif()
    set(VIVID_PYBIND11_MIN_VERSION "3.0.1" CACHE STRING "Oldest pybind11 accepted (tested with tools/golden)")
    find_package(pybind11 ${VIVID_PYBIND11_MIN_VERSION} CONFIG REQUIRED)
    message(STATUS "pybind11: ${pybind11_VERSION} (${pybind11_DIR})")

    # NOTE: libvivid itself must not depend on Python (no libpython). Only the _core extension does;
    # it hooks Python's stdout into the library's logger at import (see src/Wrapping/wrapper.cpp).
    pybind11_add_module(_core ${VIVID_SRC_DIR}/Wrapping/wrapper.cpp)
    target_link_libraries(_core PRIVATE vivid)

    install(TARGETS _core DESTINATION .)
    if(APPLE)
        set_target_properties(_core PROPERTIES INSTALL_RPATH "@loader_path/${CMAKE_INSTALL_LIBDIR}")
    else()
        set_target_properties(_core PROPERTIES INSTALL_RPATH "$ORIGIN/${CMAKE_INSTALL_LIBDIR}")
    endif()
endif()
