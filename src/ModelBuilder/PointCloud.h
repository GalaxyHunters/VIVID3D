#ifndef VIVID_POINTCLOUD_H
#define VIVID_POINTCLOUD_H

#include "ModelComponent.h"
#include <vector>
#include <algorithm>
#include "Point.h"
#include "IndexedFace.h"

namespace vivid
{

/* 3D points class*/
class CPointCloud : public CModelComponent{
private:
    /* mPoints - Points in the 3D space */
    std::vector<CPoint> mPoints = {};
    std::vector<CIndexedFace> mFaces ={};
public:
    //Constructor and Copy Constructor
    CPointCloud(){}
    CPointCloud(const std::vector<CPoint> &arPoints, const coord_t aAlpha, vector<quan_t> &arQuan, const std::string arLabel) : mPoints(arPoints), CModelComponent(aAlpha, arLabel){
        quan_t v_max = *std::max_element(arQuan.begin(), arQuan.end());
        quan_t v_min = *std::min_element(arQuan.begin(), arQuan.end());
        quan_t divide_by = 1./(v_max - v_min);
        for (size_t i = 0; i < arPoints.size(); i++){
            quan_t quan = (arQuan[i] - v_min) * divide_by;
            mFaces.push_back(CIndexedFace({i}, quan));
        }
    }
    CPointCloud(const CPointCloud &arPC) : mPoints(arPC.mPoints), mFaces(arPC.mFaces), CModelComponent(arPC){}
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
