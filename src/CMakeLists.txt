# Vivid's own code. Every directory adds its sources and include path to the one `vivid` target
# (defined in the top-level CMakeLists.txt), so a new source file is listed next to the file itself.
add_subdirectory(ModelBuilder)
add_subdirectory(ImportExport)
add_subdirectory(Textures)
add_subdirectory(Utils)
add_subdirectory(Extra)

# the Python extension module
if(VIVID_BUILD_PYTHON)
    add_subdirectory(Wrapping)
endif()
