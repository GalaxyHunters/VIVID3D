#include "Surface.h"
#include <map>

using namespace vivid;
using namespace std;

constexpr coord_t BOX_EXPAND_FACTOR = 1;
constexpr coord_t PARTICLE_SCALE_MAGNITUDE = 100;
constexpr coord_t NOISE_PERCENTAGE = 0.005;

CSurface::CSurface(const vector<CPoint> &arInputPoints, const vector<bool> &arMask, vector<quan_t> &arQuan, quan_t aVMin, quan_t aVMax)
{
    // Check input validity
    if((arInputPoints.size() != arMask.size()) || (arInputPoints.size() != arQuan.size())){
        CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::ARRAYS_NOT_EQUAL);
    }
    if(arInputPoints.empty() || arInputPoints.empty() || arQuan.empty()) {
        CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::ARRAYS_EMPTY);
    }
    if( (find(arMask.begin(),arMask.end(),true) == arMask.end()) || find(arMask.begin(), arMask.end(), false) == arMask.end() ){
        CLogFile::GetInstance().Write(ELogCode::LOG_ERROR, ELogMessage::MISSING_BOOLEAN_VALUES);
    }
    vector<CSurfacePoint> points;
    vector<quan_t> norm_quan = NormQuan(arQuan, aVMin, aVMax);
    for (int i = 0; i < arInputPoints.size(); i++) {
        points.push_back(CSurfacePoint(CPoint(arInputPoints[i]), norm_quan[i], arMask[i]));
    }
    mInputPoints = arInputPoints;
    PreProcessPoints(points);
}

