# Locate FBX
# This module defines:
# FBX_INCLUDE_DIR, where to find the headers
#
# FBX_LIBRARY, FBX_LIBRARY_DEBUG
# FBX_FOUND
#
# $FBX_DIR is an environment variable that would
# correspond to the ./configure --prefix=$FBX_DIR



# === Set FBX_LIBDIR to compiler type ==============================================================================>>>
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


# === Set FBX_LIBNAME and FBX_LIBNAME_DEBUG ========================================================================>>>
# TODO fix the if-else mechanism especially the GNU part

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

# === Find the FBX package: ========================================================================================>>>
# Common library installation paths list
# TODO: try to add linux path here!
SET(FBX_SEARCH_PATHS
        $ENV{FBX_DIR}
        "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2020.2.1"
        "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2020.2.1"
        "/Applications/Autodesk/FBX SDK/2020.2.1"
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

# look at the common list for the headers, debug/release, and shared/static libraries
FIND_PATH(FBX_INCLUDE_DIR "fbxsdk.h"
        PATHS ${FBX_SEARCH_PATHS}
        PATH_SUFFIXES "include")
FIND_LIBRARY(FBX_LIBRARY ${FBX_LIBNAME}
        PATHS ${FBX_SEARCH_PATHS}
        PATH_SUFFIXES "lib/${FBX_LIBDIR}/release" "lib/${FBX_LIBDIR}")
FIND_LIBRARY(FBX_LIBRARY_DEBUG ${FBX_LIBNAME}
        PATHS ${FBX_SEARCH_PATHS}
        PATH_SUFFIXES "lib/${FBX_LIBDIR}/debug")

# Currently unused, cmake takes only .lib files!
# -------------------------------------------------------------------------------------------------------------------->
#SET(FBX_LIBNAME_SHARED "libfbxsdk.dll")
#SET(FBX_LIBNAME_STATIC "libfbxsdk-md.lib")
#FIND_LIBRARY(FBX_LIBRARY_SHARED_RELEASE ${FBX_LIBNAME_SHARED}
#        PATHS ${FBX_SEARCH_PATHS}
#        PATH_SUFFIXES "lib/${FBX_LIBDIR}/release" "lib/${FBX_LIBDIR}")
#FIND_LIBRARY(FBX_LIBRARY_STATIC_RELEASE ${FBX_LIBNAME_STATIC}
#        PATHS ${FBX_SEARCH_PATHS}
#        PATH_SUFFIXES "lib/${FBX_LIBDIR}/release" "lib/${FBX_LIBDIR}")
#FIND_LIBRARY(FBX_LIBRARY_SHARED_DEBUG ${FBX_LIBNAME_SHARED}
#        PATHS ${FBX_SEARCH_PATHS}
#        PATH_SUFFIXES "lib/${FBX_LIBDIR}/debug")
#FIND_LIBRARY(FBX_LIBRARY_STATIC_DEBUG ${FBX_LIBNAME_STATIC}
#        PATHS ${FBX_SEARCH_PATHS}
#        PATH_SUFFIXES "lib/${FBX_LIBDIR}/debug")

IF(FBX_LIBRARY AND FBX_LIBRARY_DEBUG AND FBX_INCLUDE_DIR)
    SET(FBX_FOUND "YES")
ELSE()
    SET(FBX_FOUND "NO")
ENDIF()

# ---- FIND ZLIB & LIBXML2 LIBS ---
# TODO: finish this code, Notice we don't need it in 2019 with vs2015!
SET(LIBXML2_LIBRARY_DEBUG BLAT)
SET(LIBXML2_LIBRARY_RELEASE BLAT)
SET(ZLIB_LIBRARY_DEBUG BLAT)
SET(ZLIB_LIBRARY_RELEASE BLAT)
#
#        FIND_PACKAGE(LibXml2)
#        MESSAGE(LIBXML2_LIBRARY=${LIBXML2_LIBRARY})

#SET(FBX_ROOT "C:/Program Files/Autodesk/FBX/FBX SDK/2019.0")
##setting libxml2 and zlib variables
#SET(LIBXML2_LIBRARY_DEBUG ${FBX_ROOT}/lib/vs2015/x64/debug/libxml2-md.lib)
#SET(LIBXML2_LIBRARY_RELEASE ${FBX_ROOT}/lib/vs2015/x64/release/libxml2-md.lib)
#
#SET(ZLIB_LIBRARY_DEBUG ${FBX_ROOT}/lib/vs2015/x64/debug/zlib-md.lib)
#SET(ZLIB_LIBRARY_RELEASE ${FBX_ROOT}/lib/vs2015/x64/release/zlib-md.lib)
#
#message(FLOPPATY1)
#message(${FBX_XML2_LIBRARY})
#message(${FBX_XML2_LIBRARY_DEBUG})
#message(${FBX_ZLIB_LIBRARY})
#message(${FBX_ZLIB_LIBRARY_DEBUG})
#
## TODO (Zohar): what this part do?
#IF(WIN32)
#    MESSAGE(RUNNING_FBX_FINDER_WIN32)
#    FIND_LIBRARY(FBX_XML2_LIBRARY ${FBX_XML2_LIBNAME}
#            PATHS ${FBX_SEARCH_PATHS}
#            PATH_SUFFIXES "lib/${FBX_LIBDIR}/release" "lib/${FBX_LIBDIR}")
#    FIND_LIBRARY(FBX_XML2_LIBRARY_DEBUG ${FBX_XML2_LIBNAME}
#            PATHS ${FBX_SEARCH_PATHS}
#            PATH_SUFFIXES "lib/${FBX_LIBDIR}/debug")
#    FIND_LIBRARY(FBX_ZLIB_LIBRARY ${FBX_ZLIB_LIBNAME}
#            PATHS ${FBX_SEARCH_PATHS}
#            PATH_SUFFIXES "lib/${FBX_LIBDIR}/release" "lib/${FBX_LIBDIR}")
#    FIND_LIBRARY(FBX_ZLIB_LIBRARY_DEBUG ${FBX_ZLIB_LIBNAME}
#            PATHS ${FBX_SEARCH_PATHS}
#            PATH_SUFFIXES "lib/${FBX_LIBDIR}/debug")
#ENDIF()
#
IF(LIBXML2_LIBRARY_DEBUG AND LIBXML2_LIBRARY_RELEASE AND ZLIB_LIBRARY_DEBUG AND ZLIB_LIBRARY_RELEASE)
    SET(DEPENDECIES_FOUND "YES")
ELSE()
    SET(DEPENDECIES_FOUND "NO")
ENDIF()

#message(FLOPPATY2)
#message(${FBX_XML2_LIBRARY})
#message(${FBX_XML2_LIBRARY_DEBUG})
#message(${FBX_ZLIB_LIBRARY})
#message(${FBX_ZLIB_LIBRARY_DEBUG})




# <<<=== Find the FBX package (general part) ===============================================================================

# Apparently legacy support for 2014 FBX,
#IF(NOT FBX_FOUND)
#    #try to use 2014.1 version
#    IF(APPLE)
#        SET(FBX_LIBNAME "fbxsdk-2014.1")
#    ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
#        SET(FBX_LIBNAME "fbxsdk-2014.1")
#    ELSE()
#        SET(FBX_LIBNAME "fbxsdk-2014.1")
#    ENDIF()
#
#    SET(FBX_LIBNAME_DEBUG ${FBX_LIBNAME}d)
#
#    SET( FBX_SEARCH_PATHS
#            $ENV{FBX_DIR}
#            "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2014.1"
#            "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2014.1"
#            "/Applications/Autodesk/FBX/FBX SDK/2014.1"
#            /Applications/Autodesk/FBXSDK20141
#            )
#    # search for headers & debug/release libraries
#    FIND_PATH(FBX_INCLUDE_DIR "fbxsdk.h"
#            PATHS ${FBX_SEARCH_PATHS}
#            PATH_SUFFIXES "include")
#    FIND_LIBRARY( FBX_LIBRARY ${FBX_LIBNAME}
#            PATHS ${FBX_SEARCH_PATHS}
#            PATH_SUFFIXES "lib/${FBX_LIBDIR}")
#    FIND_LIBRARY( FBX_LIBRARY_DEBUG ${FBX_LIBNAME_DEBUG}
#            PATHS ${FBX_SEARCH_PATHS}
#            PATH_SUFFIXES "lib/${FBX_LIBDIR}")
#    IF(FBX_LIBRARY AND FBX_LIBRARY_DEBUG AND FBX_INCLUDE_DIR)
#        SET(FBX_FOUND "YES")
#    ELSE()
#        SET(FBX_FOUND "NO")
#    ENDIF()
#ENDIF()

# Apparently legacy support for 2013 FBX,
#IF(NOT FBX_FOUND)
#    #try to use 2013.3 version
#    IF(APPLE)
#        SET(FBX_LIBNAME "fbxsdk-2013.3-static")
#    ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
#        SET(FBX_LIBNAME "fbxsdk-2013.3-static")
#    ELSE()
#        SET(FBX_LIBNAME "fbxsdk-2013.3-md")
#    ENDIF()
#    SET(FBX_LIBNAME_DEBUG ${FBX_LIBNAME}d)
#    SET( FBX_SEARCH_PATHS
#            $ENV{FBX_DIR}
#            "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2013.3"
#            "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2013.3"
#            "/Applications/Autodesk/FBX/FBX SDK/2013.3"
#            /Applications/Autodesk/FBXSDK20133
#            )
#    # search for headers & debug/release libraries
#    FIND_PATH(FBX_INCLUDE_DIR "fbxsdk.h"
#            PATHS ${FBX_SEARCH_PATHS}
#            PATH_SUFFIXES "include")
#    FIND_LIBRARY( FBX_LIBRARY ${FBX_LIBNAME}
#            PATHS ${FBX_SEARCH_PATHS}
#            PATH_SUFFIXES "lib/${FBX_LIBDIR}")
#
#    FIND_LIBRARY( FBX_LIBRARY_DEBUG ${FBX_LIBNAME_DEBUG}
#            PATHS ${FBX_SEARCH_PATHS}
#            PATH_SUFFIXES "lib/${FBX_LIBDIR}")
#    IF(FBX_LIBRARY AND FBX_LIBRARY_DEBUG AND FBX_INCLUDE_DIR)
#        SET(FBX_FOUND "YES")
#    ELSE()
#        SET(FBX_FOUND "NO")
#    ENDIF()
#ENDIF()

# TODO (Zohar): I really think we can pu most of it in the general pasrt and the final compilation with the windows part, talk to me!
#IF(NOT FBX_FOUND) # TODO: TOMER My addition 20201005_1830
#linking the sdk in unix



FUNCTION(IS_FOUND VAR)
    IF(EXISTS ${${VAR}})
        MESSAGE(${VAR} - FOUND!: ${${VAR}})
    ELSE()
        MESSAGE(FATAL_ERROR ${VAR} - NOT_FOUND!)
    ENDIF()
ENDFUNCTION()



IF(UNIX)
    FIND_PACKAGE(LibXml2)
    #    SET(LIBXML2_LIBRARY "/usr/lib/x86_64-linux-gnu/libxml2.a")
#    MESSAGE(LIBXML2_LIBRARY=${LIBXML2_LIBRARY})
    IS_FOUND(LIBXML2_LIBRARY)

    #    ADD_LIBRARY(LIBXML2 STATIC IMPORTED)

    MESSAGE(RUNNING_FBX_FINDER_UNIX)
    IF(APPLE)
        SET(FBX_ROOT "/Applications/Autodesk/FBX SDK/2020.2.1")
        IS_FOUND(FBX_ROOT)

        #setting FBX_INCLUDE_DIR
        SET(FBX_INCLUDE_DIR ${FBX_ROOT}/include)
        IS_FOUND(FBX_INCLUDE_DIR)

        #setting FBX_LIBRARY
        SET(FBX_LIBRARY_SHARED_RELEASE ${FBX_ROOT}/lib/clang/release/libfbxsdk.dylib)
        IS_FOUND(FBX_LIBRARY_SHARED_RELEASE)

        SET(FBX_LIBRARY_STATIC_RELEASE ${FBX_ROOT}/lib/clang/release/libfbxsdk.a)
        IS_FOUND(FBX_LIBRARY_STATIC_RELEASE)

        #setting FBX_LIBRARY_DEBUG
        SET(FBX_LIBRARY_SHARED_DEBUG ${FBX_ROOT}/lib/clang/debug/libfbxsdk.dylib)
        IS_FOUND(FBX_LIBRARY_SHARED_DEBUG)

        SET(FBX_LIBRARY_STATIC_DEBUG ${FBX_ROOT}/lib/clang/debug/libfbxsdk.a)
        IS_FOUND(FBX_LIBRARY_STATIC_DEBUG)
    ELSE()
        SET(FBX_ROOT "/usr")              #TODO this needs to be expanded
        IS_FOUND(FBX_ROOT)

        #setting FBX_INCLUDE_DIR
        SET(FBX_INCLUDE_DIR ${FBX_ROOT}/include)
        IS_FOUND(FBX_INCLUDE_DIR)

        #setting FBX_LIBRARY
        SET(FBX_LIBRARY_SHARED_RELEASE ${FBX_ROOT}/lib/gcc/x64/release/libfbxsdk.so)
        IS_FOUND(FBX_LIBRARY_SHARED_RELEASE)

        SET(FBX_LIBRARY_STATIC_RELEASE ${FBX_ROOT}/lib/gcc/x64/release/libfbxsdk.a)
        IS_FOUND(FBX_LIBRARY_STATIC_RELEASE)


        #setting FBX_LIBRARY_DEBUG
        SET(FBX_LIBRARY_SHARED_DEBUG ${FBX_ROOT}/lib/gcc/x64/debug/libfbxsdk.so)
        IS_FOUND(FBX_LIBRARY_SHARED_DEBUG)

        SET(FBX_LIBRARY_STATIC_DEBUG ${FBX_ROOT}/lib/gcc/x64/debug/libfbxsdk.a)
        IS_FOUND(FBX_LIBRARY_STATIC_DEBUG)
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
ENDIF()



# TODO: Zohar, I've changed the flow here to be simple to read, I think we should let go also to the WIN32 If statment
IF(WIN32)
    MESSAGE(RUNNING_FBX_FINDER_WIN32)

    #creating the imported target FBX::fbx
    IF(NOT TARGET FBX::fbx) #TODO add here compilation with the debug libs !!!
        ADD_LIBRARY(FBX::fbx STATIC IMPORTED GLOBAL)
        #TODO need to create multiple code snippets to create a target for each fbx library file, also needed in windowns for relocatable lib
        SET_TARGET_PROPERTIES(FBX::fbx PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${FBX_INCLUDE_DIR}")
        SET_TARGET_PROPERTIES(FBX::fbx PROPERTIES IMPORTED_LOCATION "${FBX_LIBRARY}")
#                SET_TARGET_PROPERTIES(FBX::fbx PROPERTIES IMPORTED_LINK_INTERFACE_LIBRARIES "${LIBXML2_LIBRARY_RELEASE};${ZLIB_LIBRARY_RELEASE}") # TODO we don't need it in 2019 version
        #                TARGET_LINK_LIBRARIES(FBX::fbx INTERFACE ${LIBXML2_LIBRARY_DEBUG} ${ZLIB_LIBRARY_DEBUG})
        SET_TARGET_PROPERTIES(FBX::fbx PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
        MESSAGE(FBX::fbx=CREATED)
    ENDIF()
ENDIF()