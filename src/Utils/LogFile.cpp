#include "LogFile.h"

using namespace std;

namespace vivid {
    namespace {
        ofstream LOG_FILE;
        bool mWriteToLog = true;
        bool mWriteToConsole = true;
        string mFilePath = "VIVID_LOG.txt";

        void WriteLog(const ELogCode aCode, const string& arMsg) {
            string code = (*CODE_MAP.find(aCode)).second;
            time_t now = time(nullptr);
            auto tm = *localtime(&now);

            ostringstream oss;
            oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
            auto time_string = oss.str();

            auto log_string = time_string + " - " + code + arMsg;
            if (mWriteToConsole) {
                if (aCode == ELogCode::LOG_ERROR) {
                    cerr << log_string << endl;
                } else {
                    cout << log_string << endl;
                }
            }
            if (mWriteToLog) {
                LOG_FILE.open(mFilePath, ios::app);
                LOG_FILE << log_string << '\n';
                LOG_FILE.close();
            }
            if (aCode == ELogCode::LOG_ERROR) {
                throw runtime_error(arMsg);
            }
        }
    }

    void Log(const ELogCode aCode, const ELogMessage aMsg) {
        string msg = (*ERROR_TYPE_STRING.find(aMsg)).second;
        return WriteLog(aCode, msg);
    }
    void Log(const ELogCode aCode, const string& arMsg) {
        return WriteLog(aCode, arMsg);
    }

    void SetFileToLog(bool aWrite) {
        mWriteToLog = true;
    }
    void SetWriteToConsole(bool aWrite) {
        mWriteToConsole = aWrite;
    }
}