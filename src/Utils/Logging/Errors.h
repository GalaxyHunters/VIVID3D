#ifndef VIVID_ERRORS_H
#define VIVID_ERRORS_H

namespace vivid {
    enum ELogCode {
        LOG_ERROR = 0, LOG_WARNING = 1, LOG_VIVID = 2, LOG_INFO = 3, LOG_DEBUG = 4
    };

    const std::map<ELogCode, std::string> CODE_MAP{{LOG_ERROR,   "ERROR:   "},
                                                   {LOG_WARNING, "WARNING: "},
                                                   {LOG_VIVID,   "VIVID:   "},
                                                   {LOG_INFO,    "INFO:    "},
                                                   {LOG_DEBUG,   "DEBUG:   "},
    };

    enum ELogMessage {
        ARRAYS_NOT_EQUAL = 0, ARRAYS_EMPTY = 1, MISSING_BOOLEAN_VALUES = 2, SURFACE_THRESHOLD_OUT_OF_RANGE = 3,
        INVALID_SMOOTH_FACTOR = 4, INVALID_ALPHA_VALUE=5, INVALID_NORMAL_VALUE=6, INVALID_FRAME_VALUE=7
    };

    const std::map<ELogMessage, std::string> ERROR_TYPE_STRING{{ARRAYS_NOT_EQUAL,
                                                                       "ValueError - Input vectors have not the same size"},
                                                               {ARRAYS_EMPTY,
                                                                       "ValueError - Input vectors are empty"},
                                                               {MISSING_BOOLEAN_VALUES,
                                                                       "ValueError - Mask must contain both true and false values"},
                                                               {SURFACE_THRESHOLD_OUT_OF_RANGE,
                                                                       "ValueError - SurfaceThreshold must be within SurfaceField Range"},
                                                               {INVALID_SMOOTH_FACTOR,
                                                                       "ValueError - Smooth Factor must be between 1 and 8"},
                                                               {INVALID_ALPHA_VALUE,
                                                                       "ValueError - Alpha must be between 0 and 1"},
                                                               {INVALID_NORMAL_VALUE,
                                                                       "ValueError - Normal Value must be between 0.0 and 1.0. Rounding to the nearest possible value"},
                                                               {INVALID_FRAME_VALUE,
                                                                       "ValueError - frame must be within Models.size range"},
    };
}      //namespace vivid
#endif //VIVID_ERRORS_H