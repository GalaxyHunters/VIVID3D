cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)


project(vivid
        VERSION "0.9.9")


set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR})
set(PROJECT_LIB_DIR  ${PROJECT_SOURCE_DIR}/lib)
set(VIVID_SRC_DIR  ${PROJECT_SOURCE_DIR}/src)
message("${PROJECT_SOURCE_DIR}")


include(GNUInstallDirs)



add_subdirectory(${PROJECT_LIB_DIR}/SurfacingAlgorithms/huji-rich-Elad3DFast)


set(ASSIMP_INSTALL OFF CACHE INTERNAL  "")
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL  "")
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE INTERNAL  "")
set(ASSIMP_BUILD_ZLIB ON CACHE INTERNAL  "")
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL  "")
set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE INTERNAL "")
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE INTERNAL "")
set(ASSIMP_INSTALL_PDB OFF CACHE INTERNAL "")

set(ASSIMP_DIR ${PROJECT_LIB_DIR}/ImportExport/assimp)

add_subdirectory(${ASSIMP_DIR})
include_directories(${ASSIMP_DIR}/include)


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
target_link_libraries(vivid assimp::assimp)


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

    add_subdirectory(pybind11)
    include_directories(pybind11/include)
    set(PYBIND11_FINDPYTHON ON)
    add_definitions(-DPYTHON)

    include_directories(${PYTHON_INCLUDE_DIRS})
    target_link_libraries(vivid ${PYTHON_LIBRARIES})

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
else()
    add_executable(vivid_main
            #main
            ${VIVID_SRC_DIR}/main.cpp)
    target_link_libraries(vivid_main vivid)
endif()
