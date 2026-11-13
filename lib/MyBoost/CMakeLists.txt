# A copy of the parts of Boost that the Voronoi library and a few Vivid sources use (headers only).
# Everything links the target `Boost::headers`; when Boost is taken from the system instead, this
# directory is replaced by find_package(Boost) and nothing else changes.
add_library(vivid_boost INTERFACE)
target_include_directories(vivid_boost SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(Boost::headers ALIAS vivid_boost)
