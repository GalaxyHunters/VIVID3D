#ifndef VIVID_POINTCLOUD_H
#define VIVID_POINTCLOUD_H

#include "ModelComponent.h"
#include <vector>
#include <algorithm>
#include "Mesh.h"

namespace vivid
{

/* 3D points class*/
    class CPointCloud : public CModelComponent{
    public:
        //Constructor and Copy Constructor
        CPointCloud(){}
        CPointCloud(const std::vector<CPoint> &arPoints, const normal_float aOpacity, vector<normal_float> &arQuan, const std::string& arLabel) : CModelComponent(aOpacity, arLabel, "p"){
            mPoints = arPoints;
            normal_float v_max = *std::max_element(arQuan.begin(), arQuan.end());
            normal_float v_min = *std::min_element(arQuan.begin(), arQuan.end());
            normal_float divide_by = (float)1. / (v_max - v_min);
            for (size_t i = 0; i < arPoints.size(); i++){
                normal_float quan = (arQuan[i] - v_min) * divide_by;
                mFaces.push_back(CFace({i}, quan));
            }
        }
        CPointCloud(const CPointCloud &arPC) : CModelComponent(arPC){}
        ~CPointCloud(){}

        // Operator=
        inline CPointCloud& operator= (const CPointCloud& arPC) { CModelComponent::operator=(arPC); return *this; }

        // Add
        void AddPoints(const std::vector<CPoint> &arPoints, vector<normal_float> &arQuan);

        // CreateSurface using Voronoi algorithm
        CMesh CreateVoronoiSurface(std::vector<bool> aMask);

        // CreateSurface using Voronoi algorithm and analysis for masking.
        //CMesh CreateVoronoiSurface();
    };

}; // namespace vivid
#endif //VIVID_POINTCLOUD_H