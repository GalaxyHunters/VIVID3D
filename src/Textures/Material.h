#ifndef VIVID_MATERIAL_H
#define VIVID_MATERIAL_H

#include "NormalFloat.h"
#include "ColorMap.h"

using namespace std;

namespace vivid {
    class CMaterial {
    private:
        string mLabel = "defaultMaterial";
        normal_float mOpacity = 1.0;
        normal_float mRoughness = 0.1;
        float mEmissionStrength = 0;
        string mEmissionColor = "black";
    public:
        CMaterial() {}
        CMaterial(normal_float aOpacity, normal_float aRoughness, float aEmissionStrength, const string& arEmissionColor, const string& arLabel)
                : mOpacity(AssertNormalize(aOpacity)), mRoughness(AssertNormalize(aRoughness)), mEmissionStrength(aEmissionStrength),  mLabel(arLabel) {
            try {
                COLORS.at(arEmissionColor); // will error out if not exists
                mEmissionColor = arEmissionColor;
            } catch (const std::exception &e) {
                Log(LOG_ERROR, "Unsupported Color");
            }
        }

        inline string GetLabel() const { return mLabel; }
        inline normal_float GetOpacity() const { return mOpacity; }
        inline normal_float GetShininess() const { return mRoughness; }
        inline normal_float GetEmissionStrength() const { return mEmissionStrength; }
        inline string GetEmissionColor() const { return mEmissionColor; }

        inline void SetLabel(const string& arLabel) { mLabel = arLabel; }
        inline void SetOpacity(normal_float aOpacity) { mOpacity = AssertNormalize((aOpacity)); }
        inline void SetShininess(normal_float aRoughness) { mRoughness = AssertNormalize((aRoughness)); }
        inline void SetEmissionStrength(normal_float aEmissionStrength) { mEmissionStrength = aEmissionStrength; }
//        inline void SetEmissionColor(const color_t& arEmissionColor) { mEmissionColor = arEmissionColor; }
        inline void SetEmissionColor(const string& arEmissionColor) {
            try {
                COLORS.at(arEmissionColor); // will error out if not exists
                mEmissionColor = arEmissionColor;
            } catch (const std::exception& e) {
                Log(LOG_ERROR, "Unsupported Color");
            }
        }
    };
} // vivid

#endif //VIVID_MATERIAL_H
