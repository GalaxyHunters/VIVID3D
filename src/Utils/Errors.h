#ifndef VIVID_ERRORS_H
#define VIVID_ERRORS_H

namespace vivid {
    enum ELogCode {
        LOG_ERROR = 0, LOG_WARNING = 1, LOG_VIVID = 2, LOG_INFO = 3, LOG_DEBUG = 4
    };

    const std::map<ELogCode, std::string> CODE_MAP{{LOG_ERROR,   "ERROR:   "},
                                                   {LOG_WARNING, "WARNING: "},
                                                   {LOG_INFO,    "INFO:    "},
                                                   {LOG_DEBUG,   "INFO:    "},
                                                   {LOG_VIVID,   "VIVID:   "},
    };

    enum ELogMessage {
        ARRAYS_NOT_EQUAL = 0, ARRAYS_EMPTY = 1, MISSING_BOOLEAN_VALUES = 2,
        INVALID_SMOOTH_FACTOR = 3, INVALID_ALPHA_VALUE = 4
    };

    const std::map<ELogMessage, std::string> ERROR_TYPE_STRING{{ARRAYS_NOT_EQUAL,
                                                                       "ValueError - Input vectors have not the same size"},
                                                               {ARRAYS_EMPTY,
                                                                       "ValueError - Input vectors are empty"},
                                                               {MISSING_BOOLEAN_VALUES,
                                                                       "ValueError - Mask must contain both true and false values"},
                                                               {INVALID_SMOOTH_FACTOR,
                                                                       "ValueError - Smooth Factor must be between 1 and 8"},
                                                               {INVALID_ALPHA_VALUE,
                                                                       "ValueError - Alpha must be between 0 and 1"},
    };
}

#endif //VIVID_ERRORS_H