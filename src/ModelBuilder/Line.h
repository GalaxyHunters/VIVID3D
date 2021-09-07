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
        for (const auto & arPoint : arPoints) {
            std::vector<size_t> points (arPoint.size());
            for (size_t j = 0; j < arPoint.size(); j++) {
                points[j] = size + j;
                mPoints.push_back(arPoint.at(j));
            }
            mFaces.push_back(CFace(points, 0));
            size = size + arPoint.size();
        }
    }
    CLines(const CLines &arLine) : CModelComponent(arLine) {}
    ~CLines(){}

    // Operator=
    inline CLines& operator= (const CLines& arLine) { CModelComponent::operator=(arLine); return *this; }

    void AddLine(const std::vector<CPoint> &arPoints);
    void AddLines(const std::vector<std::vector<CPoint>> &arPoints);
};

}; // namespace vivid
#endif //VIVID_LINE_H
