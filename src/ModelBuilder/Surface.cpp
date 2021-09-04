#include "Surface.h"
#include <map>

using namespace vivid;
using namespace std;

constexpr quan_t BOX_EXPAND_FACTOR = 1;
constexpr quan_t PARTICLE_SCALE_MAGNITUDE = 100; // TODO: Is this the right scale (-100, 100)?
constexpr quan_t NOISE_PERCENTAGE = 0.01;        // TODO: Is this the right noise percentage (-1%, 1%)

// TODO: Many for loops can be replaced by foreach loops here.

CSurface::CSurface(const vector<vector<double>> &arInputPoints, const vector<bool> &arMask, vector<quan_t> &arQuan, quan_t aVMin, quan_t aVMax)
{
    // Check input validity
    if((arInputPoints.size() != arMask.size()) || (arInputPoints.size() != arQuan.size())){
        cout << arInputPoints.size() << arMask.size() << arQuan.size() << endl; // TODO: no cout anymore. cout only in report
        if (mLogFile) (mLogFile)(CLogFile::ELogCode::LOG_ERROR, CLogFile::ELogMessage::ARRAYS_NOT_EQUAL);
    }
    if(arInputPoints.empty() || arInputPoints.empty() || arQuan.empty()) {
        if (mLogFile) (mLogFile)(CLogFile::ELogCode::LOG_ERROR, CLogFile::ELogMessage::ARRAYS_EMPTY);
    }
    if( (find(arMask.begin(),arMask.end(),true) == arMask.end()) || find(arMask.begin(), arMask.end(), false) == arMask.end() ){
        if (mLogFile) (mLogFile)(CLogFile::ELogCode::LOG_ERROR, CLogFile::ELogMessage::MISSING_BOOLEAN_VALUES);
    }
    //converting input points to CPoint format
    vector<CPoint> points (arInputPoints.size());
    for (int i = 0; i < points.size(); i++) {
        points[i] = arInputPoints[i];
    }

    mInputPoints = points;
    mMask = arMask;
    mQuan = NormQuan(arQuan, aVMin, aVMax);
    PreProcessPoints();
}

// TODO: Test if works now, then delete commented code
CSurface::CSurface(const CSurface &surf)
{
    mInputPoints = surf.mInputPoints;
    mMask = surf.mMask;
    mQuan = surf.mQuan;
    mCenVector = surf.mCenVector;
    mBoxPair = surf.mBoxPair;
    mScale = surf.mScale;

    map <shared_ptr<CPoint>, shared_ptr<CPoint>> old_to_new_Points;
    mVecPoints.clear();
    size_t counter = 0;
    map < shared_ptr<CPoint>, size_t> indexes;
    for (auto it = surf.mVecPoints.begin(); it != surf.mVecPoints.end(); it++) {
        mVecPoints.push_back(shared_ptr<CPoint>(new CPoint(**it)));
        indexes[*it] = counter;
        counter++;
    }
//    cout << surf.mVecPoints[0] << endl;
//    for (size_t i = 0; i < surf.mVecPoints.size(); i++) { //TODO: should it be iterator?
//        mVecPoints.push_back(shared_ptr<CPoint>(new CPoint(*(surf.mVecPoints[i]))));
//        old_to_new_Points[surf.mVecPoints[i]] = mVecPoints.back();
//    }
    mVecFaces.clear();
    CSurfaceFace temp;
    for (auto it = (surf.mVecFaces).begin(); it != surf.mVecFaces.end(); it++) {
        temp = CSurfaceFace();
        temp.mColor = (*it).mColor;
        temp.mPairPoints = (*it).mPairPoints;
        temp.mPoints.clear();
//        for (auto point = it->mPoints.begin(); point != it->mPoints.end(); point++) {
//            if (set_points.count(indexes[*point]) == 0) {
//                face_points.push_back(indexes[*point]);
//                set_points.insert(indexes[*point]);
//            }
//        }
        for (auto pIt = it->mPoints.begin(); pIt != it->mPoints.end(); pIt++){
            temp.mPoints.push_back(mVecPoints[indexes[*pIt]]);
        }
        if (it == (surf.mVecFaces).begin()) {
            cout << "4 Works till here" << endl;
        }
        mVecFaces.push_back(temp);
    }

//    vector<CPoint> points;
//    size_t counter = 0;
//    map < shared_ptr<CPoint>, size_t> indexes;
//    for (auto it = mVecPoints.begin(); it != mVecPoints.end(); it++) {
//        //points.push_back(**it);
//        points.push_back((**it * mScale) + mCenVector);
//        indexes[*it] = counter;
//        counter++;
//    }
//    vector<CIndexedFace> faces;
//    vector<size_t> face_points;
//    set<size_t> set_points;
//    for (auto it = mVecFaces.begin(); it != mVecFaces.end(); it++) {
//        for (auto point = it->mPoints.begin(); point != it->mPoints.end(); point++) {
//            if (set_points.count(indexes[*point]) == 0) {
//                face_points.push_back(indexes[*point]);
//                set_points.insert(indexes[*point]);
//            }
//        }
//        faces.push_back(CIndexedFace(face_points, it->mColor));
//        face_points = vector<size_t>();
//        set_points.clear();
//    }
//
}

