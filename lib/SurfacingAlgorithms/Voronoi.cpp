#include "Voronoi.h"

using namespace vivid;
using namespace std;

constexpr coord_t BOX_EXPAND_FACTOR = 1;
constexpr coord_t PARTICLE_SCALE_MAGNITUDE = 100; // TODO: Is this the right scale (-100, 100)?
constexpr coord_t NOISE_PERCENTAGE = 0.01; // TODO: Is this the right scale (-100, 100)?

namespace vivid
{

// TODO: Scale should probably be seperate, but I don't know so for now its like this
vector<CPoint> CVoronoi::PreProcessPoints(const vector<CPoint> &arPoints) {
    vector<CPoint> box_dimensions = FindContainingBox(arPoints);
    CPoint box_min = box_dimensions.at(1); CPoint box_max = box_dimensions.at(2);
    CPoint center_vec = (box_min + box_max) / 2;
    coord_t re_scale_val = FindContainingRadius(arPoints) / PARTICLE_SCALE_MAGNITUDE;
    vector<CPoint> noise_vec (arPoints.size());
    generate(noise_vec.begin(), noise_vec.end(), CPoint(NOISE_PERCENTAGE*((2 * center_vec.X() * rand() / RAND_MAX) - center_vec.X()),
                                                        NOISE_PERCENTAGE*((2 * center_vec.Y() * rand() / RAND_MAX) - center_vec.Z()),
                                                        NOISE_PERCENTAGE*((2 * center_vec.Z() * rand() / RAND_MAX) - center_vec.Z())   ));
    // Scales and adds noise to points
    vector<CPoint> points (arPoints.size());
    // TODO: theres got to be a better way of doing this.
    for (int i = 0; i < points.size(); i++){
        points[i] = (arPoints[i] - center_vec + noise_vec[i]) / re_scale_val;
    }
    return points;
}

void ConvertToVorn(vector<CPoint>& arInputPoints, vector<Vector3D>& arNewPoints) {
	for (auto it = arInputPoints.begin(); it != arInputPoints.end(); it++) {
		arNewPoints.push_back(Vector3D(it->X(), it->Y(), it->Z()));
	}
}

pair<vector<Vector3D>, vector<vector<size_t>>>  CVoronoi::MosheVoronoi(const vector<CPoint>& arInputPoints) {
    // Preprocess all points
    vector<CPoint> points = PreProcessPoints(arInputPoints);
    // Find Box
    CPoint box_dim, box_min, box_max;
    vector<CPoint> box_dimensions = FindContainingBox(points);
    box_dim = box_dimensions.at(0); box_min = box_dimensions.at(1), box_max = box_dimensions.at(0);
    pair<CPoint,CPoint> box_pair (box_min-box_dim*BOX_EXPAND_FACTOR, box_max+box_dim*BOX_EXPAND_FACTOR);
    // Preprocessing

    // Convert points to Vector3D
    vector<Vector3D> vorn_points;
    ConvertToVorn(arInputPoints, vorn_points); //TODO: single line
    // set box
    vector<Vector3D> box = vector<Vector3D>({{box_pair.first.X(), box_pair.first.Y(), box_pair.first.Z() },
                                             {box_pair.second.X(), box_pair.second.Y(), box_pair.second.Z() }});
    mData.SetBox(box.at(0), box.at(1));
    mData.Build(vorn_points);
}

pair<vector<Vector3D>, vector<vector<size_t> > > CVoronoi::ComputeVoronoi(vector<CPoint>& arInputPoints, pair<CPoint,CPoint> Box) {
    vector<Vector3D> vorn_points;
    ConvertToVorn(arInputPoints, vorn_points);
    vector<Vector3D> box = vector<Vector3D>({{Box.first.X(), Box.first.Y(), Box.first.Z() }, {Box.second.X(), Box.second.Y(), Box.second.Z() }});
    mData.SetBox(box.at(0), box.at(1));
    mData.Build(vorn_points);

    vorn_points = mData.GetFacePoints();

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

// TODO: Make these into one normal function
double CVoronoi::FindContainingRadius(const vector<CPoint>& arPoints){
    return (max_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Magnitude() < arV2.Magnitude();}))->Magnitude();
}
vector<CPoint> CVoronoi::FindContainingBox(const vector<CPoint>& arPoints){
    coord_t x_max = (max_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.X() < arV2.X();}))->X();
    coord_t x_min = (min_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.X() < arV2.X();}))->X();
    coord_t y_max = (max_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Y() < arV2.Y();}))->Y();
    coord_t y_min = (min_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Y() < arV2.Y();}))->Y();
    coord_t z_max = (max_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Z() < arV2.Z();}))->Z();
    coord_t z_min = (min_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Z() < arV2.Z();}))->Z();

    CPoint box_dim = CPoint(x_max-x_min, y_max-y_min, z_max-z_min); //box dimensions
    CPoint box_min = CPoint(x_min, y_min, z_min);        //box min
    CPoint box_max = CPoint(x_max, y_max, z_max);        //box max
    //pair<CPoint,CPoint>(box_min-box_dim*BOX_EXPAND_FACTOR, box_max+box_dim*BOX_EXPAND_FACTOR)
    return vector<CPoint>{box_dim, box_min, box_max};
}


} // namespace vivid