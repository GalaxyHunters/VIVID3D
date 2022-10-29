#ifndef VIVID_POINTCLOUD_H
#define VIVID_POINTCLOUD_H

#include "ModelComponent.h"
#include <vector>
#include <algorithm>
#include "VoronoiVolume.h"
#include "Noisify.h"

namespace vivid
{

/* 3D points class*/
    class CPointCloud : public CModelComponent{
    public:
        //Constructor and Copy Constructor
        CPointCloud(normal_float aOpacity = 1.0, const std::string &arLabel = "") : CModelComponent(aOpacity, arLabel, POINTS) {}
        /**
         * CVoronoiVolume Constructor
         * @param[in] arInputPoints the input point data in x,y,z form.
         * @param[in] aAlpha the transparency value
         * @param[in] arColorField a vector containing the color field of each point
         * @param[in] aVMin the minimum value in arColorField, anything below will be set to aVMin
         * @param[in] aVMin the maximum value in arColorField, anything below will be set to aVMax
         * @param[in] arLabel the label of the point cloud
         */
        CPointCloud(const std::vector<CPoint> &arPoints, const std::string& arColor = "white", normal_float aOpacity = 1, const std::string &arLabel = "");
        CPointCloud(const std::vector<CPoint> &arPoints, vector<normal_float> &arColorField, normal_float aFieldMin = 0,
                    normal_float aFieldMax = 0, normal_float aOpacity = 1, const std::string& arLabel = "");
        CPointCloud(const CPointCloud &arPC) : CModelComponent(arPC){}
        ~CPointCloud(){}
        

        // Operator=
        inline CPointCloud& operator= (const CPointCloud& arPC) { CModelComponent::operator=(arPC); return *this; }

        // Add
        void AddPoints(const std::vector<CPoint> &arPoints, vector<normal_float> &arColorField, normal_float aFieldMin, normal_float aFieldMax);
        inline void Noisify(coord_t aMaxDisplacement) { mPoints = NoisifyPoints(mPoints, aMaxDisplacement); }
        /**
         * CreateSurface using Voronoi algorithm
         * @param[in] arMask a boolean mask of true and false points, used for generating the boundary surface
         * @param[in] aNoiseDisplacement the Voronoi algorithm struggles with equidistant point data, a small noise displacement improves algorithm speed
         * @returns CVoronoiVolume Voronoi Volume
         */
        CVoronoiVolume CreateVoronoiVolume(coord_t aNoiseDisplacement = 0);

        // CreateSurface using Voronoi algorithm and analysis for masking.
        //CMesh CreateVoronoiVolume();
    };

}; // namespace vivid
#endif //VIVID_POINTCLOUD_H