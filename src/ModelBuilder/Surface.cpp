#include "Surface.h"
#include <map>

using namespace vivid;
using namespace std;

constexpr coord_t BOX_EXPAND_FACTOR = 1;
constexpr coord_t PARTICLE_SCALE_MAGNITUDE = 1000;
constexpr coord_t NOISE_PERCENTAGE = 0.0001;

CSurface::CSurface(const vector<CPoint> &arInputPoints, const vector<bool> &arMask, vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax, coord_t aNoiseDisplacement)
{
    // Check input validity
    if(arInputPoints.empty() || arInputPoints.empty()) {
        Log(LOG_ERROR, ARRAYS_EMPTY);
    }
    if((arInputPoints.size() != arMask.size()) || !arColorField.empty() && (arInputPoints.size() != arColorField.size())){
        Log(LOG_ERROR, ARRAYS_NOT_EQUAL);
    }
    if( (find(arMask.begin(),arMask.end(),true) == arMask.end()) || find(arMask.begin(), arMask.end(), false) == arMask.end() ){
        Log(LOG_ERROR, MISSING_BOOLEAN_VALUES);
    }
    vector<CSurfacePoint> points;
    vector<normal_float> normal_field = NormalizeField(arColorField, arInputPoints.size(), aVMin, aVMax);
    for (int i = 0; i < arInputPoints.size(); i++) {
        points.push_back(CSurfacePoint(CPoint(arInputPoints[i]), normal_field[i], arMask[i]));
    }
    mInputPoints = arInputPoints;
    PreProcessPoints(points, aNoiseDisplacement);
}

CSurface::CSurface(const CSurface &surf)
{
    mInputPoints = surf.mInputPoints;
    mCreationMask = surf.mCreationMask;
    mUVcoords = surf.mUVcoords;
    mCenVector = surf.mCenVector;
    mBoxPair = surf.mBoxPair;
    mScale = surf.mScale;

    map <shared_ptr<CPoint>, shared_ptr<CPoint>> old_to_new_Points;
    mVertices.clear();
    size_t counter = 0;
    map < shared_ptr<CPoint>, size_t> indexes;
    for (const auto & mVecPoint : surf.mVertices) {
        mVertices.push_back(shared_ptr<CPoint>(new CPoint(*mVecPoint)));
        indexes[mVecPoint] = counter;
        counter++;
    }
    mSurfFaces.clear();
    CSurfaceFace temp;
    for (const auto & mVecFace : surf.mSurfFaces) {
        temp = CSurfaceFace();
        temp.mUVcoord = mVecFace.mUVcoord;
        temp.mPairPoints = mVecFace.mPairPoints;
        temp.mVertices.clear();
        for (const auto & mPoint : mVecFace.mVertices){
            temp.mVertices.push_back(mVertices[indexes[mPoint]]);
        }
        mSurfFaces.push_back(temp);
    }
}

//   ------------------------------------------------ Public Methods ------------------------------------------------>>>
//   <<<--------------------------------------------- Public Methods ---------------------------------------------------

void CSurface::CreateSurface()
{
    RunVorn();
    Log(LOG_VIVID, "Voronoi Post Processing");
    CleanEdges();
    CleanFaces();
    CleanPoints();
}

CMesh CSurface::ToMesh(const string& arLabel, normal_float aOpacity) const {
    vector<CPoint> points;
    size_t counter = 0;
    map < shared_ptr<CPoint>, size_t> indexes;
    for (const auto & mVecPoint : mVertices) {
        //points.push_back(**it);
        points.push_back((*mVecPoint * mScale) + mCenVector);
        indexes[mVecPoint] = counter;
        counter++;
    }
    vector<CFace> faces;
    vector<size_t> face_points;
    set<size_t> set_points;
    map<size_t, vector<size_t>> points_neighbours;
    for (const auto & mVecFace : mSurfFaces) {
        for (const auto & mPoint : mVecFace.mVertices) {
            if (set_points.count(indexes[mPoint]) == 0) {
                face_points.push_back(indexes[mPoint]);
                set_points.insert(indexes[mPoint]);
            }
        }
        faces.push_back(CFace(face_points, mVecFace.mUVcoord));
        face_points = vector<size_t>();
        set_points.clear();
    }
    return CMesh(points, faces, arLabel, aOpacity);
}

