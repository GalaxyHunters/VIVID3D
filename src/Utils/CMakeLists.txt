# Logging, progress bar, constrained numbers, noise.
target_sources(vivid PRIVATE
        ConstrainedNumbers.cpp
        Noisify.cpp
        Logging/ProgressBar.cpp
        Logging/LogFile.cpp)
target_include_directories(vivid PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Logging)
