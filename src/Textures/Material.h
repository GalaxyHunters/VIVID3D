#ifndef VIVID_MATERIAL_H
#define VIVID_MATERIAL_H

#include "NormalFloat.h"
#include "ColorMap.h"

using namespace std;

namespace vivid {
    class CMaterial {
    private:
        normal_float mOpacity = 1;
        normal_float mRoughness = 0;
        float mEmissionStrength = 0;
        color_t mEmissionColor = BLACK;
    public:
        CMaterial() {}
        CMaterial(normal_float aOpacity, normal_float aRoughness, float aEmissionStrength, const color_t& arEmissionColor)
                : mOpacity(AssertNormalize(aOpacity)), mRoughness(AssertNormalize(aRoughness)), mEmissionStrength(aEmissionStrength), mEmissionColor(arEmissionColor) {}

        inline normal_float GetOpacity() const { return mOpacity; }
        inline normal_float GetRoughness() const { return mRoughness; }
        inline float GetEmissionStrength() const { return mEmissionStrength; }
        inline color_t GetEmissionColor() const { return mEmissionColor; }

        inline void SetOpacity(normal_float aOpacity) { mOpacity = AssertNormalize((aOpacity)); }
        inline void SetRoughness(normal_float aRoughness) { mRoughness = AssertNormalize((aRoughness)); }
        inline void SetEmissionStrength(float aEmissionStrength) { mEmissionStrength = aEmissionStrength; }
        inline void SetEmissionColor(const color_t& arEmissionColor) { mEmissionColor = arEmissionColor; }
        inline void SetEmissionColor(const string& arEmissionColor) { mEmissionColor = COLORS.at(arEmissionColor); }

    };
} // vivid

#endif //VIVID_MATERIAL_H
