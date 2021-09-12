#include "Line.h"

using namespace vivid;
using namespace std;

void CLines::AddLine(const vector<CPoint> &arPoints)
{
    size_t size = mPoints.size();
    vector<size_t> points (arPoints.size());
    for (size_t i = 0; i < arPoints.size(); i++)
    {
        mPoints.push_back(arPoints[i]);
        points[i] = size + i;
    }
    mFaces.push_back(CFace(points, 0));
}

void CLines::AddLines(const vector<vector<CPoint>> &arPointsMatrix) {
    for (auto & points : arPointsMatrix) {
        AddLine(points);
    }
}