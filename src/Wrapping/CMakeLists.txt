# The `_core` Python extension module (pybind11).
#
# pybind11 is an external build dependency (pyproject.toml: build-system.requires). Ask the interpreter
# that is building where its pybind11 lives, so this works with any build frontend.
set(PYBIND11_FINDPYTHON ON)
find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
execute_process(
    COMMAND "${Python_EXECUTABLE}" -m pybind11 --cmakedir
    OUTPUT_VARIABLE _vivid_pybind11_dir OUTPUT_STRIP_TRAILING_WHITESPACE RESULT_VARIABLE _vivid_pybind11_rc)
if(_vivid_pybind11_rc EQUAL 0)
    list(APPEND CMAKE_PREFIX_PATH "${_vivid_pybind11_dir}")
endif()
set(VIVID_PYBIND11_MIN_VERSION "3.0.1" CACHE STRING "Oldest pybind11 accepted (tested with tools/golden)")
find_package(pybind11 ${VIVID_PYBIND11_MIN_VERSION} CONFIG REQUIRED)
message(STATUS "pybind11: ${pybind11_VERSION} (${pybind11_DIR})")

# The core library is a static library linked into this one binary; it has no Python dependency of its
# own. The module hooks Python's stdout into the library's logger at import (see wrapper.cpp).
pybind11_add_module(_core wrapper.cpp)
target_link_libraries(_core PRIVATE vivid)

# Belt and braces for third-party C code that marks its symbols visible explicitly (zlib does): tell the
# linker not to export anything that comes from a static archive.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_options(_core PRIVATE -Wl,--exclude-libs,ALL)
endif()

# scikit-build-core installs into the wheel's vivid3d/ package (tool.scikit-build.wheel.install-dir)
install(TARGETS _core DESTINATION .)
