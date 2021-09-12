#include "PointCloud.h"
#include "Surface.h"

using namespace vivid;
using namespace std;

void CPointCloud::AddPoints(const std::vector<CPoint> &arPoints, vector<quan_t> &arQuan)
{
    quan_t v_max = *std::max_element(arQuan.begin(), arQuan.end());
    quan_t v_min = *std::min_element(arQuan.begin(), arQuan.end());

    size_t size = mPoints.size();

    coord_t divide_by = 1. / (v_max - v_min);
    for (size_t i = 0; i < arPoints.size(); i++){
        quan_t quan = (arQuan[i] - v_min) * divide_by;
        mFaces.push_back(CFace({size+i}, quan));
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
    return surface.ToMesh(mLabel, mAlpha);
}