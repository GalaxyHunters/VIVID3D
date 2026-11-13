# lodepng: PNG encoder/decoder (third-party, a single self-contained source file).
add_library(lodepng STATIC lodepng.cpp)
target_include_directories(lodepng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
