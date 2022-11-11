#include "LogFile.h"
#include <vector>
#include <array>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#define PYBIND11_DETAILED_ERROR_MESSAGES

using namespace vivid;
using namespace std;

namespace py = pybind11;
using namespace py::literals;

void defineLoggingSubmodule(const py::module& m) {
    py::enum_<ELogCode>(m, "LogCode")
        .value("LOG_ERROR", LOG_ERROR)
        .value("LOG_WARNING", LOG_WARNING)
        .value("LOG_VIVID", LOG_VIVID)
        .value("LOG_INFO", LOG_INFO)
        .value("LOG_DEBUG", LOG_DEBUG)
        .export_values();

    m.def("config_logger", &ConfigLogging, R"mydelimiter(
Configure the VIVID3D in-built Logging Process

Parameters
----------
write_to_console : bool, default: True
    Print all Logging output to console.
log_level : vivid3d.LogCode, default: LOG_VIVID
    Log all messages above or at this log level.
write_to_file : bool, default: false
    Write all Logging output to file.
log_file_label : str, default: 'VIVID_LOG.txt'
    File name for Logging.

        )mydelimiter", py::arg("write_to_console") = true, py::arg("log_level") = LOG_VIVID, 
            py::arg("write_to_file") = false, py::arg("log_file_name") = "VIVID_LOG.txt");
}