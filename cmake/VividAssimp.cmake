# Provides the target assimp::assimp from one of four sources, selected by VIVID_ASSIMP:
#
#   AUTO      (default) use an installed assimp if it is new enough, otherwise FETCH
#   SYSTEM    require an installed assimp (hard error if missing or too old)
#   FETCH     download and build the pinned release below (needs network at configure time)
#   VENDORED  build the copy in lib/ImportExport/assimp (legacy; to be removed)
#
# Examples:  -DVIVID_ASSIMP=SYSTEM   -DVIVID_ASSIMP_MIN_VERSION=5.3.0

set(VIVID_ASSIMP "AUTO" CACHE STRING "assimp source: AUTO, SYSTEM, FETCH or VENDORED")
set_property(CACHE VIVID_ASSIMP PROPERTY STRINGS AUTO SYSTEM FETCH VENDORED)
set(VIVID_ASSIMP_MIN_VERSION "5.2.5" CACHE STRING
    "Oldest installed assimp accepted for SYSTEM/AUTO (checked against assimp_VERSION)")
# Installed versions that must not be used even though they are newer than the minimum.
#   6.0.3, 6.0.4: FBXExporter dereferences an end() iterator when writing a texture whose path is not
#   in its lookup table (undefined behaviour: crash or garbage). Fixed upstream in 6.0.5. Vivid hits it
#   whenever it exports a model with an embedded colormap texture to FBX. Ubuntu 26.04 ships 6.0.4.
set(VIVID_ASSIMP_EXCLUDED_VERSIONS "6.0.3;6.0.4" CACHE STRING
    "Installed assimp versions rejected as known-bad (list)")
set(VIVID_ASSIMP_FETCH_VERSION "6.0.6" CACHE STRING "assimp release downloaded by FETCH")
set(VIVID_ASSIMP_FETCH_SHA256
    "5f7ed412b76575621df0525a762b31da5874afff352b3a890c6308db4ac7204c" CACHE STRING
    "SHA-256 of the source tarball of VIVID_ASSIMP_FETCH_VERSION (official assimp/assimp release)")

if(NOT VIVID_ASSIMP MATCHES "^(AUTO|SYSTEM|FETCH|VENDORED)$")
    message(FATAL_ERROR "VIVID_ASSIMP must be AUTO, SYSTEM, FETCH or VENDORED (got '${VIVID_ASSIMP}')")
endif()

set(_vivid_assimp_source "${VIVID_ASSIMP}")

# --- Decide between SYSTEM and FETCH ------------------------------------------------------------
if(_vivid_assimp_source STREQUAL "AUTO" OR _vivid_assimp_source STREQUAL "SYSTEM")
    # assimp's own <pkg>ConfigVersion.cmake only accepts the *same major version*, so a request for
    # "assimp 5.3" would reject an installed 6.0.4. Probe the version ourselves instead. The probe
    # runs in its own directory scope so that, if the install is rejected, its imported targets do
    # not collide with a fetched assimp::assimp.
    add_subdirectory(${PROJECT_SOURCE_DIR}/cmake/probe_assimp ${CMAKE_BINARY_DIR}/probe_assimp)

    set(_vivid_assimp_usable FALSE)
    set(_why "no installed assimp (CMake package 'assimp') was found")
    if(VIVID_ASSIMP_PROBED_VERSION)
        set(_vivid_assimp_usable TRUE)
        if(VIVID_ASSIMP_PROBED_VERSION VERSION_LESS VIVID_ASSIMP_MIN_VERSION)
            set(_vivid_assimp_usable FALSE)
            set(_why "found assimp ${VIVID_ASSIMP_PROBED_VERSION}, but at least ${VIVID_ASSIMP_MIN_VERSION} is required")
        endif()
        foreach(_bad IN LISTS VIVID_ASSIMP_EXCLUDED_VERSIONS)
            if(VIVID_ASSIMP_PROBED_VERSION VERSION_EQUAL _bad)
                set(_vivid_assimp_usable FALSE)
                set(_why "found assimp ${VIVID_ASSIMP_PROBED_VERSION}, which has a known bug that crashes Vivid's FBX export (excluded versions: ${VIVID_ASSIMP_EXCLUDED_VERSIONS})")
            endif()
        endforeach()
    endif()

    if(_vivid_assimp_usable)
        find_package(assimp CONFIG REQUIRED)
        set(_vivid_assimp_source "SYSTEM")
    elseif(_vivid_assimp_source STREQUAL "SYSTEM")
        message(FATAL_ERROR
            "VIVID_ASSIMP=SYSTEM: ${_why}.\n"
            "Install it (Debian/Ubuntu: libassimp-dev, Fedora: assimp-devel, macOS: brew install assimp,\n"
            "vcpkg/conda: assimp), or use -DVIVID_ASSIMP=FETCH to download and build it.")
    else()
        message(STATUS "assimp: ${_why}; fetching ${VIVID_ASSIMP_FETCH_VERSION} instead")
        set(_vivid_assimp_source "FETCH")
    endif()
endif()

# --- Options shared by builds from source (FETCH / VENDORED) ------------------------------------
if(_vivid_assimp_source STREQUAL "FETCH" OR _vivid_assimp_source STREQUAL "VENDORED")
    set(ASSIMP_INSTALL OFF CACHE INTERNAL "")
    set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")
    set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE INTERNAL "")
    set(ASSIMP_BUILD_ZLIB ON CACHE INTERNAL "")
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
    set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE INTERNAL "")
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "")
    set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE INTERNAL "")
    set(ASSIMP_INSTALL_PDB OFF CACHE INTERNAL "")
    # assimp is linked statically into the shared libvivid, so it must be position independent.
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

if(_vivid_assimp_source STREQUAL "FETCH")
    include(FetchContent)
    FetchContent_Declare(assimp
        URL "https://github.com/assimp/assimp/archive/refs/tags/v${VIVID_ASSIMP_FETCH_VERSION}.tar.gz"
        URL_HASH "SHA256=${VIVID_ASSIMP_FETCH_SHA256}")
    FetchContent_MakeAvailable(assimp)
elseif(_vivid_assimp_source STREQUAL "VENDORED")
    set(ASSIMP_DIR ${PROJECT_LIB_DIR}/ImportExport/assimp)
    # TEMPORARY: vendored assimp 5.2.x uses uint8_t without including <cstdint>, which fails on
    # GCC 13+. Remove together with the vendored copy.
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include cstdint")
    endif()
    add_subdirectory(${ASSIMP_DIR})
    include_directories(${ASSIMP_DIR}/include)
endif()

if(NOT TARGET assimp::assimp)
    message(FATAL_ERROR "assimp source '${_vivid_assimp_source}' did not provide the target assimp::assimp")
endif()

if(_vivid_assimp_source STREQUAL "SYSTEM")
    message(STATUS "assimp: using installed ${assimp_VERSION} (minimum ${VIVID_ASSIMP_MIN_VERSION}), linked dynamically")
elseif(_vivid_assimp_source STREQUAL "FETCH")
    message(STATUS "assimp: fetched ${VIVID_ASSIMP_FETCH_VERSION}, linked statically")
else()
    message(STATUS "assimp: vendored copy, linked statically")
endif()
set(VIVID_ASSIMP_RESOLVED "${_vivid_assimp_source}" CACHE INTERNAL "How assimp was provided for this build")
