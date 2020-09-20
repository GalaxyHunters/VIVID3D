# Locate FBX
# This module defines:
# FBX_INCLUDE_DIR, where to find the headers
#
# FBX_LIBRARY, FBX_LIBRARY_DEBUG
# FBX_FOUND
#
# $FBX_DIR is an environment variable that would
# correspond to the ./configure --prefix=$FBX_DIR

IF(APPLE)
    MESSAGE(RUNNING_FBX_FINDER_MAC)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # using regular Clang or AppleClang
        SET(FBX_LIBDIR "clang")
    else()
        SET(FBX_LIBDIR "gcc4/ub")
    endif()
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    SET(FBX_LIBDIR "gcc4")
ELSEIF(MSVC80)
    SET(FBX_LIBDIR "vs2005")
ELSEIF(MSVC90)
    SET(FBX_LIBDIR "vs2008")
ELSEIF(MSVC10)
    SET(FBX_LIBDIR "vs2010")
ELSEIF(MSVC11)
    SET(FBX_LIBDIR "vs2012")
ELSEIF(MSVC_VERSION EQUAL 1800)
    SET(FBX_LIBDIR "vs2013")
ELSEIF(MSVC_VERSION EQUAL 1900)
    SET(FBX_LIBDIR "vs2015")
ELSEIF(MSVC_VERSION GREATER_EQUAL 1910 AND MSVC_VERSION LESS 1920)
    SET(FBX_LIBDIR "vs2017")
ELSEIF(MSVC_VERSION GREATER_EQUAL 1920 AND MSVC_VERSION LESS 1930)
    #   SET(FBX_LIBDIR "vs2019") # FBX doesn't have this yet as of version 2020.0.1
    SET(FBX_LIBDIR "vs2017") # Binary compatible with vs2019
ENDIF()

IF(APPLE)
    # do nothing
ELSEIF(CMAKE_CL_64)
    SET(FBX_LIBDIR ${FBX_LIBDIR}/x64)
ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(FBX_LIBDIR ${FBX_LIBDIR}/x64)
ELSE()
    SET(FBX_LIBDIR ${FBX_LIBDIR}/x86)
ENDIF()

#try to use 2015.1 or 2014.2 version

IF(APPLE)
    SET(FBX_LIBNAME "fbxsdk")
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    SET(FBX_LIBNAME "fbxsdk")
ELSE()
    OPTION(FBX_SHARED OFF)
    IF(FBX_SHARED)
        SET(FBX_LIBNAME "libfbxsdk")
    ELSE()
        SET(FBX_LIBNAME "libfbxsdk-md")
        IF(WIN32)
            SET(FBX_XML2_LIBNAME "libxml2-md")
            SET(FBX_ZLIB_LIBNAME "zlib-md")
        ENDIF()
    ENDIF()
ENDIF()

SET(FBX_LIBNAME_DEBUG ${FBX_LIBNAME}d)

SET( FBX_SEARCH_PATHS
        $ENV{FBX_DIR}
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2020.0.1"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2020.0.1"
        "/Applications/Autodesk/FBX SDK/2020.0.1"
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2020.0"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2020.0"
        "/Applications/Autodesk/FBX SDK/2020.0"
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2019.5"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2019.5"
        "/Applications/Autodesk/FBX SDK/2019.5"
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2019.2"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2019.2"
        "/Applications/Autodesk/FBX SDK/2019.2"
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2019.0"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2019.0"
        "/Applications/Autodesk/FBX SDK/2019.0"
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2018.1.1"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2018.1.1"
        "/Applications/Autodesk/FBX SDK/2018.1.1"
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2018.0"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2018.0"
        "/Applications/Autodesk/FBX SDK/2018.0"
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2017.1"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2017.1"
        "/Applications/Autodesk/FBX SDK/2017.1"
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2017.0"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2017.0"
        "/Applications/Autodesk/FBX SDK/2017.0"
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2016.1.2"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2016.1.2"
        "/Applications/Autodesk/FBX/FBX SDK/2016.1.2"
        /Applications/Autodesk/FBXSDK201612
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2016.1.1"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2016.1.1"
        "/Applications/Autodesk/FBX/FBX SDK/2016.1.1"
        /Applications/Autodesk/FBXSDK201611
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2015.1"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2015.1"
        "/Applications/Autodesk/FBX/FBX SDK/2015.1"
        /Applications/Autodesk/FBXSDK20151
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2014.2"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2014.2"
        "/Applications/Autodesk/FBX/FBX SDK/2014.2"
        /Applications/Autodesk/FBXSDK20142
        )

