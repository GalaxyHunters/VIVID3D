#include "PointCloud.h"
#include "Surface.h"

using namespace vivid;
using namespace std;

CPointCloud::CPointCloud(const std::vector<CPoint> &arPoints, const std::string* arColor, const normal_float aOpacity, const std::string& arLabel)
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

void CPointCloud::AddPoints(const std::vector<CPoint> &arPoints, vector<normal_float> &arQuan, normal_float aFieldMin, normal_float aFieldMax)
{
    arQuan = NormalizeField(arQuan, arPoints.size(), aFieldMin, aFieldMax);

    size_t size = mPoints.size();

    for (size_t i = 0; i < arPoints.size(); i++){
        mFaces.push_back(CFace({size+i}, arQuan[i]));
    }
    mPoints.insert(mPoints.end(), arPoints.begin(), arPoints.end());
}

CMesh CPointCloud::CreateVoronoiSurface(vector<bool> aMask) {
    vector<quan_t> quan;
    for (auto & face : mFaces) {
        quan.push_back(face.GetQuan());
    }
    CSurface surface = CSurface(mPoints, aMask, quan, 0., 1.);
    surface.CreateSurface();
    return surface.ToMesh(mLabel, mMaterial.GetOpacity());
}