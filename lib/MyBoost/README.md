# Vendored Boost (headers only)

Boost **1.92.0**, trimmed to what Vivid compiles. Selected by the CMake option `VIVID_BOOST`
(`VENDORED` default, `SYSTEM`, `AUTO`; see `CMakeLists.txt` here).

- Source: official release `boost-1.92.0-b2-nodocs.tar.xz` from <https://github.com/boostorg/boost/releases/tag/boost-1.92.0>,
  SHA-256 `ea7b982002cc9dfbe59b0b217b206f470dc75f3de0bb2973d844118934d82411` (matches the digest GitHub publishes).
- Contents: the union of the include closures of every compiled source (gcc `-std=gnu++17`/`c++17`/`gnu++20`,
  MSVC `/std:c++17`), all of `boost/config` and `boost/predef` (compiler/platform selection), and all of
  `boost/multiprecision`, `boost/math/tools` and `boost/math/cstdfloat` (many conditional includes).
  Library license: `LICENSE_1_0.txt` (Boost Software License 1.0).
- Used by: the Voronoi library (`array`, `container`, `random`, `multiprecision`, `foreach`) and two string
  predicates in Vivid (`algorithm/string/predicate.hpp`).

## Upgrading or adding a header

The include closure depends on the compiler, so a new compiler or a new include can reach a header that is
not here (the build then stops with `fatal error: boost/...: No such file`). Fix: copy the missing header
(and whatever it includes) from the same official release, or re-run the closure with `g++ -M` and
`cl /showIncludes` on the sources listed in `lib/SurfacingAlgorithms/huji-rich-Elad3DFast/CMakeLists.txt`
plus `src/ModelBuilder/Mesh.cpp` and `src/ImportExport/ObjImportExport.cpp`. After any change run
`tools/golden` on Linux and Windows.
