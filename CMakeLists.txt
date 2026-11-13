# 3.15: FetchContent_MakeAvailable needs 3.14, and 3.15 is what current distro/CI CMake packages
# provide at minimum (Ubuntu 20.04: 3.16). pip builds additionally require cmake>=3.24 (pyproject.toml).
cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)


project(vivid
        VERSION "1.0.0")


set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR})
set(PROJECT_LIB_DIR  ${PROJECT_SOURCE_DIR}/lib)
set(VIVID_SRC_DIR  ${PROJECT_SOURCE_DIR}/src)
message("${PROJECT_SOURCE_DIR}")


include(GNUInstallDirs)



add_subdirectory(${PROJECT_LIB_DIR}/SurfacingAlgorithms/huji-rich-Elad3DFast)


# assimp: installed copy, fetched release, or the legacy vendored copy (see cmake/VividAssimp.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/VividAssimp.cmake)


#define used cpp packages

add_library(vivid SHARED
        #external
        "${PROJECT_LIB_DIR}/Decimate.cpp"
        "${PROJECT_LIB_DIR}/SurfacingAlgorithms/Voronoi.cpp"
        "${PROJECT_LIB_DIR}/MeshDecimation/mdMeshDecimator.cpp"
        "${VIVID_SRC_DIR}/ImportExport/ColorMapToPng.cpp"
        "${PROJECT_LIB_DIR}/ImportExport/lodepng/lodepng.cpp"
        "${VIVID_SRC_DIR}/ImportExport/ObjImportExport.cpp"
        "${VIVID_SRC_DIR}/ImportExport/AssimpImportExport.cpp"
        "${VIVID_SRC_DIR}/ImportExport/BlobData.cpp"
        #vivid
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
        "${VIVID_SRC_DIR}/ModelBuilder/Vivify.cpp"

        "${VIVID_SRC_DIR}/Textures/ColorMap.h"
        "${VIVID_SRC_DIR}/Textures/ColorDefs.cpp"
        "${VIVID_SRC_DIR}/Textures/Plasma.h"
        "${VIVID_SRC_DIR}/Textures/Material.h"

        "${VIVID_SRC_DIR}/Utils/Logging/ProgressBar.cpp"
        "${VIVID_SRC_DIR}/Utils/Logging/Errors.h"
        "${VIVID_SRC_DIR}/Utils/Logging/LogFile.cpp"

        "${VIVID_SRC_DIR}/Utils/ConstrainedNumbers.cpp"
        "${VIVID_SRC_DIR}/Utils/Noisify.cpp"

        "${VIVID_SRC_DIR}/Extra/Analysis.cpp")


target_include_directories(vivid PUBLIC
        $<BUILD_INTERFACE:${VIVID_SRC_DIR}/ImportExport>
        $<BUILD_INTERFACE:${VIVID_SRC_DIR}/ModelBuilder>
        $<BUILD_INTERFACE:${VIVID_SRC_DIR}/Utils>
        $<BUILD_INTERFACE:${VIVID_SRC_DIR}/Textures>
        $<BUILD_INTERFACE:${VIVID_SRC_DIR}/Utils/Logging>
        $<BUILD_INTERFACE:${VIVID_SRC_DIR}/Wrapping>
        $<BUILD_INTERFACE:${PROJECT_LIB_DIR}>
        $<BUILD_INTERFACE:${PROJECT_LIB_DIR}/MeshDecimation>
        $<BUILD_INTERFACE:${PROJECT_LIB_DIR}/SurfacingAlgorithms>
        $<BUILD_INTERFACE:${PROJECT_LIB_DIR}/MyBoost>
        $<BUILD_INTERFACE:${PROJECT_LIB_DIR}/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon>
        $<BUILD_INTERFACE:${PROJECT_LIB_DIR}/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc>
        $<BUILD_INTERFACE:${PROJECT_LIB_DIR}/ImportExport/lodepng>
        $<BUILD_INTERFACE:${PROJECT_LIB_DIR}/ImportExport>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/pybind11/include/pybind11>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


message("VIVID SHARED BUILD")

target_link_libraries(vivid voronoi)
if(VIVID_ASSIMP_RESOLVED STREQUAL "SYSTEM")
    target_link_libraries(vivid assimp::assimp)
else()
    # assimp is built inside this project and linked statically (FETCH). It is an
    # implementation detail, so keep it out of the exported vividTargets (install(EXPORT) would
    # otherwise fail because assimp's own install rules are disabled).
    target_link_libraries(vivid $<BUILD_INTERFACE:assimp::assimp>)
endif()


# libvivid and libvoronoi are installed side by side: let libvivid find its neighbours itself instead
# of relying on the loader inheriting the RPATH of whoever loaded it (_core).
if(APPLE)
    set_target_properties(vivid PROPERTIES INSTALL_RPATH "@loader_path")
else()
    set_target_properties(vivid PROPERTIES INSTALL_RPATH "$ORIGIN")
endif()

install(
        TARGETS vivid
        EXPORT vividTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


set(VIVID_CMAKE_PACKAGE_INSTALL_SUBDIR "lib/cmake/vivid")   #"share/vivid/cmake"

install(
        EXPORT vividTargets
        NAMESPACE vivid::
        DESTINATION ${VIVID_CMAKE_PACKAGE_INSTALL_SUBDIR})


include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        vividConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMinorVersion)

configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/vividConfig.cmake.in" vividConfig.cmake
        INSTALL_DESTINATION ${VIVID_CMAKE_PACKAGE_INSTALL_SUBDIR})

install(FILES "${PROJECT_BINARY_DIR}/vividConfig.cmake"
        "${PROJECT_BINARY_DIR}/vividConfigVersion.cmake"
        DESTINATION ${VIVID_CMAKE_PACKAGE_INSTALL_SUBDIR})


if(DEFINED SKBUILD)
    set(ignoreMe "${SKBUILD}")

    # Must be set BEFORE pybind11 is added: it selects the modern FindPython module. (It used to be set
    # afterwards, which had no effect, so pybind11 used the legacy FindPythonInterp/FindPythonLibs that
    # CMake has deprecated, policy CMP0148.)
    set(PYBIND11_FINDPYTHON ON)
    add_subdirectory(pybind11)
    include_directories(pybind11/include)

    # NOTE: libvivid itself must not depend on Python (no PYTHON define, no libpython). Only the
    # _core extension module does; it hooks Python's stdout into the library's logger at import.
    # Linking libpython into a shared library makes the wheel depend on one exact interpreter build
    # and is not allowed in manylinux wheels.
    pybind11_add_module(_core
            ${VIVID_SRC_DIR}/Wrapping/wrapper.cpp
            ${VIVID_SRC_DIR}/Wrapping/TypeCasting.h
            )


    target_link_libraries(_core PRIVATE vivid)

    # Installing the extension module to the root of the package
    install(TARGETS _core DESTINATION .)


    if(APPLE)
        set_target_properties(
                _core PROPERTIES INSTALL_RPATH "@loader_path/${CMAKE_INSTALL_LIBDIR}")
    else()
        set_target_properties(_core PROPERTIES INSTALL_RPATH
                "$ORIGIN/${CMAKE_INSTALL_LIBDIR}")
    endif()
endif()
