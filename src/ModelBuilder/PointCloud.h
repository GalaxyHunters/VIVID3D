#ifndef VIVID_POINTCLOUD_H
#define VIVID_POINTCLOUD_H

#include "ModelComponent.h"
#include <vector>
#include "Point.h"

namespace vivid
{

/* 3D points class*/
class PointCloud : public CModelComponent{
private:
    /* mPoints - Points in the 3D space */
    std::vector<CPoint> mPoints = {};
public:
    //Constructor and Copy Constructor
    PointCloud(){}
    PointCloud(std::vector<CPoint> &aPoints) : mPoints(aPoints){}
    PointCloud(const PointCloud &aPC) : mPoints(aPC.mPoints) {}

    // Set and Get
    inline const std::vector<CPoint> Points() const{ return mPoints; }
    inline void Points(std::vector<CPoint> &aPoints) { mPoints = aPoints; }

    // Add
    inline void AddPoints(std::vector<CPoint> &aPoints) { mPoints.insert(mPoints.end(), aPoints.begin(), aPoints.end()); }
};

}; // namespace vivid
#endif //VIVID_POINTCLOUD_H