//   ------------------------------------------------ Public Methods ------------------------------------------------>>>
//   <<<--------------------------------------------- Public Methods ---------------------------------------------------

void CSurface::CreateSurface()
{
    RunVorn();

    cout << "Cleaning Surface" << endl; //todo cout
    CleanEdges();
    CleanFaces();
    CleanPoints();
}

void CSurface::Smooth(bool aSuperSmooth, int aSmoothFactor)
{
    if (aSmoothFactor < 1 || aSmoothFactor > 8) {
        if (mLogFile) (mLogFile)(CLogFile::ELogCode::LOG_ERROR, CLogFile::ELogMessage::INVALID_SMOOTH_FACTOR); //TODO
    }
    //begin smooth part 1, collecting all the cpoints from the faces on the surf
    cout << "Begin Smooth" << endl; //TODO
    vector<size_t> p_out, p_in;
    SetPinPout(p_out, p_in);
    UpdateInput(p_out, p_in);
    RunVorn();
    cout << mVecFaces.size() << " " << mVecPoints.size() << " " << mInputPoints.size()<< endl;
    cout << "Begin Smooth part 2" << endl;
    //begin smooth  part 2, adding new points between the cpoints by aSmoothFactor
    UpdatePoutPin(p_out, p_in);
    if (aSuperSmooth)
    {
        Stage2ModifyPoints(p_out, p_in);
    }
    else
    {
        Stage2AddPoints(p_out, p_in, aSmoothFactor);
    }
    //begin smooth part 3, running the model and cleaning it
    UpdateInput(p_out, p_in);
    MakeMask(p_out.size(), p_in.size());
    CreateSurface();
    //done.
}

// TODO: CSurface is currently inheriting from Mesh, need to discuss this
const CMesh CSurface::ToMesh(string aLabel, quan_t aAlpha) const {
    // TODO why aAlpha is a quan_t type?
    //check input valdilty
    if(aAlpha > 1 || aAlpha <= 0){
        if (mLogFile) (mLogFile)(CLogFile::ELogCode::LOG_WARNING, CLogFile::ELogMessage::INVALID_ALPHA_VALUE);
        if (aAlpha > 1) { aAlpha = 1; }
        if (aAlpha < 0) { aAlpha = 0.1; }
//        aAlpha = min(0.1 , max(aAlpha ,1)) //TODO also 0, 0.1 and 1 should be defined at the begining of the file as they are magic numbers!
    }

    vector<CPoint> points;
    size_t counter = 0;
    map < shared_ptr<CPoint>, size_t> indexes;
    for (auto it = mVecPoints.begin(); it != mVecPoints.end(); it++) {
        //points.push_back(**it);
        points.push_back((**it * mScale) + mCenVector);
        indexes[*it] = counter;
        counter++;
    }
    vector<CIndexedFace> faces;
    vector<size_t> face_points;
    set<size_t> set_points;
    for (auto it = mVecFaces.begin(); it != mVecFaces.end(); it++) {
        for (auto point = it->mPoints.begin(); point != it->mPoints.end(); point++) {
            if (set_points.count(indexes[*point]) == 0) {
                face_points.push_back(indexes[*point]);
                set_points.insert(indexes[*point]);
            }
        }
        faces.push_back(CIndexedFace(face_points, it->mColor));
        face_points = vector<size_t>();
        set_points.clear();
    }
    return CMesh(points, faces, aLabel, aAlpha);
}

