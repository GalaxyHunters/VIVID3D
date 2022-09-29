#include <algorithm>
#include "NormalFloat.h"

using namespace std;

namespace vivid {
    vector<normal_float> NormalizeField(vector<normal_float>& arField, size_t aArraySize, float aMin, float aMax) {
        if (arField.empty()) { // Default field from python wrapper
            arField = vector<normal_float>(aArraySize, 0);
            return arField;
        }
        if(aMin == 0 && aMax == 0) { // default values
            aMax = *max_element(arField.begin(), arField.end());
            aMin = *min_element(arField.begin(), arField.end());
        }
        if (aMin == aMax) { // all elements in field are identical
            arField = vector<normal_float>(arField.size(), 0);
            return arField;
        }
        normal_float divide_by = 1.0f / (aMax - aMin);
        for (float & it : arField) {
            it = (it - aMin) * divide_by;
            if (it > 1) it = 1;
            if (it < 0.01) it = 0.01;
        }
        return arField;
    }
}