#ifndef VIVID_LOGFILE_H
#define VIVID_LOGFILE_H

#pragma once
#include <ctime>
#include <map>

namespace vivid { // TODO Shouldn't be in namespace vivid. should be in utils.

class CLogFile
{
protected: // private
    CLogFile() {}; // TODO add here string FileName (with path)
    CLogFile(CLogFile const&);
    void operator=(CLogFile const&);

    std::ofstream LOG_FILE;

public:
    //TODO FLUSH
    //TODO CLOSE? in d~tor
    static CLogFile &GetInstance()
    {
        static CLogFile singleton;
        return singleton;
    }

    // Order in class!!!
    //TODO: These enum variables aren't to conventions, but they are more like global warnings/messages so I think its fine.
    enum ELogCode // I think there should be enum type too.
    {
        LOG_ERROR=0, LOG_WARNING=1, LOG_INFO=2
    };

    const std::map<ELogCode, std::string> CodeMap { {ELogCode::LOG_ERROR,"ERROR: "}, //TODO Define at the begining and as const_expr. By conventions CodeMap -> CODE_MAP and ERROR_TYPE_STRING is a better name
                                                    {ELogCode::LOG_WARNING,"WARNING: "},
                                                    {ELogCode::LOG_INFO,"INFO: "},
                                                    };
    // TODO Errors.h
    // TODO in Erros.h: typedef CLogFile::LogCallBackFunction reportCB_t
    enum ELogMessage //Same here
    {
        ARRAYS_NOT_EQUAL=0, ARRAYS_EMPTY=1, MISSING_BOOLEAN_VALUES=2,
        INVALID_SMOOTH_FACTOR=3, INVALID_ALPHA_VALUE=4
    };

    const std::map<ELogMessage, std::string> TypeMap {{ELogMessage::ARRAYS_NOT_EQUAL,       "ValueError - Input vectors have not the same size"}, //Same here
                                                      {ELogMessage::ARRAYS_EMPTY,           "ValueError - Input vectors are empty"},
                                                      {ELogMessage::MISSING_BOOLEAN_VALUES, "ValueError - Mask must contain both true and false values" },
                                                      {ELogMessage::INVALID_SMOOTH_FACTOR,  "ValueError - Smooth Factor must be between 1 and 8"},
                                                      {ELogMessage::INVALID_ALPHA_VALUE,    "ValueError - Alpha must be between 1 and 8"},
                                                    };

    typedef std::function<void (const CLogFile::ELogCode aCode, const CLogFile::ELogMessage aMsg)> (LogCallBackFunction); // TODO why defined inside the class? naming andmake sure you use the instance !

    void Write(const CLogFile::ELogCode aCode, const CLogFile::ELogMessage aMsg) //TODO Use ELogCode + ELogMessage depending on LogCode. //TODO get String instead of CLogFile::ELogMessage
    {
        std::string code = GetInstance().CodeMap.find(aCode)->second; //TODO GetInstance
        std::string msg = GetInstance().TypeMap.find(aMsg)->second;
        std::cerr << msg << std::endl;
        if (!GetInstance().LOG_FILE.is_open()) {
            time_t now = time(0);
            std::string file_path = "./VIVID_LOG"; //TODO should be generic
            // file_path.append(std::ctime(&now));
            GetInstance().LOG_FILE.open(file_path + ".txt");
        }
        // TODO: DateTimeString before each line.
        GetInstance().LOG_FILE << code << msg << '\n';
    }
};

}// namespace vivid
#endif //VIVID_LOGFILE_H