/*------------------------------------------------- Private Methods --------------------------------------------------*/

/* ---------------------------------------------- Handle RunVorn Methods ---------------------------------------------*/

vector<shared_ptr<CPoint> > ConvertFromVorn(const vector<Vector3D>& arVornPoints)
{
    vector<shared_ptr<CPoint> > new_vec;
    for (auto it = arVornPoints.begin(); it != arVornPoints.end(); it++) {
        new_vec.push_back(shared_ptr<CPoint>(new CPoint(it->x, it->y, it->z))); //TODO y not copy c~tor?
    }
    return new_vec;
}

void CSurface::RunVorn()
{
    cout << "start vorn" << endl; //TODO cout
    mVoronoi.ComputeVoronoi(mInputPoints, mBoxPair);
    cout << "vorn done" << endl;
    cout << "Vorn Faces # = " << mVoronoi.mData.GetTotalFacesNumber() << endl;
    //set the points
    mVecPoints = ConvertFromVorn(mVoronoi.mData.GetFacePoints());
    cout << "Total Points # = " << mVoronoi.mData.GetTotalPointNumber() << endl;
    //set the faces
    vector<vector<size_t> > vorn_faces = mVoronoi.GetFaces();
    vector<CSurfaceFace> new_faces;
    vector<vector<size_t>> points_map;
    points_map.resize(mVoronoi.mData.GetTotalPointNumber(), vector<size_t>(0));
    size_t c_point1, c_point2;
    quan_t quan;
    vector<shared_ptr<CPoint>> face_points;
    int counter =0;
    for (auto face = vorn_faces.begin(); face != vorn_faces.end(); face++) {
        c_point1 = face->back();
        face->pop_back();
        c_point2 = face->back();
        face->pop_back();
        quan = 0; //quan will be defined later in the program(clean faces) so for now this will be a placeholder
        face_points = vector<shared_ptr<CPoint> >();

        for (auto point = face->begin(); point != face->end(); point++) {
            face_points.push_back(mVecPoints[*point]);
        }
        new_faces.push_back(CSurfaceFace(face_points, quan, pair<size_t, size_t>(c_point1, c_point2)));
        points_map[c_point1].push_back(new_faces.size()-1);
        points_map[c_point2].push_back(new_faces.size()-1);
        counter++;
    }

    mVecFaces = new_faces;
    mPointsInFaces = points_map;
}

/*---------------------------------------------- Smoothing Sub-Methods -----------------------------------------------*/
/*----------------------------------------------------- Part 1 -------------------------------------------------------*/

void CSurface::SetPinPout(vector<size_t>& arPOut, vector<size_t>& arPIn)
{
    map<size_t, bool> p_in_map;
    map<size_t, bool> p_out_map;
    size_t c_point1, c_point2; // TODO why size_t? didn't we got rid of it?
    for (auto it = mVecFaces.begin(); it != mVecFaces.end(); it++) {
        c_point1 = get<0>(it->mPairPoints);
        c_point2 = get<1>(it->mPairPoints);
        if (mMask[c_point1]) {
            if (p_in_map.count(c_point1) == 0) {
                arPIn.push_back(c_point1);
                p_in_map[c_point1] = true;
            }
            if (p_out_map.count(c_point2) == 0) {
                arPOut.push_back(c_point2);
                p_out_map[c_point2] = true;
            }
        }
        else
        {
            if (p_in_map.count(c_point2) == 0) {
                arPIn.push_back(c_point2);
                p_in_map[c_point2] = true;
            }
            if (p_out_map.count(c_point1) == 0) {
                arPOut.push_back(c_point1);
                p_out_map[c_point1] = true;
            }
        }
    }
}

