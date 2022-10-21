#include "LogFile.h"

#ifdef PYTHON
#include <pybind11/pybind11.h>
namespace py = pybind11;
#endif

using namespace std;

namespace vivid {
    namespace {
        ofstream mLogFile;
        string mFilePath = "VIVID_LOG.txt";
        ELogCode mLogLevel = LOG_VIVID;
        bool mWriteToLog = false;
        bool mWriteToConsole = true;

        void WriteLog(const ELogCode aCode, const string& arMsg) {
            if (aCode <= mLogLevel) {
                string code = (*CODE_MAP.find(aCode)).second;
                time_t now = time(nullptr);
                auto tm = *localtime(&now);

                ostringstream oss;
                oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
                auto time_string = oss.str();

                auto log_string = time_string + " - " + code + arMsg;
                if (mWriteToConsole) {
#ifdef PYTHON
                    py::print(log_string);
#else
                    if (aCode == ELogCode::LOG_ERROR) {
                        cerr << log_string << endl;
                    } else {
                        cout << log_string << endl;
                    }
#endif
                }
                if (mWriteToLog) {
                    mLogFile.open(mFilePath, ios::app);
                    mLogFile << log_string << '\n';
                    mLogFile.close();
                }
                if (aCode == ELogCode::LOG_ERROR) {
                    throw runtime_error(arMsg);
                }
            }
        }
    }

    void Log(const ELogCode aCode, const ELogMessage aMsg) {
        const string& msg = ERROR_TYPE_STRING.at(aMsg);
        return WriteLog(aCode, msg);
    }
    void Log(const ELogCode aCode, const string& arMsg) {
        return WriteLog(aCode, arMsg);
    }

    void ConfigLogging(bool aWriteToConsole, const ELogCode aLogLevel, bool aWriteToFile, const string& arLogFileName) {
        mWriteToConsole = aWriteToConsole;
        mWriteToLog = aWriteToFile;
        mFilePath = arLogFileName;
        mLogLevel = aLogLevel;
    }
}