# search for headers & debug/release libraries
FIND_PATH(FBX_INCLUDE_DIR "fbxsdk.h"
        PATHS ${FBX_SEARCH_PATHS}
        PATH_SUFFIXES "include")
FIND_LIBRARY( FBX_LIBRARY ${FBX_LIBNAME}
        PATHS ${FBX_SEARCH_PATHS}
        PATH_SUFFIXES "lib/${FBX_LIBDIR}/release" "lib/${FBX_LIBDIR}")

#Once one of the calls succeeds the result variable will be set and stored in the cache so that no call will search again.

#no debug d suffix, search in debug folder only
FIND_LIBRARY( FBX_LIBRARY_DEBUG ${FBX_LIBNAME}
        PATHS ${FBX_SEARCH_PATHS}
        PATH_SUFFIXES "lib/${FBX_LIBDIR}/debug")
FIND_LIBRARY( FBX_LIBRARY_DEBUG ${FBX_LIBNAME_DEBUG}
        PATHS ${FBX_SEARCH_PATHS}
        PATH_SUFFIXES "lib/${FBX_LIBDIR}")

IF(WIN32)
    MESSAGE(RUNNING_FBX_FINDER_WIN32)
    FIND_LIBRARY( FBX_XML2_LIBRARY ${FBX_XML2_LIBNAME}
            PATHS ${FBX_SEARCH_PATHS}
            PATH_SUFFIXES "lib/${FBX_LIBDIR}/release" "lib/${FBX_LIBDIR}")
    FIND_LIBRARY( FBX_ZLIB_LIBRARY ${FBX_ZLIB_LIBNAME}
            PATHS ${FBX_SEARCH_PATHS}
            PATH_SUFFIXES "lib/${FBX_LIBDIR}/release" "lib/${FBX_LIBDIR}")
    FIND_LIBRARY( FBX_XML2_LIBRARY_DEBUG ${FBX_XML2_LIBNAME}
            PATHS ${FBX_SEARCH_PATHS}
            PATH_SUFFIXES "lib/${FBX_LIBDIR}/debug")
    FIND_LIBRARY( FBX_ZLIB_LIBRARY_DEBUG ${FBX_ZLIB_LIBNAME}
            PATHS ${FBX_SEARCH_PATHS}
            PATH_SUFFIXES "lib/${FBX_LIBDIR}/debug")
ENDIF()

IF(FBX_LIBRARY AND FBX_LIBRARY_DEBUG AND FBX_INCLUDE_DIR)
    SET(FBX_FOUND "YES")
ELSE()
    SET(FBX_FOUND "NO")
ENDIF()

IF(NOT FBX_FOUND)
    #try to use 2014.1 version
    IF(APPLE)
        SET(FBX_LIBNAME "fbxsdk-2014.1")
    ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
        SET(FBX_LIBNAME "fbxsdk-2014.1")
    ELSE()
        SET(FBX_LIBNAME "fbxsdk-2014.1")
    ENDIF()

    SET(FBX_LIBNAME_DEBUG ${FBX_LIBNAME}d)

    SET( FBX_SEARCH_PATHS
            $ENV{FBX_DIR}
            "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2014.1"
            "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2014.1"
            "/Applications/Autodesk/FBX/FBX SDK/2014.1"
            /Applications/Autodesk/FBXSDK20141
            )

    # search for headers & debug/release libraries
    FIND_PATH(FBX_INCLUDE_DIR "fbxsdk.h"
            PATHS ${FBX_SEARCH_PATHS}
            PATH_SUFFIXES "include")
    FIND_LIBRARY( FBX_LIBRARY ${FBX_LIBNAME}
            PATHS ${FBX_SEARCH_PATHS}
            PATH_SUFFIXES "lib/${FBX_LIBDIR}")

    FIND_LIBRARY( FBX_LIBRARY_DEBUG ${FBX_LIBNAME_DEBUG}
            PATHS ${FBX_SEARCH_PATHS}
            PATH_SUFFIXES "lib/${FBX_LIBDIR}")
    IF(FBX_LIBRARY AND FBX_LIBRARY_DEBUG AND FBX_INCLUDE_DIR)
        SET(FBX_FOUND "YES")
    ELSE()
        SET(FBX_FOUND "NO")
    ENDIF()