void CSurface::UpdateInput(vector<size_t>& arPOut, vector<size_t>& arPIn)
{
    // TODO should we update quan instead of updating arPOut & arPIn?
    vector<CPoint> new_points;
    vector<quan_t> quan;
    for (auto it = arPOut.begin(); it != arPOut.end(); it++) {
        new_points.push_back(mInputPoints[*it]);
        quan.push_back(mQuan[*it]);
    }
    for (auto it = arPIn.begin(); it != arPIn.end(); it++) {
        new_points.push_back(mInputPoints[*it]);
        quan.push_back(mQuan[*it]);
    }
    mInputPoints = new_points;
    mQuan = quan;
}

/*----------------------------------------------------- Part 2 -------------------------------------------------------*/

void CSurface::UpdatePoutPin(vector<size_t>& aPOut, vector<size_t>& aPIn)
{
    for (int i = 0; (unsigned)i < aPOut.size(); i++) {
        aPOut[i] = i;
    }
    for (int i = 0; (unsigned)i < aPIn.size(); i++) {
        aPIn[i] = i + aPOut.size();
    }
}
//TODO SIMILAR to THE OLDER CODE OF CompPointRD
bool CompPointData(const CSurfacePoint aObj1, const CSurfacePoint aObj2)
{
    if (abs(aObj1.mPoint.X() - aObj2.mPoint.X()) <= POINT_SIMILARITY_THRESHOLD) { //the x value is nurmallcly the same
        if (abs(aObj1.mPoint.Y() - aObj2.mPoint.Y()) <= POINT_SIMILARITY_THRESHOLD) { //the y value is nurmallcly the same
            if (abs(aObj1.mPoint.Z() - aObj2.mPoint.Z()) <= POINT_SIMILARITY_THRESHOLD) { //the z value is nurmallcly the same
                return false;
            }
            else
            { // we compare the points by z to see who needs to go first
                return aObj1.mPoint.Z() > aObj2.mPoint.Z();
            }
        }
        else // we compare by y to see who needs to go first
        {
            return aObj1.mPoint.Y() > aObj2.mPoint.Y();
        }
    }
    else // we compare by x to see who goes first
    {
        return aObj1.mPoint.X() > aObj2.mPoint.X();
    }
}

// Alt Stage2AddPoints
void CSurface::Stage2ModifyPoints(vector<size_t> &arPOut, vector<size_t> &arPIn)
{
    size_t c_point1;
    size_t c_point2;
    size_t c_point3;

    vector<CPoint> new_points;
    vector<quan_t> new_quan;
    size_t p_out_size = arPOut.size();
    size_t p_in_size = p_out_size + arPIn.size();
    arPIn.clear();
    arPOut.clear();
    size_t new_index = 0; // the index for the new point to be added
    //go over pout
    for (auto it = mVecFaces.begin(); it != mVecFaces.end(); it++) {
        c_point1 = get<0>(it->mPairPoints);
        c_point2 = get<1>(it->mPairPoints);
        FindPairPoints(c_point1, c_point2, arPIn, arPOut, p_out_size, p_in_size, *it, new_points, new_quan, new_index);
        FindPairPoints(c_point2, c_point1, arPIn, arPOut, p_out_size, p_in_size, *it, new_points, new_quan, new_index);
    }
    CleanDoublePointsVorn(new_points, new_quan, arPIn, arPOut);
}

