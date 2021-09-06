#ifndef VIVID_LINE_H
#define VIVID_LINE_H

#include "ModelComponent.h"

#include <vector>
#include "Point.h"


namespace vivid
{

/* 3D Line class */
class CLines : public CModelComponent{
private:

public:
    //Constructor and Copy Constructor
    CLines(){}
    CLines(const std::vector<CPoint> &arPoints, const coord_t aAlpha, const std::string &arLabel) : CModelComponent(aAlpha, arLabel, "l", "White"){
        mPoints = arPoints;
        std::vector<size_t> points (arPoints.size());
        for (int i = 0; i < arPoints.size(); i++) { points[i] = i; }
        mFaces = {CFace(points, 0)};
    }
    CLines(const std::vector<std::vector<CPoint>> &arPoints, const coord_t aAlpha, const std::string &arLabel) : CModelComponent(aAlpha, arLabel, "l", "White"){
        size_t size = 0;
        for (auto it = arPoints.begin(); it != arPoints.end(); it++) {
            std::vector<size_t> points (it->size());
            for (size_t j = 0; j < it->size(); j++) {
                points[j] = size + j;
                mPoints.push_back(it->at(j));
            }
            mFaces.push_back(CFace(points, 0));
            size = size + it->size();
        }
    }
    CLines(const CLines &arLine) : CModelComponent(arLine) {}
    ~CLines(){}

    // Operator=
    inline CLines& operator= (const CLines& arLine) { CModelComponent::operator=(arLine); return *this; }

    void AddLine(const std::vector<CPoint> &arPoints);
};

}; // namespace vivid
#endif //VIVID_LINE_H
