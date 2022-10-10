#include "PointCloud.h"
#include "VoronoiVolume.h"

using namespace vivid;
using namespace std;

CPointCloud::CPointCloud(const std::vector<CPoint> &arPoints, const std::string& arColor, const normal_float aOpacity, const std::string& arLabel)
    : CModelComponent(aOpacity, arLabel, POINTS)
{
    mPoints = arPoints;
    SetColor(arColor);
    for (size_t i = 0; i < arPoints.size(); i++){
        mFaces.push_back(CFace({i}, 0));
    }
}

CPointCloud::CPointCloud(const std::vector<CPoint> &arPoints, vector<normal_float> &arQuan, normal_float aFieldMin, normal_float aFieldMax, const normal_float aOpacity, const std::string& arLabel)
    : CModelComponent(aOpacity, arLabel, POINTS)
{
    mPoints = arPoints;
    arQuan = NormalizeField(arQuan, arPoints.size(), aFieldMin, aFieldMax);
    for (size_t i = 0; i < arPoints.size(); i++){
        mFaces.push_back(CFace({i}, arQuan[i]));
    }
}

void CPointCloud::AddPoints(const std::vector<CPoint> &arPoints, vector<normal_float> &arColorField, normal_float aFieldMin, normal_float aFieldMax)
{
    arColorField = NormalizeField(arColorField, arPoints.size(), aFieldMin, aFieldMax);

    size_t size = mPoints.size();

    for (size_t i = 0; i < arPoints.size(); i++){
        mFaces.push_back(CFace({size+i}, arColorField[i]));
    }
    mPoints.insert(mPoints.end(), arPoints.begin(), arPoints.end());
}

CVoronoiVolume CPointCloud::CreateVoronoiVolume(coord_t aNoiseDisplacement) {
    vector<normal_float> UV_coords;
    for (auto & face : mFaces) {
        UV_coords.push_back(face.GetUVcoord());
    }
    CVoronoiVolume volume = CVoronoiVolume(mPoints, UV_coords, 0., 1., aNoiseDisplacement);
    volume.CreateSurface();
    return volume;
}