void CSurface::FindPairPoints(size_t aCPoint1, size_t aCPoint2, vector<size_t> &arPIn, vector<size_t> &arPOut, size_t aPOutSize, size_t aPInSize,
                              CSurfaceFace &arFace, vector<CPoint> &arNewPoints, vector<quan_t> &arNewQuan, size_t &arIndex)
{
    size_t c_point3;
    for (auto f_index = mPointsInFaces[aCPoint1].begin(); f_index != mPointsInFaces[aCPoint1].end(); f_index++)
    {
        CSurfaceFace face = mVecFaces[*f_index];
        if (face.mPairPoints != arFace.mPairPoints) {
            if (face.mPairPoints.first == aCPoint1) {
                c_point3 = get<1>(face.mPairPoints);
            } else {
                c_point3 = get<0>(face.mPairPoints);
            }
            // TODO: Add points only if certain p_in and p_out conditions are met
            if (aCPoint1 < aPOutSize && aCPoint2 < aPOutSize && c_point3 < aPOutSize) //pout - [1,2,3,4...pout.size] so we are checking if cpoint is a part of pout
            {
                AddPointsAlt(arPIn, arNewPoints, arNewQuan, arIndex, aCPoint1, aCPoint2, c_point3);
            }
            //go over pin
            if ((aPInSize > aCPoint1 && aCPoint1 >= aPOutSize) && (aPInSize > aCPoint2 && aCPoint2 >= aPOutSize) &&
                (aPInSize > c_point3 && c_point3 >= aPOutSize)) //pin - [pout.size...pout.size+pin.size] so we are checking if cpoint is a part of pin
            {
                AddPointsAlt(arPOut, arNewPoints, arNewQuan, arIndex, aCPoint1, aCPoint2, c_point3);
            }
        }
    }
}

void CSurface::AddPointsAlt(vector<size_t> &arPVec, vector<CPoint> &arNewPoints, vector<quan_t> &arNewQuan,
                            size_t &arNewIndex, size_t aCPoint1, size_t aCPoint2, size_t aCPoint3)
{
    arPVec.push_back(arNewIndex);
    CPoint new_point = (mInputPoints[aCPoint1] * 2 + mInputPoints[aCPoint2] + mInputPoints[aCPoint3]) / 4.;
    arNewPoints.push_back(new_point);
    arNewQuan.push_back((mQuan[aCPoint1] + mQuan[aCPoint2] + mQuan[aCPoint3]) / 4); // Results in color washing, need to consider
    arNewIndex++;
}

void CSurface::Stage2AddPoints(vector<size_t>& arPOut, vector<size_t>& arPIn, int aSmoothFactor)
{
    size_t c_point1;
    size_t c_point2;
    size_t p_out_size = arPOut.size();
    size_t p_in_size = p_out_size + arPIn.size();
    vector<CPoint> new_points;
    vector<quan_t> new_quan;
    arPIn.clear();
    arPOut.clear();
    size_t new_index = 0; // the index for the new point to be added
    //go over pout
    for (auto it = mVecFaces.begin(); it != mVecFaces.end(); it++) {
        c_point1 = get<0>(it->mPairPoints);
        c_point2 = get<1>(it->mPairPoints);
        if (c_point1 < p_out_size && c_point2 < p_out_size) //pout - [1,2,3,4...pout.size] so we are checking if cpoint is a part of pout
        {
            AddPoints(&arPIn, &new_points, &new_quan, &new_index, c_point1, c_point2, aSmoothFactor);
        }
        //go over pin
        if ((p_in_size > c_point1 && c_point1 >= p_out_size) && (p_in_size > c_point2 && c_point2 >= p_out_size)) //pin - [pout.size...pout.size+pin.size] so we are checking if cpoint is a part of pin
        {
            AddPoints(&arPOut, &new_points, &new_quan, &new_index, c_point1, c_point2, aSmoothFactor);
        }
    }
    CleanDoublePointsVorn(new_points, new_quan, arPIn, arPOut);
}

