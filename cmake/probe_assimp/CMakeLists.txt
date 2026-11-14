# Finds an installed assimp and records only its version. Runs in its own directory scope so the
# imported targets created by find_package() do not leak into the main project (see VividAssimp.cmake).
find_package(assimp CONFIG QUIET)
if(assimp_FOUND)
    set(VIVID_ASSIMP_PROBED_VERSION "${assimp_VERSION}" CACHE INTERNAL "Version of the installed assimp, if any")
else()
    unset(VIVID_ASSIMP_PROBED_VERSION CACHE)
endif()