ENDIF()

IF(NOT FBX_FOUND)
    #try to use 2013.3 version
    IF(APPLE)
        SET(FBX_LIBNAME "fbxsdk-2013.3-static")
    ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
        SET(FBX_LIBNAME "fbxsdk-2013.3-static")
    ELSE()
        SET(FBX_LIBNAME "fbxsdk-2013.3-md")
    ENDIF()

    SET(FBX_LIBNAME_DEBUG ${FBX_LIBNAME}d)

    SET( FBX_SEARCH_PATHS
            $ENV{FBX_DIR}
            "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2013.3"
            "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2013.3"
            "/Applications/Autodesk/FBX/FBX SDK/2013.3"
            /Applications/Autodesk/FBXSDK20133
            )

    # search for headers & debug/release libraries
    FIND_PATH(FBX_INCLUDE_DIR "fbxsdk.h"
            PATHS ${FBX_SEARCH_PATHS}
            PATH_SUFFIXES "include")
    FIND_LIBRARY( FBX_LIBRARY ${FBX_LIBNAME}
            PATHS ${FBX_SEARCH_PATHS}
            PATH_SUFFIXES "lib/${FBX_LIBDIR}")

    FIND_LIBRARY( FBX_LIBRARY_DEBUG ${FBX_LIBNAME_DEBUG}
            PATHS ${FBX_SEARCH_PATHS}
            PATH_SUFFIXES "lib/${FBX_LIBDIR}")
    IF(FBX_LIBRARY AND FBX_LIBRARY_DEBUG AND FBX_INCLUDE_DIR)
        SET(FBX_FOUND "YES")
    ELSE()
        SET(FBX_FOUND "NO")
    ENDIF()

ENDIF()

#linking the sdk in unix
IF(UNIX)
    FIND_PACKAGE(LibXml2)
#    SET(LIBXML2_LIBRARY "/usr/lib/x86_64-linux-gnu/libxml2.a")
    MESSAGE(LIBXML2_LIBRARY=${LIBXML2_LIBRARY})

