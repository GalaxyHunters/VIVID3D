#ifndef VIVID_MATERIAL_H
#define VIVID_MATERIAL_H

#include "ConstrainedNumbers.h"
#include "ColorMap.h"

using namespace std;

namespace vivid {
    class CMaterial {
    private:
        string mLabel;
        normal_float mOpacity;
        normal_float mRoughness;
        normal_float mMetalness;
        positive_float mEmissionStrength ;
    public:
        CMaterial(normal_float aOpacity = 1.0, normal_float aMetalness = 0.5, normal_float aRoughness = 0.5, positive_float aEmissionStrength = 0.0, const string& arLabel = "defaultMaterial")
                : mOpacity(AssertNormalize(aOpacity)), mMetalness(AssertNormalize(aMetalness)), mRoughness(AssertNormalize(aRoughness)), mEmissionStrength(AssertPositive(aEmissionStrength)), mLabel(arLabel) {}

        inline string GetLabel() const { return mLabel; }
        inline normal_float GetOpacity() const { return mOpacity; }
        inline normal_float GetRoughness() const { return mRoughness; }
        inline normal_float GetMetalness() const { return mMetalness; }
        inline positive_float GetEmissionStrength() const { return mEmissionStrength; }

        inline void SetLabel(const string& arLabel) { mLabel = arLabel; }
        inline void SetOpacity(normal_float aOpacity) { mOpacity = AssertNormalize((aOpacity)); }
        inline void SetRoughness(normal_float aRoughness) { mRoughness = AssertNormalize((aRoughness)); }
        inline void SetMetalness(normal_float aMetalness) { mMetalness = AssertNormalize((aMetalness)); }
        inline void SetEmissionStrength(positive_float aEmissionStrength) { mEmissionStrength = AssertPositive(aEmissionStrength); }
    };
} // vivid

#endif //VIVID_MATERIAL_H
