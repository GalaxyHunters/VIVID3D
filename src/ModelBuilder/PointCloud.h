#ifndef VIVID_POINTCLOUD_H
#define VIVID_POINTCLOUD_H

#include "ModelComponent.h"
#include <vector>
#include <algorithm>
#include "Point.h"
#include "Face.h"

namespace vivid
{

/* 3D points class*/
class CPointCloud : public CModelComponent{
private:

public:
    //Constructor and Copy Constructor
    CPointCloud(){}
    CPointCloud(const std::vector<CPoint> &arPoints, const coord_t aAlpha, vector<coord_t> &arQuan, const std::string arLabel) : CModelComponent(aAlpha, arLabel, "p"){
        mPoints = arPoints;
        coord_t v_max = *std::max_element(arQuan.begin(), arQuan.end());
        coord_t v_min = *std::min_element(arQuan.begin(), arQuan.end());
        coord_t divide_by = 1. / (v_max - v_min);
        for (size_t i = 0; i < arPoints.size(); i++){
            coord_t quan = (arQuan[i] - v_min) * divide_by;
            mFaces.push_back(CFace({i}, quan));
        }
    }
    CPointCloud(const CPointCloud &arPC) : CModelComponent(arPC){}
    ~CPointCloud(){}

    // Operator=
    inline CPointCloud& operator= (const CPointCloud& arPC) { mPoints = arPC.mPoints; CModelComponent::operator=(arPC); return *this; }

    // Add
    void AddPoints(const std::vector<CPoint> &arPoints, vector<coord_t> &arQuan);
};

}; // namespace vivid
#endif //VIVID_POINTCLOUD_H
