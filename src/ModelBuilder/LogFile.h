#ifndef VIVID_LOGFILE_H
#define VIVID_LOGFILE_H

#pragma once
#include <ctime>
#include <map>

namespace vivid {

class CLogFile
{
protected:
    CLogFile() {};
    CLogFile(CLogFile const&);
    void operator=(CLogFile const&);

    std::ofstream LOG_FILE;

public:

    static CLogFile &GetInstance()
    {
        static CLogFile singleton;
        return singleton;
    }

    //TODO: These enum variables aren't to conventions, but they are more like global warnings/messages so I think its fine.
    enum class ELogCode
    {
        LOG_ERROR=0, LOG_WARNING=1, LOG_INFO=2
    };

    const std::map<ELogCode, std::string> CodeMap { {ELogCode::LOG_ERROR,"ERROR: "},
                                                    {ELogCode::LOG_WARNING,"WARNING: "},
                                                    {ELogCode::LOG_INFO,"INFO: "},
                                                    };

    enum class ELogType
    {
        ARRAYS_NOT_EQUAL=0, ARRAYS_EMPTY=1, MISSING_BOOLEAN_VALUES=2,
        INVALID_SMOOTH_FACTOR=3, INVALID_ALPHA_VALUE=4
    };

    const std::map<ELogType, std::string> TypeMap { {ELogType::ARRAYS_NOT_EQUAL,"ValueError - Input vectors have not the same size"},
                                                    {ELogType::ARRAYS_EMPTY,"ValueError - Input vectors are empty"},
                                                    {ELogType::MISSING_BOOLEAN_VALUES,"ValueError - Mask must contain both true and false values" },
                                                    {ELogType::INVALID_SMOOTH_FACTOR,"ValueError - Smooth Factor must be between 1 and 8"},
                                                    {ELogType::INVALID_ALPHA_VALUE,"ValueError - Alpha must be between 1 and 8"},
                                                    };


    typedef std::function<void (const ELogType aType)> (LogCallBackFunction);

    void WriteToLog(const ELogType aType) //TODO Use ELogCode + ELogMessage depending on LogCode.
    {
        std::string msg = GetInstance().TypeMap.find(aType)->second;
        std::cerr << msg << std::endl;
        if (!GetInstance().LOG_FILE.is_open()) {
            time_t now = time(0);
            std::string file_path = "./VIVID_LOG";
            file_path.append(std::ctime(&now));
            GetInstance().LOG_FILE.open(file_path + ".txt");
        }
        GetInstance().LOG_FILE << msg << '\n';
    }
};

}// namespace vivid
#endif //VIVID_LOGFILE_H