/*------------------------------------------------- Private Methods --------------------------------------------------*/

/* ---------------------------------------------- Handle RunVorn Methods ---------------------------------------------*/

vector<shared_ptr<CPoint> > ConvertFromVorn(const vector<Vector3D>& arVornPoints)
{
    vector<shared_ptr<CPoint> > new_vec;
    for (const auto & arVornPoint : arVornPoints) {
        new_vec.push_back(shared_ptr<CPoint>(new CPoint(arVornPoint.x, arVornPoint.y, arVornPoint.z))); //TODO y not copy c~tor?
    }
    return new_vec;
}

void CSurface::RunVorn()
{
    Log(LOG_VIVID, "Begin Compute Voronoi with "  + to_string(mInputPoints.size()) + " points");
    mVoronoi.ComputeVoronoi(mInputPoints, mBoxPair);
    //set the points
    mVertices = ConvertFromVorn(mVoronoi.mData.GetFacePoints());
    //set the faces
    vector<vector<size_t> > vorn_faces = mVoronoi.GetFaces();
    vector<CSurfaceFace> new_faces;
    vector<vector<size_t>> points_map;
    points_map.resize(mVoronoi.mData.GetTotalPointNumber(), vector<size_t>(0));
    size_t c_point1, c_point2;
    normal_float UVcoord;
    vector<shared_ptr<CPoint>> face_points;

    for (auto & vorn_face : vorn_faces) {
        c_point1 = vorn_face.back();
        vorn_face.pop_back();
        c_point2 = vorn_face.back();
        vorn_face.pop_back();
        UVcoord = 0; //UVcoord will be defined later in the program(clean faces) so for now this will be a placeholder
        face_points = vector<shared_ptr<CPoint> >();

        for (size_t & point : vorn_face) {
            face_points.push_back(mVertices[point]);
        }
        new_faces.push_back(CSurfaceFace(face_points, UVcoord, pair<size_t, size_t>(c_point1, c_point2)));
    }

    mSurfFaces = new_faces;

    Log(LOG_VIVID, "Completed Voronoi Computation");
}

/* -------------------------------------------- Centralization Sub-Methods -------------------------------------------*/

vector<CPoint> CSurface::FindContainingBox()
{
    coord_t x_max = (max_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.X() < arV2.X();}))->X();
    coord_t x_min = (min_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.X() < arV2.X();}))->X();
    coord_t y_max = (max_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Y() < arV2.Y();}))->Y();
    coord_t y_min = (min_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Y() < arV2.Y();}))->Y();
    coord_t z_max = (max_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Z() < arV2.Z();}))->Z();
    coord_t z_min = (min_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Z() < arV2.Z();}))->Z();

    CPoint box_dim = CPoint(x_max-x_min, y_max-y_min, z_max-z_min);
    CPoint box_min = CPoint(x_min, y_min, z_min);
    CPoint box_max = CPoint(x_max, y_max, z_max);

    return vector<CPoint>{box_dim, box_min, box_max};
}


coord_t CSurface::FindContainingRadius()
{
    return (max_element(mInputPoints.begin(),mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Magnitude() < arV2.Magnitude();}))->Magnitude();
}

/*----------------------------------------------- Cleaning Sub-Methods -----------------------------------------------*/

// Todo: switch to bool statement, use in CleanFaces
void CSurface::CleanEdges()
{
    coord_t clean_by = 0.35;

    vector<CSurfaceFace> new_faces;
    for (auto & mVecFace : mSurfFaces) {
        bool is_out_of_radius = false;
        for (auto & mPoint : mVecFace.mVertices) {
            if (abs((*mPoint).X()) > (mBoxPair.second.X() * clean_by) || abs((*mPoint).Y()) > (mBoxPair.second.Y() * clean_by) || abs((*mPoint).Z()) > (mBoxPair.second.Z() * clean_by)) {
                is_out_of_radius = true;
            }
        }
        if (!is_out_of_radius) { //the face is inside the box Radius and should be kept
            new_faces.push_back(mVecFace);
        }
    }
    mSurfFaces = new_faces;
}

