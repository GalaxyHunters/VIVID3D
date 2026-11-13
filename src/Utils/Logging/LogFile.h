#ifndef VIVID_LOGFILE_H
#define VIVID_LOGFILE_H

#pragma once
#include <iomanip>
#include <iostream>
#include <ctime>
#include <sstream>
#include <map>
#include <fstream>
#include <functional>
#include "Errors.h"

using namespace std;

namespace vivid {
    /**
     * Smooth faces on CMesh by Laplacian Smooth
     * @param[in] aCode LOG_ERROR | LOG_WARNING | LOG_VIVID | LOG_INFO | LOG_DEBUG
     * @param[in] aMsg predefined message code
     */
    void Log(ELogCode aCode, ELogMessage aMsg);
    /**
     * Log a custom message to the VIVID Logger
     * @param[in] aCode LOG_ERROR | LOG_WARNING | LOG_VIVID | LOG_INFO | LOG_DEBUG
     * @param[in] arMsg custom message string
     */
    void Log(ELogCode aCode, const string& arMsg);

    /**
     * Configure VIVID in-built Logging Process
     * @param[in] aWriteToConsole select whether to print to console
     * @param[in] aLogLevel Values below this will not be logged. Default is LOG_VIVID
     * @param[in] aWriteToFile select whether to write to file
     * @param[in] arLogFileName custom file to write to. Default file is "./VIVID_LOG.txt"
     */
    void ConfigLogging(bool aWriteToConsole, ELogCode aLogLevel, bool aWriteToFile, const string& arLogFileName);

    /** Receives each fully formatted console log line (timestamp, level and message). */
    using ConsoleSink = std::function<void(const string&)>;

    /**
     * Redirect console output to a custom sink. The Python bindings use this to print through
     * Python's sys.stdout (so notebooks and redirects capture it), which keeps this library free
     * of any Python dependency. Pass an empty function to restore the default std::cout / std::cerr.
     */
    void SetConsoleSink(ConsoleSink aSink);
}
#endif //VIVID_LOGFILE_H