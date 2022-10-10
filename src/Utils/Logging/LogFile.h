#ifndef VIVID_LOGFILE_H
#define VIVID_LOGFILE_H

#pragma once
#include <iomanip>
#include <iostream>
#include <ctime>
#include <sstream>
#include <map>
#include <fstream>
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
     * @param[in] aLogLevel weighted value for smoothing, recommended range 0.2<a<0.8
     * @param[in] aWriteToFile select whether to write to file
     * @param[in] arLogFileName custom file to write to. Default file is "./VIVID_LOG.txt"
     */
    void ConfigLogging(bool aWriteToConsole, ELogCode aLogLevel, bool aWriteToFile, const string& arLogFileName);
}
#endif //VIVID_LOGFILE_H