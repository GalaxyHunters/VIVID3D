#ifndef COLORMAP
#define COLORMAP

#include <vector>
#include "Plasma.h"
#include "ColorDefs.h"

using namespace std;

namespace vivid {
    class CColorMap {
    private:
        vector<color_t> mClm{};
        string mName = "";

        string ToLower(const string &arStr) {
            string new_str;
            for (char i : arStr) {
                if (i != ' ') {
                    new_str += tolower(i);
                }
            }
            return new_str;
        }

        vector<color_t> ToColor_TClm(const vector<CColor> &arClm) {
            vector<color_t> color_t_clm;
            for (auto color : arClm) {
                color_t_clm.push_back(color.ToColor_T());
            }
            return color_t_clm;
        }

    public:
        CColorMap() : mClm(PLASMA_CLM.begin(), PLASMA_CLM.end()), mName(PLASMA) {}

        CColorMap(const vector<color_t> &arClm, const string &arCName) : mClm(arClm), mName(arCName) {}
        CColorMap(const vector<CColor> &arClm, const string &arCName) : mClm(ToColor_TClm(arClm)), mName(arCName) {}

        CColorMap(const string &arCName) : mClm({COLORS.at(ToLower(arCName))}), mName(ToLower(arCName)) {}

        inline CColorMap &operator=(const CColorMap &arClm) {
            mClm = arClm.mClm;
            mName = arClm.mName;
            return *this;
        }

        inline string GetName() const { return mName; }

        inline int GetColorIndex(normal_float aVal) const {
            if (mClm.empty()) {
                return 0;
            }
            return floor(aVal * (double(mClm.size()) - 1)); //returns the index of the color in ClmPlasma
        }

        inline color_t GetColor(normal_float aVal) const { return mClm[GetColorIndex(aVal)]; }

        inline vector<rgb_t> GetColorTexture() const
        {
            vector<rgb_t> texture;

            for (const auto &rgb: mClm) {
                texture.push_back(rgb[0]);
                texture.push_back(rgb[1]);
                texture.push_back(rgb[2]);
                texture.push_back(RGB_MAX);
            }
            texture.resize(texture.size());
            return texture;
        }
    };
}
#endif