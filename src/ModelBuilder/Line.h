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
    /* mPoints - Points in which the line pass through */
    std::vector<CPoint> mPoints = {};
public:
    //Constructor and Copy Constructor
    CLine(){}
    CLine(const std::vector<CPoint> &arPoints, const coord_t aAlpha, const std::string &arLabel) : mPoints(arPoints), CModelComponent(aAlpha, arLabel){}
    CLine(const CLine &arLine) : mPoints(arLine.mPoints), CModelComponent(arLine) {}
    ~CLine(){}

    // Operator=
    inline CLine& operator= (const CLine& arLine) { mPoints = arLine.mPoints; CModelComponent::operator=(arLine); return *this; }

    // Set and Get
    inline const std::vector<CPoint> GetPoints() const{ return mPoints; }
    inline void SetPoints(const std::vector<CPoint> &arPoints) { mPoints = arPoints; }
};

}; // namespace vivid
#endif //VIVID_LINE_H
