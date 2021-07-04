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
    CLine(std::vector<CPoint> &aPoints) : mPoints(aPoints){}
    CLine(const CLine &aLine) : mPoints(aLine.mPoints) {}
    ~CLine(){}

    // Operator=
    inline CLine& operator= (const CLine& aLine) { mPoints = aLine.mPoints; return *this; }

    // Set and Get
    inline const std::vector<CPoint> Points() const{ return mPoints; }
    inline void Points(std::vector<CPoint> &aPoints) { mPoints = aPoints; }
};

}; // namespace vivid
#endif //VIVID_LINE_H
