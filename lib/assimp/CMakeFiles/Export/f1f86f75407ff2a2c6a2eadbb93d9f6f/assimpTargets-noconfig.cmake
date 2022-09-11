#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "assimp::zlibstatic" for configuration ""
set_property(TARGET assimp::zlibstatic APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(assimp::zlibstatic PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libzlibstatic.a"
  )

list(APPEND _cmake_import_check_targets assimp::zlibstatic )
list(APPEND _cmake_import_check_files_for_assimp::zlibstatic "${_IMPORT_PREFIX}/lib/libzlibstatic.a" )

# Import target "assimp::assimp" for configuration ""
set_property(TARGET assimp::assimp APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(assimp::assimp PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libassimp.so.5.2.4"
  IMPORTED_SONAME_NOCONFIG "libassimp.so.5"
  )

list(APPEND _cmake_import_check_targets assimp::assimp )
list(APPEND _cmake_import_check_files_for_assimp::assimp "${_IMPORT_PREFIX}/lib/libassimp.so.5.2.4" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