void CSurface::AddPoints(vector<size_t> * apPVec, vector<CPoint> * apNewPoints, vector<quan_t> * apNewQuan, size_t * apNewIndex, size_t aCPoint1, size_t aCPoint2, int aSmoothFactor)
{
    quan_t x, y, z;
    for (int i = 1; i <= aSmoothFactor; i++){
        (*apPVec).push_back(*apNewIndex);
        x = (mInputPoints[aCPoint1].X() * (aSmoothFactor + 1 - i) + mInputPoints[aCPoint2].X() * i) / (aSmoothFactor+1);
        y = (mInputPoints[aCPoint1].Y() * (aSmoothFactor + 1 - i) + mInputPoints[aCPoint2].Y() * i) / (aSmoothFactor+1);
        z = (mInputPoints[aCPoint1].Z() * (aSmoothFactor + 1 - i) + mInputPoints[aCPoint2].Z() * i) / (aSmoothFactor+1);
        (*apNewPoints).push_back(CPoint(x, y, z));
        (*apNewQuan).push_back((mQuan[aCPoint1] * (aSmoothFactor + 1 - i) + mQuan[aCPoint2] * i) / (aSmoothFactor+1));
        (*apNewIndex)++;
    }
}

void CSurface::CleanDoublePointsVorn(vector<CPoint>& arNewPoints, vector<quan_t>& arNewQuan, vector<size_t>& arNewIn, vector<size_t>& arNewOut)
{
    vector<CSurfacePoint> data;
    data.clear();
    for (auto it = arNewIn.begin(); it != arNewIn.end(); it++) {
        data.push_back(CSurfacePoint(arNewPoints[*it], arNewQuan[*it], true));
    }
    for (auto it = arNewOut.begin(); it != arNewOut.end(); it++) {
        data.push_back(CSurfacePoint(arNewPoints[*it], arNewQuan[*it], false));
    }
    data = RemoveDoublesVornInput(data);
    arNewPoints.clear();
    arNewQuan.clear();
    arNewIn.clear();
    arNewOut.clear();
    for (size_t i = 0; i < data.size(); i++) {
        arNewPoints.push_back(data[i].mPoint);
        arNewQuan.push_back(data[i].mQuan);
        if (data[i].mIsIn) {
            arNewIn.push_back(i);
        }
        else
        {
            arNewOut.push_back(i);
        }
    }
    mInputPoints = arNewPoints;
    mQuan = arNewQuan;
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
            //cout << "removed point" << endl;
        }
        i = j - 1; //set i to the last a duplicate (ot to i if there were no duplicates).
    }
    cout << "Removed Points: " << removed_points << endl;

    return cleaned_data;
}


/*----------------------------------------------------- Part 3 -------------------------------------------------------*/

void CSurface::MakeMask(size_t aPOutSize, size_t aPInSize)
{
    vector<bool> new_mask;
    for (size_t i = 0; i < aPOutSize; i++) { //TODO might be faster using fill or something similar in vector
        new_mask.push_back(false);

    }
    for (size_t i = 0; i < aPInSize; i++) {
        new_mask.push_back(true);
    }
    mMask = new_mask;
}

/* -------------------------------------------- Centralization Sub-Methods -------------------------------------------*/

vector<CPoint> CSurface::FindContainingBox()
{
    quan_t x_max = (max_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.X() < arV2.X();}))->X();
    quan_t x_min = (min_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.X() < arV2.X();}))->X();
    quan_t y_max = (max_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Y() < arV2.Y();}))->Y();
    quan_t y_min = (min_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Y() < arV2.Y();}))->Y();
    quan_t z_max = (max_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Z() < arV2.Z();}))->Z();
    quan_t z_min = (min_element(mInputPoints.begin(), mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Z() < arV2.Z();}))->Z();

    CPoint box_dim = CPoint(x_max-x_min, y_max-y_min, z_max-z_min);
    CPoint box_min = CPoint(x_min, y_min, z_min);
    CPoint box_max = CPoint(x_max, y_max, z_max);

    return vector<CPoint>{box_dim, box_min, box_max};
}


quan_t CSurface::FindContainingRadius()
{
    return (max_element(mInputPoints.begin(),mInputPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Magnitude() < arV2.Magnitude();}))->Magnitude();
}

/*----------------------------------------------- Cleaning Sub-Methods -----------------------------------------------*/

