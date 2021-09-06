#include "PointCloud.h"

using namespace vivid;
using namespace std;

void CPointCloud::AddPoints(const std::vector<CPoint> &arPoints, vector<coord_t> &arQuan)
{
    coord_t v_max = *std::max_element(arQuan.begin(), arQuan.end());
    coord_t v_min = *std::min_element(arQuan.begin(), arQuan.end());
    coord_t divide_by = 1. / (v_max - v_min);
    for (size_t i = 0; i < arPoints.size(); i++){
        coord_t quan = (arQuan[i] - v_min) * divide_by;
        mFaces.push_back(CFace({i}, quan));
    }
    mPoints.insert(mPoints.end(), arPoints.begin(), arPoints.end());
}