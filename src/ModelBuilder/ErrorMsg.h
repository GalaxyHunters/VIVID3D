#ifndef VIVID_ERRORMSG_H
#define VIVID_ERRORMSG_H

#pragma once
#include <ctime>

namespace vivid {

static std::ofstream LOG_FILE; // TODO: read about static global parameter and why it is consoder bad practice

enum class e_LogType // TODO: convention...
{
    ARRAYS_NOT_EQUAL, ARRAYS_EMPTY, MISSING_BOOLEAN_VALUES, INVALID_SMOOTH_FACTOR, INVALID_ALPHA_VALUE //TODO: initialize the enum so the values would remain even when changing the order
};

typedef void (*LogCallBackFunction)(const e_LogType &a); //TODO: use functional, this way is a c way not cpp
    
//TODO: use constexpr map of enum and errmsg    

static std::string GetFromEnum(const e_LogType &a) //TODO should be map
{
    std::string msg;
    switch (a) {
        case e_LogType::ARRAYS_NOT_EQUAL:
            msg = "ValueError - Input vectors have not the same size";
            break;
        case e_LogType::ARRAYS_EMPTY:
            msg = "ValueError - Input vectors are empty";
            break;
        case e_LogType::MISSING_BOOLEAN_VALUES:
            msg = "ValueError - Mask must contain both true and false values";
            break;
        case e_LogType::INVALID_SMOOTH_FACTOR:
            msg = "ValueError - Smooth Factor must be between 1 and 8";
            break;
        case e_LogType::INVALID_ALPHA_VALUE:
            msg = "ValueError - Alpha must be between 0 and 1";
            break;
        default:
            msg = "Unknown Error Code";
            break;
    }
    return msg;
}

static void WriteToLog(const e_LogType &a) //TODO shouldn't be static, may get next to the enum also some parameter or two of the report (if we need one)
{
    std::string msg = GetFromEnum(a);
    std::cerr << msg << std::endl;
    if (!LOG_FILE.is_open()) {
        time_t now = time(0);
        std::string file_path = "./VIVID_LOG";
        file_path.append(std::ctime(&now));
        LOG_FILE.open(file_path + ".txt");
    }
    LOG_FILE << msg << '\n';
}

}// namespace vivid
#endif //VIVID_ERRORMSG_H
