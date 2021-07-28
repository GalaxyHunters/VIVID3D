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
    CPointCloud(const std::vector<CPoint> &arPoints, const coord_t aAlpha, const std::string arLabel) : mPoints(arPoints), CModelComponent(aAlpha, arLabel){}
    CPointCloud(const CPointCloud &arPC) : mPoints(arPC.mPoints), CModelComponent(arPC){}
    ~CPointCloud(){}

    // Operator=
    inline CPointCloud& operator= (const CPointCloud& arPC) { mPoints = arPC.mPoints; CModelComponent::operator=(arPC); return *this; }

    // Set and Get
    inline const std::vector<CPoint> GetPoints() const{ return mPoints; }
    inline void SetPoints(const std::vector<CPoint> &arPoints) { mPoints = arPoints; }

    // Add
    inline void AddPoints(const std::vector<CPoint> &arPoints) { mPoints.insert(mPoints.end(), arPoints.begin(), arPoints.end()); }
};

}; // namespace vivid
#endif //VIVID_POINTCLOUD_H