// takes the data from mVoronoi and using the mask cleans it so that only the wanted surface is left
void CSurface::CleanFaces()
{
    vector<CSurfaceFace> new_faces;
    size_t mask_len = mCreationMask.size();
    for (auto & mVecFace : mSurfFaces) {
        if (mask_len > mVecFace.mPairPoints.first && mask_len > mVecFace.mPairPoints.second) { //the indexs are both in range and not a part of the box
            if (mCreationMask[mVecFace.mPairPoints.first] != mCreationMask[mVecFace.mPairPoints.second]) { //the face is a part of the surf
                vector<CSurfaceFace> triangulized_faces = TriangulizeFace(mVecFace);
                for (auto & mTriangleFace : triangulized_faces) {
                    new_faces.push_back(mTriangleFace);
                }
            }
        }
//        else {
//            new_faces.push_back(mVecFace);
//            new_faces.back().mUVcoord = 0;
//        }

    }
    mSurfFaces = new_faces;
}

vector<CSurfaceFace> CSurface::TriangulizeFace(const CSurfaceFace &arFace)
{
    vector<CSurfaceFace> triangulized_faces;
    for (size_t i = 1; i < arFace.mVertices.size()-1; i++) {
        CSurfaceFace triangle = CSurfaceFace(
                {arFace.mVertices[0], arFace.mVertices[i], arFace.mVertices[i + 1]},
                0,
//                mUVcoords[arFace.mPairPoints.first] * 0.5 + mUVcoords[arFace.mPairPoints.second] * 0.5,
                arFace.mPairPoints);
        NormalizeFace(triangle);
        triangulized_faces.push_back(triangle);
    }
    return triangulized_faces;
}

void CSurface::NormalizeFace(CSurfaceFace& arTriangleFace)
{
    bool c_point1_mask = mCreationMask[arTriangleFace.mPairPoints.first];
    bool c_point2_mask = mCreationMask[arTriangleFace.mPairPoints.second];
    CPoint c_point1 = mInputPoints[arTriangleFace.mPairPoints.first];
    CPoint c_point2 = mInputPoints[arTriangleFace.mPairPoints.second];

    // assumption 1
    // true -> false is normal direction
    CPoint true_normal;
    if (c_point1_mask) {
        arTriangleFace.mUVcoord = mUVcoords[arTriangleFace.mPairPoints.first];
        true_normal = (c_point2 - c_point1).Normalize();
    } else {
        arTriangleFace.mUVcoord = mUVcoords[arTriangleFace.mPairPoints.second];
        true_normal = (c_point1 - c_point2).Normalize();
    }
    // assumption 2
    // vectors used to calculate normal is: (p2 - p1) x (p3 - p1).
    // In order to switch direction, switch either p1<->p3, or p2<->p3 (preferred with the above assumption
    CPoint face_normal = ((*arTriangleFace.mVertices[1] - *arTriangleFace.mVertices[0]).Cross(*arTriangleFace.mVertices[2] - *arTriangleFace.mVertices[0])).Normalize();
    true_normal.Scale(face_normal);
    // if the direction is opposite
    if (true_normal.Dot({1,1,1}) < 0) {
        arTriangleFace.mVertices = {arTriangleFace.mVertices[0], arTriangleFace.mVertices[2], arTriangleFace.mVertices[1]};
    }
}

// Do to the removal of many unneeded faces in CSurface::CleanFaces there are plenty of unused points in the data, there are also double points (two points on the exact same place),
// this function cleans both
void CSurface::CleanPoints()
{
    vector<shared_ptr<CPoint> > new_points;
    for (auto & mVecPoint : mVertices) {
        if (mVecPoint.use_count() > 1) { // the points is also being called from another obj, other then this vector
            new_points.push_back(mVecPoint);
        }
    }
    mVertices = new_points;
    CleanDoublePoints();
}

//----------------------------------------remove double points (two points on the exact same place) functions ----------------------------------------------------------------------


