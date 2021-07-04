#ifndef VIVID_POINTCLOUD_H
#define VIVID_POINTCLOUD_H

#include "ModelComponent.h"
#include <vector>
#include "Point.h"

namespace vivid
{

/* 3D points class*/
class CPointCloud : public CModelComponent{
private:
    /* mPoints - Points in the 3D space */
    std::vector<CPoint> mPoints = {};
public:
    //Constructor and Copy Constructor
    CPointCloud(){}
    CPointCloud(std::vector<CPoint> &aPoints) : mPoints(aPoints){}
    CPointCloud(const CPointCloud &aPC) : mPoints(aPC.mPoints) {}
    ~CPointCloud(){}

    // Operator=
    inline CPointCloud& operator= (const CPointCloud& aPC) { mPoints = aPC.mPoints; return *this; }

    // Set and Get
    inline const std::vector<CPoint> Points() const{ return mPoints; }
    inline void Points(std::vector<CPoint> &aPoints) { mPoints = aPoints; }

    // Add
    inline void AddPoints(std::vector<CPoint> &aPoints) { mPoints.insert(mPoints.end(), aPoints.begin(), aPoints.end()); }
};

}; // namespace vivid
#endif //VIVID_POINTCLOUD_H
