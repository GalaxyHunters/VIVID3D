#include "PointCloud.h"
#include "Surface.h"

using namespace vivid;
using namespace std;

void CPointCloud::AddPoints(const std::vector<CPoint> &arPoints, vector<coord_t> &arColorField, coord_t aVMin, coord_t aVMax)
{
    aVMax = *std::max_element(arColorField.begin(), arColorField.end());
    aVMin = *std::min_element(arColorField.begin(), arColorField.end());

    size_t size = mPoints.size();

    coord_t divide_by = 1. / (aVMax - aVMin);
    for (size_t i = 0; i < arPoints.size(); i++){
        coord_t UV_coord = (arColorField[i] - aVMin) * divide_by;
        mFaces.push_back(CFace({size+i}, UV_coord));
    }
    mPoints.insert(mPoints.end(), arPoints.begin(), arPoints.end());
}

CMesh CPointCloud::CreateVoronoiSurface(const std::vector<bool>& arMask, coord_t aNoiseDisplacement) {
    vector<coord_t> UV_coords;
    for (auto & face : mFaces) {
        UV_coords.push_back(face.GetUVcoord());
    }
    CSurface surface = CSurface(mPoints, arMask, UV_coords, 0., 1., aNoiseDisplacement);
    surface.CreateSurface();
    return surface.ToMesh(mLabel, mAlpha);
}