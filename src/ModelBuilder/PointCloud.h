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
        CPointCloud() : CModelComponent(POINTS) {}
        CPointCloud(const std::vector<CPoint> &arPoints, const std::string* arColor, normal_float aOpacity, const std::string& arLabel);
        CPointCloud(const std::vector<CPoint> &arPoints, vector<normal_float> &arQuan, normal_float aFieldMin, normal_float aFieldMax, normal_float aOpacity, const std::string& arLabel);
        CPointCloud(const CPointCloud &arPC) : CModelComponent(arPC){}
        ~CPointCloud(){}

        // Operator=
        inline CPointCloud& operator= (const CPointCloud& arPC) { CModelComponent::operator=(arPC); return *this; }

        // Add
        void AddPoints(const std::vector<CPoint> &arPoints, vector<normal_float> &arQuan, normal_float aFieldMin, normal_float aFieldMax);

        // CreateSurface using Voronoi algorithm
        CMesh CreateVoronoiSurface(std::vector<bool> aMask);

        // CreateSurface using Voronoi algorithm and analysis for masking.
        //CMesh CreateVoronoiSurface();
    };

}; // namespace vivid
#endif //VIVID_POINTCLOUD_H