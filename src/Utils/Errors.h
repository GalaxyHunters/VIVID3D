#ifndef VIVID_ERRORS_H
#define VIVID_ERRORS_H

// TODO Maybe namespace vivid

enum ELogCode
{
    LOG_ERROR=0, LOG_WARNING=1, LOG_INFO=2, LOG_VIVID=3
};

const std::map<ELogCode, std::string> CODE_MAP { {ELogCode::LOG_ERROR,"ERROR: "},
                                                 {ELogCode::LOG_WARNING,"WARNING: "},
                                                 {ELogCode::LOG_INFO,"INFO: "},
                                                 {ELogCode::LOG_VIVID, "VIVID: "},
};

enum ELogMessage
{
    ARRAYS_NOT_EQUAL=0, ARRAYS_EMPTY=1, MISSING_BOOLEAN_VALUES=2,
    INVALID_SMOOTH_FACTOR=3, INVALID_ALPHA_VALUE=4
};

//typedef std::function<void (const ELogCode aCode, const ELogMessage aMsg)> (reportCB_t);

const std::map<ELogMessage, std::string> ERROR_TYPE_STRING {  {ELogMessage::ARRAYS_NOT_EQUAL,
                                                                      "ValueError - Input vectors have not the same size"},
                                                              {ELogMessage::ARRAYS_EMPTY,
                                                                      "ValueError - Input vectors are empty"},
                                                              {ELogMessage::MISSING_BOOLEAN_VALUES,
                                                                      "ValueError - Mask must contain both true and false values" },
                                                              {ELogMessage::INVALID_SMOOTH_FACTOR,
                                                                      "ValueError - Smooth Factor must be between 1 and 8"},
                                                              {ELogMessage::INVALID_ALPHA_VALUE,
                                                                      "ValueError - Alpha must be between 0 and 1"},
};


#endif //VIVID_ERRORS_H