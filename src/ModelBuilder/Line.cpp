#include "Line.h"

using namespace vivid;
using namespace std;

void CLine::AddLine(const vector<CPoint> &arPoints)
{
    size_t size = mPoints.size();
    vector<size_t> points (arPoints.size());
    for (size_t i = 0; i < arPoints.size(); i++)
    {
        mPoints.push_back(arPoints[i]);
        points[i] = size + i;
    }
    mFaces.push_back(CIndexedFace(points, 0));
}