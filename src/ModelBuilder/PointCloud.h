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
    private:

    public:
        //Constructor and Copy Constructor
        CPointCloud(){}
        /**
         * CSurface Constructor
         * @param[in] arInputPoints the input point data in x,y,z form.
         * @param[in] aAlpha the transparency value
         * @param[in] arColorField a vector containing the color field of each point
         * @param[in] aVMin the minimum value in arColorField, anything below will be set to aVMin
         * @param[in] aVMin the maximum value in arColorField, anything below will be set to aVMax
         * @param[in] arLabel the label of the point cloud
         */
        CPointCloud(const std::vector<CPoint> &arPoints, vector<coord_t> &arColorField, coord_t aVMin, coord_t aVMax, const coord_t aAlpha = 1, const std::string arLabel = "VIVID_POINT_CLOUD")
        : CModelComponent(aAlpha, arLabel, "p") {
            mPoints = arPoints;
            coord_t v_max = *std::max_element(arColorField.begin(), arColorField.end());
            coord_t v_min = *std::min_element(arColorField.begin(), arColorField.end());
            coord_t divide_by = 1. / (v_max - v_min);
            for (size_t i = 0; i < arPoints.size(); i++) {
                coord_t color = (arColorField[i] - v_min) * divide_by;
                mFaces.push_back(CFace({i}, color));
            }
        }
        CPointCloud(const CPointCloud &arPC) : CModelComponent(arPC) {}
        ~CPointCloud() {}

        // Operator=
        inline CPointCloud& operator= (const CPointCloud& arPC) { CModelComponent::operator=(arPC); return *this; }

        // Add
        void AddPoints(const std::vector<CPoint> &arPoints, vector<coord_t> &arColorField, coord_t aVMin, coord_t aVMax);

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