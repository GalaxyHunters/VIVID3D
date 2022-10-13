cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)


project(vivid
        VERSION "0.2.4")


set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR})
message("${PROJECT_SOURCE_DIR}")


include(GNUInstallDirs)



#find_package( Boost 1.58.0 REQUIRED)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/SurfacingAlgorithms/huji-rich-Elad3DFast)


set(ASSIMP_INSTALL OFF CACHE INTERNAL  "")
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL  "")
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE INTERNAL  "")
set(ASSIMP_BUILD_ZLIB ON CACHE INTERNAL  "")
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL  "")

set(ASSIMP_DIR ${PROJECT_SOURCE_DIR}/lib/assimp)

add_subdirectory(${ASSIMP_DIR})
include_directories(${ASSIMP_DIR}/include)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/lib/assimp/cmake-modules)
#find_package(assimp COMPONENTS assimp REQUIRED)

#define used cpp packages

add_library(vivid SHARED
        #external
        "${PROJECT_SOURCE_DIR}/lib/Decimate.cpp"
        "${PROJECT_SOURCE_DIR}/lib/SurfacingAlgorithms/Voronoi.cpp"
        "${PROJECT_SOURCE_DIR}/lib/MeshDecimation/mdMeshDecimator.cpp"
        "${PROJECT_SOURCE_DIR}/lib/ImportAndExport/DataToImage.cpp"
        "${PROJECT_SOURCE_DIR}/lib/ImportAndExport/lodepng/lodepng.cpp"
        "${PROJECT_SOURCE_DIR}/lib/ImportAndExport/ObjImportExport.cpp"
        "${PROJECT_SOURCE_DIR}/lib/ImportAndExport/AssimpImportExport.cpp"
        #lib/ImportAndExport/FbxCode.cpp
        #lib/ImportAndExport/FBXImportExport.cpp
        #vivid
        "${PROJECT_SOURCE_DIR}/src/ModelBuilder/Vivify.cpp"
        "${PROJECT_SOURCE_DIR}/src/ModelBuilder/Model.cpp"
        "${PROJECT_SOURCE_DIR}/src/ModelBuilder/ModelComponent.cpp"
        "${PROJECT_SOURCE_DIR}/src/ModelBuilder/Mesh.cpp"
        "${PROJECT_SOURCE_DIR}/src/ModelBuilder/Line.cpp"
        "${PROJECT_SOURCE_DIR}/src/ModelBuilder/PointCloud.cpp"

        "${PROJECT_SOURCE_DIR}/src/ModelBuilder/VoronoiVolume.cpp"
        "${PROJECT_SOURCE_DIR}/src/ModelBuilder/MarchingCubesSurface.cpp"

        "${PROJECT_SOURCE_DIR}/src/ModelBuilder/Shapes.cpp"

        "${PROJECT_SOURCE_DIR}/src/ModelBuilder/Animation.cpp"
        "${PROJECT_SOURCE_DIR}/src/ModelBuilder/StopMotionAnimation.cpp"

        "${PROJECT_SOURCE_DIR}/src/Extra/Analysis.cpp"
        "${PROJECT_SOURCE_DIR}/src/ModelBuilder/SurfByFunc.cpp"
        "${PROJECT_SOURCE_DIR}/src/ModelBuilder/SurfByFunc.h"
        "${PROJECT_SOURCE_DIR}/src/Utils/Logging/ProgressBar.cpp"
        "${PROJECT_SOURCE_DIR}/src/Utils/Logging/Errors.h"
        "${PROJECT_SOURCE_DIR}/src/Utils/Logging/LogFile.cpp"
        "${PROJECT_SOURCE_DIR}/src/Utils/ColorDefs.cpp"
        "${PROJECT_SOURCE_DIR}/src/Utils/Plasma.h"
#        "${PROJECT_SOURCE_DIR}/src/Extra/VoronoiSmoothing.cpp"
        "${PROJECT_SOURCE_DIR}/src/Textures/Material.h"
        "${PROJECT_SOURCE_DIR}/src/Utils/NormalFloat.cpp" src/ModelBuilder/Vivify.cpp src/ModelBuilder/Vivify.h)


