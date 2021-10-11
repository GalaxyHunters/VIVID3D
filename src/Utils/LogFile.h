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

class CLogFile
{
private:
    ofstream LOG_FILE;
    bool mWriteToLog = false;
    bool mPrintToConsole = true;
    int mCountToFlush = 0;
    int mMaxBeforeFlush = 5;
    string mFIlePath = "";
    CLogFile() {};
    CLogFile(CLogFile const&);
    ~CLogFile() {
        LOG_FILE.close();
    }
    void operator=(CLogFile const&);

public:
    //TODO FLUSH
    static CLogFile &GetInstance()
    {
        static CLogFile singleton;
        return singleton;
    }

    void Write(const ELogCode aCode, const ELogMessage aMsg) //TODO Use ELogCode + ELogMessage depending on LogCode. //TODO get String instead of CLogFile::ELogMessage
    {
        mCountToFlush++;
        if (mCountToFlush >= mMaxBeforeFlush) { LOG_FILE.flush(); }
        string code = CODE_MAP.find(aCode)->second;
        string msg = ERROR_TYPE_STRING.find(aMsg)->second;

        time_t now = time(nullptr);
        auto tm = *localtime(&now);

        ostringstream oss;
        oss << put_time(&tm, "%d-%m-%Y %H:%M:%S");
        auto str = oss.str();
        if (mPrintToConsole) {
            if (code == "LOG INFO: "){
                cout << str << endl;
                cout << code << msg << endl;
            } else {
                cerr << str << endl;
                cerr << code << msg << endl;
            }
        }
        if (LOG_FILE.is_open()) {
            string file_path = "./VIVID_LOG"; //TODO should be generic
            LOG_FILE.open(file_path + ".txt");
        }

        LOG_FILE << str << '\n';
        LOG_FILE << code << msg << '\n';
    }

    void WriteCustom(const ELogCode aCode, const string &arMsg)
    {
        mCountToFlush++;
        if (mCountToFlush >= mMaxBeforeFlush) { LOG_FILE.flush(); }

        string code = CODE_MAP.find(aCode)->second;

        time_t now = time(nullptr);
        auto tm = *localtime(&now);

        ostringstream oss;
        oss << put_time(&tm, "%d-%m-%Y %H-%M-%S");
        auto str = oss.str();
        if (LOG_FILE.is_open()) {
            string file_path = "./VIVID_LOG"; //TODO should be generic
            // file_path.append(ctime(&now));
            LOG_FILE.open(file_path + ".txt");
        }
        if (mPrintToConsole) {
            cout << str << endl;
            cout << code << arMsg << endl;
        }
        LOG_FILE << str << '\n';
        LOG_FILE << code << arMsg << '\n';
    }

    void SetWriteToLog(bool aWrite) { mWriteToLog = aWrite; }
    void SetPrintToConsole(bool aWrite) { mPrintToConsole = aWrite; }
};

inline void SetLogFile(bool aWriteToLog, bool aPrintToConsole) {
    CLogFile::GetInstance().SetWriteToLog(aWriteToLog);
    CLogFile::GetInstance().SetPrintToConsole(aPrintToConsole);
}

#endif //VIVID_LOGFILE_H