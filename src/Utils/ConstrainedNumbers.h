#ifndef VIVID_CONSTRAINEDNUMBERS_H
#define VIVID_CONSTRAINEDNUMBERS_H

#include <vector>
#include "Logging/LogFile.h"

typedef float normal_float;
typedef float positive_float;

constexpr float MIN_NORMAL_VAL = 0.0;
constexpr float MAX_NORMAL_VAL = 1.0;

namespace vivid {
    inline normal_float Normalize(const normal_float aValue) {
        return max(MIN_NORMAL_VAL, min(aValue, MAX_NORMAL_VAL));
    }
    inline normal_float AssertNormalize(const normal_float aValue) {
        if (aValue > MAX_NORMAL_VAL || aValue < MIN_NORMAL_VAL){
            Log(LOG_WARNING, INVALID_NORMAL_VALUE);
        }
        return Normalize(aValue);
    };
    std::vector<normal_float> NormalizeField(std::vector<normal_float>& arField, size_t aArraySize, float aMin = 0, float aMax = 0);
	inline positive_float AssertPositive(const positive_float aValue) {
		if (aValue < MIN_NORMAL_VAL) {
            Log(LOG_WARNING, INVALID_POSITIVE_VALUE);
        }
        return max(MIN_NORMAL_VAL, aValue);
	}
}

#endif //VIVID_CONSTRAINEDNUMBERS_H