CSurface::CSurface(const CSurface &surf)
{
    mInputPoints = surf.mInputPoints;
    mCreationMask = surf.mCreationMask;
    mQuan = surf.mQuan;
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
        temp.mColor = mVecFace.mColor;
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

void CSurface::CreateSurface(bool aPostProcessing)
{
    RunVorn();
    CleanEdges();
    if (aPostProcessing) {
        CleanFaces();
        CleanPoints();
    }
}

CMesh CSurface::ToMesh(const string& arLabel, coord_t aAlpha) const {
    //check input validity
    if(aAlpha > 1 || aAlpha <= 0){
        CLogFile::GetInstance().Write(ELogCode::LOG_WARNING, ELogMessage::INVALID_ALPHA_VALUE);
        aAlpha = max(0.1 , min(aAlpha ,1.)); //TODO also 0, 0.1 and 1 should be defined at the begining of the file as they are magic numbers!
    }

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
        faces.push_back(CFace(face_points, mVecFace.mColor));
        face_points = vector<size_t>();
        set_points.clear();
    }
    return CMesh(points, faces, arLabel, aAlpha);
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
    CLogFile::GetInstance().WriteCustom(ELogCode::LOG_VIVID, "Begin Compute Voronoi");
    mVoronoi.ComputeVoronoi(mInputPoints, mBoxPair);
    //set the points
    mVertices = ConvertFromVorn(mVoronoi.mData.GetFacePoints());
    //set the faces
    vector<vector<size_t> > vorn_faces = mVoronoi.GetFaces();
    vector<CSurfaceFace> new_faces;
    vector<vector<size_t>> points_map;
    points_map.resize(mVoronoi.mData.GetTotalPointNumber(), vector<size_t>(0));
    size_t c_point1, c_point2;
    quan_t quan;
    vector<shared_ptr<CPoint>> face_points;

    for (auto & vorn_face : vorn_faces) {
        c_point1 = vorn_face.back();
        vorn_face.pop_back();
        c_point2 = vorn_face.back();
        vorn_face.pop_back();
        quan = 0; //quan will be defined later in the program(clean faces) so for now this will be a placeholder
        face_points = vector<shared_ptr<CPoint> >();

        for (size_t & point : vorn_face) {
            face_points.push_back(mVertices[point]);
        }
        new_faces.push_back(CSurfaceFace(face_points, quan, pair<size_t, size_t>(c_point1, c_point2)));
    }

    mSurfFaces = new_faces;

    CLogFile::GetInstance().WriteCustom(ELogCode::LOG_VIVID, "Finished Compute Voronoi");
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
    size_t c_point1;
    size_t c_point2;
    for (auto & mVecFace : mSurfFaces) {
        c_point1 = get<0>(mVecFace.mPairPoints);
        c_point2 = get<1>(mVecFace.mPairPoints);
        if (mask_len > c_point1 && mask_len > c_point2) { //the indexs are both in range and not a part of the box
            if (mCreationMask[c_point1] != mCreationMask[c_point2]) { //the face is a part of the surf
                new_faces.push_back(mVecFace);
                new_faces.back().mColor = mQuan[c_point1]*0.5 + mQuan[c_point2]*0.5;
            }
        } else {
            new_faces.push_back(mVecFace);
            new_faces.back().mColor = 0;
        }

    }
    mSurfFaces = new_faces;
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

// TODO: Should use different sorting algorithm
bool CompPointRD(const shared_ptr<CPoint>& arV1, const shared_ptr<CPoint>& arV2) //TODO this 1st func is the same as the one after!
{
    return ((*arV1).Dist(*arV2) <= POINT_SIMILARITY_THRESHOLD); //// TODO lambda? or something with CPoint? POINT_SIMILARITY_THRESHOLD is a CPOINT issue
}

//TODO SIMILAR to THE OLDER CODE OF CompPointRD
bool CompPointData(const CSurfacePoint &arObj1, const CSurfacePoint &arObj2)
{
    if (abs(arObj1.mPoint.X() - arObj2.mPoint.X()) <= POINT_SIMILARITY_THRESHOLD) {
        if (abs(arObj1.mPoint.Y() - arObj2.mPoint.Y()) <= POINT_SIMILARITY_THRESHOLD) {
            if (abs(arObj1.mPoint.Z() - arObj2.mPoint.Z()) <= POINT_SIMILARITY_THRESHOLD) {
                return false;
            }
            else
            { // we compare the points by z to see who needs to go first
                return arObj1.mPoint.Z() > arObj2.mPoint.Z();
            }
        }
        else // we compare by y to see who needs to go first
        {
            return arObj1.mPoint.Y() > arObj2.mPoint.Y();
        }
    }
    else // we compare by x to see who goes first
    {
        return arObj1.mPoint.X() > arObj2.mPoint.X();
    }
}

vector<CSurfacePoint> CSurface::RemoveDoublesVornInput(vector<CSurfacePoint>& arData)
{
    //sort the array
    sort(arData.begin(), arData.end(), CompPointData);

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
    string str = "Removed Points = " + removed_points;
    CLogFile::GetInstance().WriteCustom(ELogCode::LOG_VIVID, str);

    return cleaned_data;
}
void CSurface::CleanDoubleInputPoints(vector<CSurfacePoint> &arPoints)
{
    vector<CSurfacePoint> new_points = RemoveDoublesVornInput(arPoints);
    mInputPoints.clear();
    mQuan.clear();
    mCreationMask.clear();
    for (auto & point : new_points)
    {
        mInputPoints.push_back(point.mPoint);
        mQuan.push_back(point.mQuan);
        mCreationMask.push_back(point.mMaskIsTrue);
    }
}

void CSurface::CleanDoublePoints()
{
    //sort the array
    sort(mVertices.begin(), mVertices.end(), CompPointRD);
    map < shared_ptr<CPoint>, shared_ptr<CPoint> > old_new_points; // will hold the new pointer fiting to each point
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
vector<coord_t>& CSurface::NormQuan(vector<quan_t> &arQuan, quan_t aVMin, quan_t aVMax)
{
    if (arQuan.size() == 0){ //incase the user doesnt input any color
        arQuan = vector<quan_t>(mCreationMask.size(), 1);
        return arQuan;
    }
    if(aVMin ==  aVMax) { //in cased the user inputs color but not aVMin and aVMax
        aVMax = *max_element(arQuan.begin(), arQuan.end());
        aVMin = *min_element(arQuan.begin(), arQuan.end());
    }
    if (aVMin == aVMax) { //in case where Vmin-Vmax == 0 (aQuan is a vector where all the values are the same)
        arQuan = vector<coord_t>(arQuan.size(), 1);
        return arQuan;
    }
    coord_t divide_by = 1. / (aVMax - aVMin);
    for (auto it = arQuan.begin(); it != arQuan.end(); it++) {
        *it = (*it - aVMin) * divide_by;
        if (*it > 1) *it = 1;
        if (*it < 0) *it = 0;
    }
    return arQuan;
}

void CSurface::PreProcessPoints(vector<CSurfacePoint> &arPoints)
{
    CLogFile::GetInstance().WriteCustom(ELogCode::LOG_VIVID, "Preprocessing Data");

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
        i = {1 + NOISE_PERCENTAGE*((coord_t)(rand() % 20 - 10) / 10.),
             1 + NOISE_PERCENTAGE*((coord_t)(rand() % 20 - 10) / 10.),
             1 + NOISE_PERCENTAGE*((coord_t)(rand() % 20 - 10) / 10.)   };
    }
    for (int i = 0; i < mInputPoints.size(); i++){
        mInputPoints[i] = (mInputPoints[i].Scale(noise_vec[i])) / mScale;
        arPoints[i].mPoint = mInputPoints[i];
    }

    CleanDoubleInputPoints(arPoints);

    box_dim = box_dim / mScale; box_min = (box_min - mCenVector) / mScale; box_max = (box_max - mCenVector) / mScale;
    mBoxPair = {box_min-box_dim*BOX_EXPAND_FACTOR, box_max+box_dim*BOX_EXPAND_FACTOR};

}