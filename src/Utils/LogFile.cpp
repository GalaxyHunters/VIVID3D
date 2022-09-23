#include "LogFile.h"

using namespace std;

namespace vivid {
    namespace {
//        ofstream LOG_FILE;
        bool mWriteToLog = false;
        bool mWriteToConsole = true;
//        int mCountToFlush = 0;
//        int mMaxBeforeFlush = 5;
//        string mFilePath = "";

        void WriteLog(const ELogCode aCode, const string& arMsg) {
//            mCountToFlush++;
//            if (mCountToFlush >= mMaxBeforeFlush) { LOG_FILE.flush(); }

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
//                if (LOG_FILE.is_open()) {
//                    string file_path = "./VIVID_LOG"; //TODO should be generic
//                    LOG_FILE.open(file_path + ".txt");
//                }
//
//                LOG_FILE << time_string << '\n';
//                LOG_FILE << code << arMsg << '\n';
            }
            if (aCode == ELogCode::LOG_ERROR) {
                throw runtime_error(arMsg);
            }
//            return 0;
        }
    }

    void Log(const ELogCode aCode, const ELogMessage aMsg) {
        string msg = (*ERROR_TYPE_STRING.find(aMsg)).second;
        return WriteLog(aCode, msg);
    }
    void Log(const ELogCode aCode, const string& arMsg) {
        return WriteLog(aCode, arMsg);
    }

    void SetWriteToLog(bool aWrite) {
        mWriteToLog = aWrite;
    }
    void SetWriteToConsole(bool aWrite) {
        mWriteToConsole = aWrite;
    }
}