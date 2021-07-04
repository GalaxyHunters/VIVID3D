#include "Voronoi.h"

using namespace vivid;
using namespace std;


namespace vivid
{

void ConvertToVorn(std::vector<CPoint>& arInputPoints, std::vector<Vector3D>& arNewPoints) {
	for (auto it = arInputPoints.begin(); it != arInputPoints.end(); it++) {
		arNewPoints.push_back(Vector3D(it->X(), it->Y(), it->Z()));
	}
}

// TODO: Alt compute_vornoi func

std::pair<std::vector<Vector3D>, std::vector<std::vector<size_t> > > compute_vornoi(std::vector<CPoint>& arInputPoints, double aBoxR) {
    std::vector<Vector3D> vorn_points;
	vivid::ConvertToVorn(arInputPoints, vorn_points);
	Voronoi3D temp(Vector3D(-aBoxR, -aBoxR, -aBoxR), Vector3D(aBoxR, aBoxR, aBoxR));
	temp.Build(vorn_points);
	vorn_points = temp.GetFacePoints();

	vector<vector<size_t> > faces;
	size_t total_cells = temp.GetAllCellFaces().size();
	face_vec cell;
	point_vec face_points;
	size_t c_point1, c_point2;
	for (size_t i = 0; i < total_cells; i++) {
		cell = temp.GetCellFaces(i);
		for (auto face = cell.begin(); face != cell.end(); face++) {
			c_point1 = get<0>(temp.GetFaceNeighbors(*face));
			c_point2 = get<1>(temp.GetFaceNeighbors(*face));
			if (!(c_point1 < i) && !(c_point2 < i)) { //the face doent belong to a cell we read already
				faces.push_back(vector<size_t>());
				face_points = temp.GetPointsInFace(*face);
				for (point_vec::iterator point = face_points.begin(); point != face_points.end(); point++) {
					faces.back().push_back(*point);
				}
				faces.back().push_back(c_point1);
				faces.back().push_back(c_point2);
			}
		}
	}
    std::pair<std::vector<Vector3D>, std::vector<vector<size_t> > > output(vorn_points, faces); //TODO ....
	return output;
}
// TODO: ADAM is it yours?
pair<vector<Vector3D>, vector<vector<size_t> > > compute_vornoi(vector<CPoint>& arInputPoints, vector<Vector3D> Box) {
    vector<Vector3D> vorn_points;
    ConvertToVorn(arInputPoints, vorn_points);
    Voronoi3D temp(Box.at(0), Box.at(1));
    temp.Build(vorn_points);
    vorn_points = temp.GetFacePoints();

    vector<vector<size_t> > faces;
    size_t total_cells = temp.GetAllCellFaces().size();
    face_vec cell;
    point_vec face_points;
    size_t c_point1, c_point2;
    for (size_t i = 0; i < total_cells; i++) {
        cell = temp.GetCellFaces(i);
        for (auto face = cell.begin(); face != cell.end(); face++) {
            c_point1 = get<0>(temp.GetFaceNeighbors(*face));
            c_point2 = get<1>(temp.GetFaceNeighbors(*face));
            if (!(c_point1 < i) && !(c_point2 < i)) { //the face doent belong to a cell we read already
                faces.push_back(vector<size_t>());
                face_points = temp.GetPointsInFace(*face);
                for (point_vec::iterator point = face_points.begin(); point != face_points.end(); point++) {
                    faces.back().push_back(*point);
                }
                faces.back().push_back(c_point1);
                faces.back().push_back(c_point2);
            }
        }
    }
    pair<vector<Vector3D>, vector<vector<size_t> > > output(vorn_points, faces);
    return output;
}


} // namespace vivid