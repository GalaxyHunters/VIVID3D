#ifndef COLORMAP
#define COLORMAP

#include <vector>
#include <iostream>
#include <map>
#include "Point.h"
#include "Plasma.h"
#include "ColorDefs.h"

using namespace std;

class CColorMap
{
private:
    vector<color_t> Clm {};
    string ClmName = "";

    static string ToLower(string &arStr) {
        for (int i = 0; i < arStr.length(); i++) {
            arStr[i] = tolower(arStr[i]);
        }
        return arStr;
    }

public:
    CColorMap() : Clm(GetPlasma()), ClmName("Plasma") {}
    CColorMap(const vector<color_t> &arClm, const string &arCName) : Clm(arClm), ClmName(arCName) {}
    CColorMap(string &arCName) : Clm(GetColorByString(ToLower(arCName))), ClmName(ToLower(arCName)) {}
    inline CColorMap& operator= (const CColorMap& arClm) { Clm=arClm.Clm; ClmName=arClm.ClmName; return *this; }

    void SetColorMap(const vector<color_t> &arClm, const string &arCName) {
        Clm = arClm; ClmName = arCName;
    }

    void SetColorMap(string &arCName) {
        Clm = GetColorByString(ToLower(arCName));
        ClmName = ToLower(arCName);
    }

    inline string GetName() const { return ClmName; }

    int GetColorIndex(quan_t aVal) const {
        if (Clm.empty()){
            return 0;
        }
        return floor (aVal * (double(Clm.size())-1)); //returns the index of the color in ClmPlasma
    }

    inline color_t GetColor(quan_t aVal) const { return Clm[GetColorIndex(aVal)]; }

    vector<unsigned char> GetColorTexture() const //float aColorMap
    {
        vector<unsigned char> texture;

        for(const auto & it : Clm){
            texture.push_back((unsigned char) (it.R*RGB_MAX) );
            texture.push_back((unsigned char) (it.G*RGB_MAX) );
            texture.push_back((unsigned char) (it.B*RGB_MAX) );
            texture.push_back((unsigned char) (RGB_MAX));
        }
        texture.resize(texture.size());
        return texture;
    }

};

#endif