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
        /**
         * CSurface Constructor
         * @param[in] arInputPoints the input point data in x,y,z form.
         * @param[in] aAlpha the transparency value
         * @param[in] arColorField a vector containing the color field of each point
         * @param[in] aVMin the minimum value in arColorField, anything below will be set to aVMin
         * @param[in] aVMin the maximum value in arColorField, anything below will be set to aVMax
         * @param[in] arLabel the label of the point cloud
         */
        CPointCloud(const std::vector<CPoint> &arPoints, const std::string& arColor, normal_float aOpacity, const std::string& arLabel);
        CPointCloud(const std::vector<CPoint> &arPoints, vector<normal_float> &arColorField, normal_float aFieldMin, normal_float aFieldMax, normal_float aOpacity, const std::string& arLabel);
        CPointCloud(const CPointCloud &arPC) : CModelComponent(arPC){}
        ~CPointCloud(){}
        

        // Operator=
        inline CPointCloud& operator= (const CPointCloud& arPC) { CModelComponent::operator=(arPC); return *this; }

        // Add
        void AddPoints(const std::vector<CPoint> &arPoints, vector<normal_float> &arColorField, normal_float aFieldMin, normal_float aFieldMax);

        /**
         * CreateSurface using Voronoi algorithm
         * @param[in] arMask a boolean mask of true and false points, used for generating the boundary surface
         * @param[in] aNoiseDisplacement the Voronoi algorithm struggles with equidistant point data, a small noise displacement improves algorithm speed
         * @returns CMesh Voronoi Surface
         */
        CMesh CreateVoronoiSurface(const std::vector<bool>& arMask, coord_t aNoiseDisplacement = 0);

        // CreateSurface using Voronoi algorithm and analysis for masking.
        //CMesh CreateVoronoiSurface();
    };

}; // namespace vivid
#endif //VIVID_POINTCLOUD_H