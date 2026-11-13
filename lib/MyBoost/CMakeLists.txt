# Provides the target Boost::headers (Boost is used header-only) from one of three sources,
# selected by VIVID_BOOST:
#
#   VENDORED  (default) the copy in this directory: Boost 1.92.0 trimmed to the ~1000 headers the
#             compiled sources need (gcc + MSVC include closure; see README.md in this directory)
#   SYSTEM    require an installed Boost of at least VIVID_BOOST_MIN_VERSION (find_package, hard error)
#   AUTO      use an installed Boost when found and new enough, otherwise VENDORED
#
# Examples:  -DVIVID_BOOST=SYSTEM   -DBOOST_ROOT=/opt/boost   -DCMAKE_PREFIX_PATH=C:/vcpkg/installed/x64-windows
# Only the vendored copy is tested on every release; 1.80.0 (the previous vendored version) and 1.92.0
# are the versions tools/golden has verified.

set(VIVID_BOOST "VENDORED" CACHE STRING "Boost source: VENDORED, SYSTEM or AUTO")
set_property(CACHE VIVID_BOOST PROPERTY STRINGS VENDORED SYSTEM AUTO)
set(VIVID_BOOST_MIN_VERSION "1.80" CACHE STRING "Oldest installed Boost accepted for SYSTEM/AUTO")

if(NOT VIVID_BOOST MATCHES "^(VENDORED|SYSTEM|AUTO)$")
    message(FATAL_ERROR "VIVID_BOOST must be VENDORED, SYSTEM or AUTO (got '${VIVID_BOOST}')")
endif()

set(_vivid_boost_source "${VIVID_BOOST}")

if(NOT _vivid_boost_source STREQUAL "VENDORED")
    # CMake >= 3.30 has no FindBoost module (CMP0167): Boost's own BoostConfig.cmake is used instead.
    if(POLICY CMP0167)
        cmake_policy(SET CMP0167 NEW)
    endif()
    if(_vivid_boost_source STREQUAL "SYSTEM")
        find_package(Boost ${VIVID_BOOST_MIN_VERSION} REQUIRED)
    else()
        find_package(Boost ${VIVID_BOOST_MIN_VERSION} QUIET)
    endif()
    if(Boost_FOUND AND TARGET Boost::headers)
        message(STATUS "Vivid: using system Boost ${Boost_VERSION} (VIVID_BOOST=${VIVID_BOOST})")
        return()   # Boost::headers is provided by find_package
    endif()
    if(_vivid_boost_source STREQUAL "SYSTEM")
        message(FATAL_ERROR "VIVID_BOOST=SYSTEM: Boost found but the target Boost::headers is missing")
    endif()
    message(STATUS "Vivid: no usable system Boost >= ${VIVID_BOOST_MIN_VERSION}, using the vendored copy")
endif()

add_library(vivid_boost INTERFACE)
target_include_directories(vivid_boost SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(Boost::headers ALIAS vivid_boost)
message(STATUS "Vivid: using vendored Boost 1.92.0")