vector<CSurfacePoint> CSurface::RemoveDoublesVornInput(vector<CSurfacePoint>& arData)
{
    //sort the array
    sort(arData.begin(), arData.end());

    vector<CSurfacePoint> cleaned_data;
    size_t j;
    int removed_points =0;
    for (size_t i = 0; i < arData.size(); i++) {
        j = i+1;
        cleaned_data.push_back(arData[i]); // push the point to the cleaned data
        if (j >= arData.size())
        {
            break;
        }
        while (arData[i].mPoint.Dist(arData[j].mPoint) <= POINT_SIMILARITY_THRESHOLD) { //check if the point has duplicates that we need to skip
            j += 1;
            if (j >= arData.size())
            {
                break;
            }
            removed_points ++;
        }
        i = j - 1; //set i to the last a duplicate (ot to i if there were no duplicates).
    }

    return cleaned_data;
}
void CSurface::CleanDoubleInputPoints(vector<CSurfacePoint> &arPoints)
{
    vector<CSurfacePoint> new_points = RemoveDoublesVornInput(arPoints);
    mInputPoints.clear();
    mUVcoords.clear();
    mCreationMask.clear();
    for (auto & point : new_points)
    {
        mInputPoints.push_back(point.mPoint);
        mUVcoords.push_back(point.UVcoord);
        mCreationMask.push_back(point.mMaskIsTrue);
    }
}

void CSurface::CleanDoublePoints()
{
    //sort the array
    sort(mVertices.begin(), mVertices.end());
    map < shared_ptr<CPoint>, shared_ptr<CPoint> > old_new_points; // will hold the new pointer fitting to each point
    vector<shared_ptr<CPoint> > cleaned_points;
    size_t j;
    for (size_t i = 0; i < mVertices.size(); i++) {
        j = i + 1;
        cleaned_points.push_back(mVertices[i]); // push the point to the cleaned data
        old_new_points[mVertices[i]] = mVertices[i];
        if (j >= mVertices.size())
        {
            break;
        }
        while ((*mVertices[i]).Dist((*mVertices[j])) <= POINT_SIMILARITY_THRESHOLD) { //check if the point has duplicates that we need to skip
            old_new_points[mVertices[j]] = mVertices[i];
            j += 1;
            if (j >= mVertices.size())
            {
                break;
            }
        }
        i = j - 1; //set i to the last a duplicate (ot to i if there were no duplicates).
    }
    // change the points ptrs within the faces
    for (auto faceIt = mSurfFaces.begin(); faceIt != mSurfFaces.end(); faceIt++) {
        for (auto pointIt = faceIt->mVertices.begin(); pointIt != faceIt->mVertices.end(); pointIt++) {
            (*pointIt) = old_new_points[*pointIt];
        }
    }
    mVertices = cleaned_points;
}

/*-------------------------------------------- Handle-Input Sub-Methods -------------------------------------------*/

void CSurface::PreProcessPoints(vector<CSurfacePoint> &arPoints, coord_t aNoiseDisplacement)
{
    Log(ELogCode::LOG_VIVID, "Preprocessing Data");
    CleanDoubleInputPoints(arPoints);

    CPoint box_dim, box_min, box_max;
    vector<CPoint> box_dimensions = FindContainingBox();
    box_dim = box_dimensions.at(0); box_min = box_dimensions.at(1); box_max = box_dimensions.at(2);
    mCenVector = (box_min + box_max) / 2.;
    for (int i = 0; i < mInputPoints.size(); i++){
        mInputPoints[i] -= mCenVector;
    }
    mScale = FindContainingRadius() / PARTICLE_SCALE_MAGNITUDE;

    vector<CPoint> noise_vec (mInputPoints.size());

    // Consider generator here.
    srand(time(nullptr));
    for (auto & i : noise_vec){
        i = {1,1,1};
        if(aNoiseDisplacement) {
            i = {1 + aNoiseDisplacement * ((coord_t) (rand() % 20 - 10) / 10.),
                 1 + aNoiseDisplacement * ((coord_t) (rand() % 20 - 10) / 10.),
                 1 + aNoiseDisplacement * ((coord_t) (rand() % 20 - 10) / 10.)};
        }
    }
    for (int i = 0; i < mInputPoints.size(); i++){
        mInputPoints[i] = (mInputPoints[i].Scale(noise_vec[i])) / mScale;
    }

    //CleanDoubleInputPoints(arPoints);

    box_dim = box_dim / mScale; box_min = (box_min - mCenVector) / mScale; box_max = (box_max - mCenVector) / mScale;
    mBoxPair = {box_min-box_dim*BOX_EXPAND_FACTOR, box_max+box_dim*BOX_EXPAND_FACTOR};
}