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
        color_t mEmissionColor = Black()[0];
        CColorMap *mTexture = & BASE_TEXTURE;
    public:
        CMaterial() {}
        CMaterial(normal_float aOpacity, normal_float aRoughness, float aEmissionStrength, const color_t& arEmissionColor)
                : mOpacity(AssertNormalize(aOpacity)), mRoughness(AssertNormalize(aRoughness)), mEmissionStrength(aEmissionStrength), mEmissionColor(arEmissionColor) {}

        inline normal_float GetOpacity() const { return mOpacity; }
        inline normal_float GetRoughness() const { return mRoughness; }
        inline float GetEmissionStrength() const { return mEmissionStrength; }
        inline color_t GetEmissionColor() const { return mEmissionColor; }
        inline CColorMap GetTexture() const { return *mTexture; }

        inline void SetOpacity(normal_float aOpacity) { mOpacity = AssertNormalize((aOpacity)); }
        inline void SetRoughness(normal_float aRoughness) { mRoughness = AssertNormalize((aRoughness)); }
        inline void SetEmissionStrength(float aEmissionStrength) { mEmissionStrength = aEmissionStrength; }
        inline void SetEmissionColor(color_t& arEmissionColor) { mEmissionColor = arEmissionColor; }
        inline void SetTexture(const CColorMap& arTexture) { *mTexture = arTexture; }
    };

    static CMaterial BASE_MAT = CMaterial();
} // vivid

#endif //VIVID_MATERIAL_H