target_include_directories(vivid PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/ModelBuilder>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/Utils>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/Textures>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/Utils/Logging>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/Wrapping>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib/MeshDecimation>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib/SurfacingAlgorithms>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib/MyBoost>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib/ImportAndExport/lodepng>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib/ImportAndExport>
#        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/pybind11/include/pybind11>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


message("VIVID SHARED BUILD")

target_link_libraries(vivid voronoi)
target_link_libraries(vivid assimp::assimp)
#target_include_directories(vivid PUBLIC ${Boost_INCLUDE_DIR})
#target_link_libraries(vivid Boost::boost)


install(DIRECTORY
        "${PROJECT_SOURCE_DIR}/src/ModelBuilder"
        "${PROJECT_SOURCE_DIR}/src/Textures"
        "${PROJECT_SOURCE_DIR}/src/Utils"
        "${PROJECT_SOURCE_DIR}/src/Utils/Logging"
        "${PROJECT_SOURCE_DIR}/src/Wrapping"
        "${PROJECT_SOURCE_DIR}/lib"
        "${PROJECT_SOURCE_DIR}/lib/MeshDecimation"
        "${PROJECT_SOURCE_DIR}/lib/SurfacingAlgorithms"
        "${PROJECT_SOURCE_DIR}/lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon"
        "${PROJECT_SOURCE_DIR}/lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc"
        "${PROJECT_SOURCE_DIR}/lib/MyBoost"
        "${PROJECT_SOURCE_DIR}/lib/ImportAndExport/lodepng"
        "${PROJECT_SOURCE_DIR}/lib/ImportAndExport"
#        "${PROJECT_SOURCE_DIR}/pybind11/include/pybind11"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h")

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

    # prevent an unused variable warning
    set(ignoreMe "${SKBUILD}")

    add_subdirectory(pybind11)
    set(PYBIND11_FINDPYTHON ON)
    set(PYBIND11_PYTHON_VERSION 3.9)


    pybind11_add_module(_vivid
            ${PROJECT_SOURCE_DIR}/src/Wrapping/wrapper.cpp
            ${PROJECT_SOURCE_DIR}/src/Wrapping/TypeCasting.h)

#    target_include_directories(vivid_py PUBLIC
#            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/ModelBuilder>
#            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/pybind11/include/pybind11>
#            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
#            )

    target_link_libraries(_vivid PRIVATE vivid)

    # Installing the extension module to the root of the package
    install(TARGETS _vivid DESTINATION .)

#    configure_file("${PROJECT_SOURCE_DIR}/src/vivid_py/__main__.py.in"
#            "${PROJECT_BINARY_DIR}/src/vivid_py/__main__.py")
#
#    install(FILES "${PROJECT_BINARY_DIR}/src/vivid_py/__main__.py" DESTINATION .)

    if(APPLE)
        set_target_properties(
                _vivid PROPERTIES INSTALL_RPATH "@loader_path/${CMAKE_INSTALL_LIBDIR}")
    else()
        set_target_properties(_vivid PROPERTIES INSTALL_RPATH
                "$ORIGIN/${CMAKE_INSTALL_LIBDIR}")
    endif()
else()
    add_executable(vivid_main
            #main
            ${PROJECT_SOURCE_DIR}/src/main.cpp)
    target_link_libraries(vivid_main vivid)
endif()




#
#
#
#
#
#    # TODO: Choose between Linux\Windows\mac
#    # TODO: Choose between py3\py27
#    # TODO: Choose between 32\64
#    # TODO: debug vs release ( different FBX path)
#
#    # TODO: Compile Elads code as a lib, and use it as such (or as subdirectory? )
#
#    # Read more at: https://github.com/ttroy50/cmake-examples
#
#
#    set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR})
#
#    message(${PROJECT_SOURCE_DIR})
#
#
#    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/SurfacingAlgorithms/huji-rich-Elad3DFast; ${CMAKE_CURRENT_SOURCE_DIR}/lib/ImportAndExport/FBX;)
#
#    execute_process(
#            COMMAND bash -c "uname -m"
#            OUTPUT_VARIABLE TGT_MACHINEpy
#    )
#
#    option( VIVID_PYTHON_COMPILE  "compile for cpp or python"  ON) #this isnt working not sure why
#    set(VIVID_PYTHON_COMPILE ON) #TODO this needs to be changes in setup.py not here
#
#    find_package( Boost 1.58.0 REQUIRED)
#    # TODO =============================================================> IF SOME PACKAGE IS NOT FOUND, WRITE A MESSAGE TO INSTALL THE LIB (AND VERSIONS? )
#
#    # FindPackage voronoi creates a new library called VoronoiLib from present local cpp files.
#    #add_subdirectory(lib/SurfacingAlgorithms/huji-rich-Elad3DFast)
#    find_package(VORONOI REQUIRED)
#
#
#    add_subdirectory(pybind11)
#    add_subdirectory(lib/assimp)
#    set(PYBIND11_FINDPYTHON ON)
#    set(PYBIND11_PYTHON_VERSION 3.9)
#
#    include_directories(src) #TODO: (TOMER) I prefer to include headers by name
#    include_directories(src/ModelBuilder)
#    include_directories(src/Utils)
#    include_directories(lib)
#    include_directories(lib/MeshDecimation)
#    include_directories(lib/SurfacingAlgorithms)
#    include_directories(lib/pybind11/include)
#    include_directories(lib/ImportAndExport)
#    include_directories(lib/ImportAndExport/lodepng)
#    include_directories(lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon)
#    include_directories(lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/)
#
#    #if (NOT ${VIVID_PYTHON_COMPILE})
#    #    MESSAGE("building vivid cpp version")
#    #    add_executable(vivid
#    #            #external
#    #            lib/Decimate.cpp
#    #            lib/SurfacingAlgorithms/Voronoi.cpp
#    #            lib/MeshDecimation/mdMeshDecimator.cpp
#    #            lib/ImportAndExport/DataToImage.cpp
#    #            lib/ImportAndExport/lodepng/lodepng.cpp
#    #            lib/ImportAndExport/ObjImportExport.cpp
#    #            lib/ImportAndExport/AssimpImportExport.cpp
#    #            #lib/ImportAndExport/FbxCode.cpp
#    #            #lib/ImportAndExport/FBXImportExport.cpp
#    #            #vivid
#    #            src/ModelBuilder/Model.cpp
#    #            src/ModelBuilder/ModelComponent.cpp
#    #            src/ModelBuilder/Mesh.cpp
#    #            src/ModelBuilder/Line.cpp
#    #            src/ModelBuilder/PointCloud.cpp
#    #
#    #            src/ModelBuilder/Surface.cpp
#    #            src/ModelBuilder/VoronoiSurface.cpp
#    #            src/ModelBuilder/MarchingCubesSurface.cpp
#    #
#    #            src/ModelBuilder/Shapes.cpp
#    #
#    #            src/ModelBuilder/Animation.cpp
#    #            src/ModelBuilder/StopMotionAnimation.cpp
#    #
#    #            src/Extra/Analysis.cpp
#    #            src/ModelBuilder/SurfByFunc.cpp
#    #            src/ModelBuilder/SurfByFunc.h
#    #            src/Utils/ProgressBar.h
#    #            src/Utils/ColorDefs.h
#    #            src/Utils/Errors.h
#    #            src/Utils/Plasma.h
#    #            src/Wrapping/TypeCasting.h
#    #            src/Extra/VoronoiSmoothing.cpp
#    #
#    #            #main
#    #            src/main.cpp)
#    #
#    #    # We use VoronoiLib and not Voronoi::voronoi because voronoi is locally created.
#    #    target_link_libraries(vivid VoronoiLib)
#    #    target_link_libraries(vivid assimp)
#    #    target_include_directories(vivid PUBLIC ${Boost_INCLUDE_DIR})
#    #    target_link_libraries(vivid Boost::boost)
#    #
#    #else()
#    #    MESSAGE("building vivid python version")
#    #    pybind11_add_module(vivid_py
#    #            #voronoi
#    #            # source/3D/GeometryCommon
#    ##            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Voronoi3D.cpp
#    ##            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Delaunay3D.cpp
#    ##            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Face.cpp
#    ##            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Intersections.cpp
#    ##            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tessellation3D.cpp
#    ##            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tetrahedron.cpp
#    ##            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Vector3D.cpp
#    ##
#    ##            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D.cpp
#    ##            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Predicates3D.cpp
#    ##            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp
#    #            #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/predicates.cxx
#    #            #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/RoundGrid3D.cpp
#    #
#    #            # source/3D/r3d
#    #            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/serializable.cpp
#    #            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/universal_error.cpp
#    #
#    #            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/int2str.cpp
#    #            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/io3D.cpp
#    #            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/simple_io.cpp
#    #            lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/utils.cpp
#    #            #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/r3d/Intersection3D.cpp
#    #            #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/r3d/r3d.c
#    #            #                #source/misc
#    #            #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/bisection.cpp
#    #            #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/func_1_var.cpp
#    #            #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/horner.cpp
#    #            #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/mesh_generator.cpp
#    #            #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/mesh_generator3D.cpp
#    #            #                #extra
#    #            #                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/tessellation/geometry.cpp
#    #            #external
#    #            lib/Decimate.cpp
#    #            lib/SurfacingAlgorithms/Voronoi.cpp
#    #            lib/MeshDecimation/mdMeshDecimator.cpp
#    #            lib/ImportAndExport/DataToImage.cpp
#    #            lib/ImportAndExport/lodepng/lodepng.cpp
#    #            lib/ImportAndExport/ObjImportExport.cpp
#    #            lib/ImportAndExport/AssimpImportExport.cpp
#    #            #lib/ImportAndExport/FbxCode.cpp
#    #            #vivid
#    #            src/ModelBuilder/Surface.cpp
#    #            src/ModelBuilder/Mesh.cpp
#    #            src/ModelBuilder/Line.cpp
#    #            src/ModelBuilder/PointCloud.cpp
#    #            src/ModelBuilder/Model.cpp
#    #            src/ModelBuilder/Shapes.cpp
#    #            src/ModelBuilder/Animation.cpp
#    #            src/Extra/Analysis.h
#    #            #lib/ImportAndExport/FBXImportExport.cpp
#    #
#    #            src/ModelBuilder/SurfByFunc.cpp
#    #            src/ModelBuilder/SurfByFunc.h
#    #            src/Utils/ProgressBar.h
#    #            src/Utils/ColorDefs.h
#    #            src/Utils/Errors.h
#    #            src/Utils/Plasma.h
#    #            src/Extra/VoronoiSmoothing.cpp
#    #
#    #            #wrapper
#    #            src/Wrapping/wrapper.cpp)
#    #
#    #
#    #    target_link_libraries(vivid_py VoronoiLib)
#    #    target_link_libraries(vivid_py assimp)
#    #    target_include_directories(vivid_py PUBLIC ${Boost_INCLUDE_DIR})
#    #    target_link_libraries(vivid_py Boost::boost)
#    #
#    #    target_compile_definitions(vivid_py
#    #            PRIVATE VERSION_INFO=${VERSION_INFO})
#    #
#    #endif ()
#
#
#
#    #target_link_libraries(vivid ${FBX_LIBRARY_SHARED_DEBUG} xml2) # TODO Y FBX::fbx?
#

