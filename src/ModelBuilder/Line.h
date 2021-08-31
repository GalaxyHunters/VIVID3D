#ifndef VIVID_LINE_H
#define VIVID_LINE_H

#include "ModelComponent.h"

#include <vector>
#include "Point.h"


namespace vivid
{

/* 3D Line class */
class CLine : public CModelComponent{
private:

public:
    //Constructor and Copy Constructor
    CLine(){}
    CLine(const std::vector<CPoint> &arPoints, const coord_t aAlpha, const std::string &arLabel) : CModelComponent(aAlpha, arLabel, "l", "White"){
        mPoints = arPoints;
        std::vector<size_t> points (arPoints.size());
        for (int i = 0; i < arPoints.size(); i++) { points[i] = i; }
        mFaces = {CIndexedFace(points, 0)};
    }
    CLine(const std::vector<std::vector<CPoint>> &arPoints, const coord_t aAlpha, const std::string &arLabel) : CModelComponent(aAlpha, arLabel, "l", "White"){
        size_t size = 0;
        for (auto it = arPoints.begin(); it != arPoints.end(); it++) {
            std::vector<size_t> points (it->size());
            for (size_t j = 0; j < it->size(); j++) {
                points[j] = size + j;
                mPoints.push_back(it->at(j));
            }
            mFaces.push_back(CIndexedFace(points, 0));
            size = size + it->size();
        }
    }
    CLine(const CLine &arLine) : CModelComponent(arLine) {}
    ~CLine(){}

    // Operator=
    inline CLine& operator= (const CLine& arLine) { CModelComponent::operator=(arLine); return *this; }

    void AddLine(const std::vector<CPoint> &arPoints);
};

}; // namespace vivid
#endif //VIVID_LINE_H