void CSurface::CleanEdges()
{
    bool is_out_of_radius = false;
    vector<CSurfaceFace> new_faces;
    for (auto face = mVecFaces.begin(); face != mVecFaces.end(); face++) {
        for (auto point = face->mPoints.begin(); point != face->mPoints.end(); point++) {
            if (abs((**point).X()) > (mBoxPair.second.X() * 0.35) || abs((**point).Y()) > (mBoxPair.second.Y() * 0.35) || abs((**point).Z()) > (mBoxPair.second.Z() * 0.35)) {
                is_out_of_radius = true;
            }
        }
        if (!is_out_of_radius) { //the face is inside the box Radius and should be kept
            new_faces.push_back(*face);
        }
        is_out_of_radius = false;
    }
    mVecFaces = new_faces;
}

// takes the data from mVoronoi and using the mask cleans it so that only the wanted surface is left
void CSurface::CleanFaces()
{
    vector<CSurfaceFace> new_faces;
    size_t mask_len = mMask.size();
    size_t c_point1;
    size_t c_point2;
    for (auto it = mVecFaces.begin(); it != mVecFaces.end(); it++) {
        c_point1 = get<0>(it->mPairPoints);
        c_point2 = get<1>(it->mPairPoints);
        if (mask_len > c_point1 && mask_len > c_point2) { //the indexs are both in range and not a part of the box
            if (mMask[c_point1] != mMask[c_point2]) { //the face is a part of the surf
                new_faces.push_back(*it);
                new_faces.back().mColor = (mQuan[c_point1] + mQuan[c_point2]) / 2; //TODO 0.5* +0.5* is better
            }
        }

    }
    mVecFaces = new_faces;
}

// Do to the removal of many unneeded faces in CSurface::CleanFaces there are plenty of unused points in the data, there are also double points (two points on the exact same place),
// this function cleans both
void CSurface::CleanPoints()
{
    vector<shared_ptr<CPoint> > new_points;
    for (auto it = mVecPoints.begin(); it != mVecPoints.end(); it++) {
        if (it->use_count() > 1) { // the points is also being called from another obj, other then this vector
            new_points.push_back(*it);
        }
    }
    mVecPoints = new_points;
    CleanDoublePoints();
}

//----------------------------------------remove double points (two points on the exact same place) functions ----------------------------------------------------------------------

// TODO: Should use different sorting algorithm
bool CompPointRD(const shared_ptr<CPoint>& arV1, const shared_ptr<CPoint>& arV2) //TODO this 1st func is the same as the one after!
{
    return ((*arV1).Dist(*arV2) <= POINT_SIMILARITY_THRESHOLD); //// TODO lambda? or something with CPoint? POINT_SIMILARITY_THRESHOLD is a CPOINT issue
}
bool CompPointD(const CPoint& arV1, const CPoint& arV2)
{
    return (arV1.Dist(arV2) <= POINT_SIMILARITY_THRESHOLD);
}

// TODO: Repeating code. Why?
void CSurface::CleanDoubleInputPoints()
{
    //sort the array
    sort(mInputPoints.begin(), mInputPoints.end(), CompPointD);
    // Used to be shared_ptr<CPoint>
    vector<CPoint> cleaned_points; // Used to be shared_ptr<CPoint>
    size_t j;
    for (size_t i = 0; i < mInputPoints.size(); i++) {
        j = i + 1;
        cleaned_points.push_back(mInputPoints[i]); // push the point to the cleaned data
        if (j >= mInputPoints.size())
        {
            break;
        }
        // Used to be *mVecPoints (pointer)
        while ((mInputPoints[i]).Dist((mInputPoints[j])) <= POINT_SIMILARITY_THRESHOLD) { //check if the point has duplicates that we need to skip
            j += 1;
            if (j >= mInputPoints.size())
            {
                break;
            }
            //cout << "removed point" << endl;
        }
        i = j - 1; //set i to the last a duplicate (ot to i if there were no duplicates).
    }

    mInputPoints = cleaned_points;
}