#    ADD_LIBRARY(LIBXML2 STATIC IMPORTED)

    MESSAGE(RUNNING_FBX_FINDER_UNIX)
    SET(FBX_ROOT "/usr/local/lib/FBX")              #TODO this needs to be expanded
    IF(FBX_ROOT)
        #setting FBX_INCLUDE_DIR
        SET(FBX_INCLUDE_DIR ${FBX_ROOT}/include)
        IF(FBX_INCLUDE_DIR)
            MESSAGE(FBX_INCLUDE_DIR=FOUND)
        ELSE()
            MESSAGE(FBX_INCLUDE_DIR=NOT_FOUND)
        ENDIF()

        #setting FBX_LIBRARY
        SET(FBX_LIBRARY_SHARED_RELEASE ${FBX_ROOT}/lib/gcc/x64/release/libfbxsdk.so)
        SET(FBX_LIBRARY_STATIC_RELEASE ${FBX_ROOT}/lib/gcc/x64/release/libfbxsdk.a)
        IF(FBX_LIBRARY_SHARED_RELEASE AND FBX_LIBRARY_STATIC_RELEASE)
            MESSAGE(FBX_LIBRARY=FOUND)
        ELSE()
            MESSAGE(FBX_LIBRARY=NOT_FOUND)
        ENDIF()

        #setting FBX_LIBRARY_DEBUG
        SET(FBX_LIBRARY_SHARED_DEBUG ${FBX_ROOT}/lib/gcc/x64/debug/libfbxsdk.so)
        SET(FBX_LIBRARY_STATIC_DEBUG ${FBX_ROOT}/lib/gcc/x64/debug/libfbxsdk.a)
        IF(FBX_LIBRARY_SHARED_DEBUG AND FBX_LIBRARY_STATIC_DEBUG)
            MESSAGE(FBX_LIBRARY_DEBUG=FOUND)
        ELSE()
            MESSAGE(FBX_LIBRARY_DEBUG=NOT_FOUND)
        ENDIF()

        #Creating the imported target FBX::fbx

        IF(NOT TARGET FBX::fbx)
            ADD_LIBRARY(FBX::fbx STATIC IMPORTED GLOBAL)
            SET_TARGET_PROPERTIES(FBX::fbx PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${FBX_INCLUDE_DIR}")     #TODO need to create multiple code snippets to create a target for each fbx library file, also needed in windowns for relocatable lib
            SET_TARGET_PROPERTIES(FBX::fbx PROPERTIES IMPORTED_LOCATION "${FBX_LIBRARY_STATIC_DEBUG}")
            SET_TARGET_PROPERTIES(FBX::fbx PROPERTIES IMPORTED_LINK_INTERFACE_LIBRARIES "${LIBXML2_LIBRARY}")
            SET_TARGET_PROPERTIES(FBX::fbx PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
#            ADD_DEPENDENCIES(FBX::fbx LibXml2::LibXml2)
            MESSAGE(FBX::fbx=CREATED)
        ENDIF()

        #TODO this might be using a C compiler, check online

        #defining a xml2 target for dependency

#        IF(LibXml2_FOUND AND NOT TARGET LibXml2::LibXml2)
#            ADD_LIBRARY(LibXml2::LibXml2 UNKNOWN IMPORTED)
#            SET_TARGET_PROPERTIES(LibXml2::LibXml2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LIBXML2_INCLUDE_DIRS}")
#            SET_TARGET_PROPERTIES(LibXml2::LibXml2 PROPERTIES INTERFACE_COMPILE_OPTIONS "${LIBXML2_DEFINITIONS}")
#            SET_TARGET_PROPERTIES(LibXml2::LibXml2 PROPERTIES INTERFACE_LINK_LIBRARIES "${LIBXML2_LIBRARY}")
#            SET_PROPERTY(TARGET LibXml2::LibXml2 APPEND PROPERTY IMPORTED_LOCATION "${LIBXML2_LIBRARY}")
#        ENDIF()
#
#        IF(NOT TARGET FBX::fbx)
#            ADD_LIBRARY(FBX::fbx INTERFACE IMPORTED)                                                          #TODO for tommorow, this is not supposed to be interface cuz fbx is not a header only library, utilize http://mesos.apache.org/documentation/latest/cmake-examples/
#            SET_TARGET_PROPERTIES(FBX::fbx PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${FBX_INCLUDE_DIR}")     #TODO somehow add xml2 into this
#            SET_TARGET_PROPERTIES(FBX::fbx PROPERTIES INTERFACE_LINK_LIBRARIES "${FBX_LIBRARY_DEBUG}")
##            SET_TARGET_PROPERTIES(FBX::fbx PROPERTIES INTERFACE_LINK_LIBRARIES "${LIBXML2_LIBRARY}")
#            ADD_DEPENDENCIES(FBX::fbx xml2)
#            MESSAGE(FBX::fbx=CREATED)
#        ENDIF()

        #TODO extern c
        #TODO Mangled Symbols


        IF(FBX_LIBRARY_SHARED_RELEASE AND FBX_LIBRARY_SHARED_DEBUG AND FBX_LIBRARY_STATIC_RELEASE AND FBX_LIBRARY_STATIC_DEBUG AND FBX_INCLUDE_DIR AND TARGET FBX::fbx)
            SET(FBX_FOUND "YES")
        ELSE()
            SET(FBX_FOUND "NO")
        ENDIF()



    ENDIF()
ENDIF()