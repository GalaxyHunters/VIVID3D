# Vivid: the C++ core library and the `_core` Python extension module.
#
# Normally built through pip (scikit-build-core, see pyproject.toml). Direct use:
#   cmake -S . -B build                            # core library + Python extension (needs pybind11)
#   cmake -S . -B build -DVIVID_BUILD_PYTHON=OFF   # C++ library only
#
# Layout: lib/ holds third-party and independent code (one target per directory), src/ holds Vivid's own
# code (every directory adds its sources to the single `vivid` target), src/Wrapping builds the module.
#
# 3.15: FetchContent_MakeAvailable needs 3.14, and 3.15 is what current distro/CI CMake packages
# provide at minimum (Ubuntu 20.04: 3.16). pip builds get a recent CMake through scikit-build-core.
cmake_minimum_required(VERSION 3.15)

# The version has a single source of truth: src/vivid3d/_details.py (pyproject.toml reads it too).
# A pre-release suffix is allowed ("1.1.0rc1", "1.1.0.dev3"); CMake's project(VERSION) only takes X.Y.Z.
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/src/vivid3d/_details.py" _vivid_version_line REGEX "^__version__ *=")
string(REGEX MATCH "\"([0-9]+\\.[0-9]+\\.[0-9]+)[^\"]*\"" _vivid_version_match "${_vivid_version_line}")
if(NOT _vivid_version_match)
    message(FATAL_ERROR "Cannot read __version__ (\"X.Y.Z\" plus an optional suffix) from src/vivid3d/_details.py")
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

# Everything is linked into one shared object (the Python extension), so the static libraries below
# must be position independent.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ... and none of their symbols may leak out of it. pybind11 only hides the symbols of the module's own
# source file; without this the extension would export everything from the static libraries (assimp's
# bundled zlib: adler32, crc32, ...; Boost; Vivid itself) into the Python process, where it can clash with
# the real zlib and other libraries loaded by other extension modules. Only PyInit__core is exported.
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

if(MSVC)
    # <cmath> only defines M_PI & co. on MSVC when this is set (the sources use them, as on Linux/macOS)
    add_compile_definitions(_USE_MATH_DEFINES)
else()
    # The Voronoi library's exact predicates and the decimator's float cost arithmetic assume plain IEEE multiply and add.
    # ARM compilers fuse a*b+c by default (the x86-64 baseline has no FMA), which changes results in the last bit and,
    # through the decimator's priority queue, the mesh. Same results on every CPU.
    add_compile_options(-ffp-contract=off)
endif()

# The Vivid core library. Sources and include paths are added by the CMakeLists.txt of each directory.
add_library(vivid STATIC)

add_subdirectory(lib)    # Boost::headers, lodepng, meshdecimation, voronoi (+ the Vivid glue sources)

# assimp: installed copy or a pinned release download, see cmake/VividAssimp.cmake (target: assimp::assimp)
include(${PROJECT_SOURCE_DIR}/cmake/VividAssimp.cmake)

add_subdirectory(src)    # Vivid's own sources, and the Python module

# assimp is linked dynamically when an installed copy is used, statically when it was fetched
target_link_libraries(vivid PUBLIC voronoi lodepng meshdecimation Boost::headers assimp::assimp)