void CSurface::CleanDoublePoints()
{
    //sort the array
    sort(mVecPoints.begin(), mVecPoints.end(), CompPointRD);
    map < shared_ptr<CPoint>, shared_ptr<CPoint> > old_new_points; // will hold the new pointer fiting to each point
    vector<shared_ptr<CPoint> > cleaned_points;
    size_t j;
    for (size_t i = 0; i < mVecPoints.size(); i++) {
        j = i + 1;
        cleaned_points.push_back(mVecPoints[i]); // push the point to the cleaned data
        old_new_points[mVecPoints[i]] = mVecPoints[i];
        if (j >= mVecPoints.size())
        {
            break;
        }
        while ((*mVecPoints[i]).Dist((*mVecPoints[j])) <= POINT_SIMILARITY_THRESHOLD) { //check if the point has duplicates that we need to skip
            old_new_points[mVecPoints[j]] = mVecPoints[i];
            j += 1;
            if (j >= mVecPoints.size())
            {
                break;
            }
            //cout << "removed point" << endl;
        }
        i = j - 1; //set i to the last a duplicate (ot to i if there were no duplicates).
    }
    // change the points ptrs within the faces
    for (auto faceIt = mVecFaces.begin(); faceIt != mVecFaces.end(); faceIt++) {
        for (auto pointIt = faceIt->mPoints.begin(); pointIt != faceIt->mPoints.end(); pointIt++) {
            (*pointIt) = old_new_points[*pointIt];
        }
    }
    mVecPoints = cleaned_points;
}

/*-------------------------------------------- Handle-Input Sub-Methods -------------------------------------------*/
//TODO should be one line with some std function
vector<quan_t>& CSurface::NormQuan(vector<quan_t> &arQuan, quan_t aVMin, quan_t aVMax)
{
    if (arQuan.size() == 0){ //incase the user doesnt input any color
        arQuan = vector<quan_t>(mMask.size(), 1);
        return arQuan;
    }
    if(aVMin ==  aVMax) { //in cased the user inputs color but not aVMin and aVMax
        aVMax = *max_element(arQuan.begin(), arQuan.end());
        aVMin = *min_element(arQuan.begin(), arQuan.end());
    }
    if (aVMin == aVMax) { //in case where Vmin-Vmax == 0 (aQuan is a vector where all the values are the same)
        arQuan = vector<quan_t>(arQuan.size(), 1);
        return arQuan;
    }
    quan_t divide_by = 1. / (aVMax - aVMin);
    for (auto it = arQuan.begin(); it != arQuan.end(); it++) {
        *it = (*it - aVMin) * divide_by;
        if (*it > 1) *it = 1;
        if (*it < 0) *it = 0;
    }
    return arQuan;
}

void CSurface::PreProcessPoints()
{
    //CleanDoubleInputPoints();
    CPoint box_dim, box_min, box_max;
    vector<CPoint> box_dimensions = FindContainingBox();
    box_dim = box_dimensions.at(0); box_min = box_dimensions.at(1); box_max = box_dimensions.at(2);
    mCenVector = (box_min + box_max) / 2;
    for (int i = 0; i < mInputPoints.size(); i++){
        mInputPoints[i] = mInputPoints[i] - mCenVector;
    }
    mScale = FindContainingRadius() / PARTICLE_SCALE_MAGNITUDE;
    vector<CPoint> noise_vec (mInputPoints.size());

    // Consider generator here.
    srand(time(NULL));
    for (int i = 0; i<noise_vec.size(); i++){
        noise_vec[i] = {1 + NOISE_PERCENTAGE*((quan_t)(rand() % 20 - 10) / 10.),
                        1 + NOISE_PERCENTAGE*((quan_t)(rand() % 20 - 10) / 10.),
                        1 + NOISE_PERCENTAGE*((quan_t)(rand() % 20 - 10) / 10.)   };
    }

    for (int i = 0; i < mInputPoints.size(); i++){
        mInputPoints[i] = (mInputPoints[i].Scale(noise_vec[i])) / mScale;
        //mInputPoints[i] = mInputPoints[i] / mScale;
    }

    box_dim = box_dim / mScale; box_min = (box_min - mCenVector) / mScale; box_max = (box_max - mCenVector) / mScale;

    mBoxPair = {box_min-box_dim*BOX_EXPAND_FACTOR, box_max+box_dim*BOX_EXPAND_FACTOR};

}
