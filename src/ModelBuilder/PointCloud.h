#ifndef VIVID_POINTCLOUD_H
#define VIVID_POINTCLOUD_H

#include "ModelComponent.h"
#include <vector>
#include "Point.h"

namespace vivid
{

/* 3D points class*/
class PointCloud : public CModelComponent{
    /* mPoints - Points in the 3D space */
    std::vector<CPoint> mPoints = {};

    // set, get, add?
};

}; // namespace vivid
#endif //VIVID_POINTCLOUD_H
