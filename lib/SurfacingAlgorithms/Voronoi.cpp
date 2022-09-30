#include "Voronoi.h"

using namespace vivid;
using namespace std;

namespace vivid
{
    vector<Vector3D> CVoronoi::ConvertToVorn(const vector<CPoint>& arInputPoints) {
        vector<Vector3D> vorn_points;
        for (const auto & point : arInputPoints) {
            vorn_points.push_back(Vector3D(point.X(), point.Y(), point.Z()));
        }
        return vorn_points;
    }

    void CVoronoi::ComputeVoronoi(const vector<CPoint>& arInputPoints, pair<CPoint,CPoint> Box) {
        vector<Vector3D> vorn_points = ConvertToVorn(arInputPoints);
        mData.SetBox({Box.first.X(), Box.first.Y(), Box.first.Z() }, {Box.second.X(), Box.second.Y(), Box.second.Z() });
        mData.Build(vorn_points);
    }

    vector<vector<size_t>> CVoronoi::GetFaces() {
        vector<vector<size_t> > faces;
        size_t total_cells = mData.GetAllCellFaces().size();
        face_vec cell;
        point_vec face_points;
        size_t c_point1, c_point2;
        for (size_t i = 0; i < total_cells; i++) {
            cell = mData.GetCellFaces(i);
            for (auto face = cell.begin(); face != cell.end(); face++) {
                c_point1 = get<0>(mData.GetFaceNeighbors(*face));
                c_point2 = get<1>(mData.GetFaceNeighbors(*face));
                if (!(c_point1 < i) && !(c_point2 < i)) { //the face doent belong to a cell we read already
                    faces.push_back(vector<size_t>());
                    face_points = mData.GetPointsInFace(*face);
                    for (auto & face_point : face_points) {
                        faces.back().push_back(face_point);
                    }
                    faces.back().push_back(c_point1);
                    faces.back().push_back(c_point2);
                }
            }
        }

        return faces;
    }

} // namespace vivid