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
    void Log(const ELogCode aCode, const ELogMessage aMsg);
    void Log(const ELogCode aCode, const string& arMsg);

    void SetWriteToLog(bool aWrite);
    void SetWriteToConsole(bool aWrite);
}
#endif //VIVID_LOGFILE_H