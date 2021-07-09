#include "Surface.h"
#include "Voronoi.h"

#include <map>

using namespace vivid;
using namespace std;

constexpr coord_t BOX_EXPAND_FACTOR = 1;
constexpr coord_t PARTICLE_SCALE_MAGNITUDE = 100; // TODO: Is this the right scale (-100, 100)?
constexpr coord_t NOISE_PERCENTAGE = 0.01; // TODO: Is this the right scale (-100, 100)?

CSurface::CSurface(std::vector<std::vector<double>> aInputPoints, std::vector<bool> aMask, std::vector<coord_t> aQuan, coord_t aVMin, coord_t aVMax) {
    // Check input validity
    if((aInputPoints.size() != aMask.size()) || (aInputPoints.size() != aQuan.size())){
        throw ("ValueError - input vectors have not the same size"); //TODO find better sentence and put all of it in ErrMsg.h
    }
    if(aInputPoints.empty() || aInputPoints.empty() || aQuan.empty()) {
        throw ("ValueError - input vectors is empty"); //TODO find better sentence and put all of it in ErrMsg.h
    }
    if( (find(aMask.begin(),aMask.end(),true) == aMask.end()) || std::find(aMask.begin(), aMask.end(), false) == aMask.end() ){

        throw ("ValueError - mask must contain both true and false values"); //TODO find better sentence and put all of it in ErrMsg.h
    }

    //model centralization
    vector<CPoint> points (aInputPoints.size());
    for (int i = 0; i < points.size(); i++) {  //TODO for each
        points[i] = aInputPoints[i];
    }

    points.resize(points.size()); // Why dont we set temp to be the size of aInputPoints ahead of time?
    SetInputPoints(points);
    SetMask(aMask);
    SetQuan(NormQuan(aQuan, aVMin, aVMax));
    // We can scale them while doing the temp.push_back thing, would be more efficient
    PreProcessPoints();
}

CSurface::CSurface(const CSurface &surf){ //TODO: why like this? why do you use "this->"? no need
    mInputPoints = surf.mInputPoints;
    mMask = surf.mMask;
    mQuan = surf.mQuan;

    std::map < shared_ptr<CPoint>, shared_ptr<CPoint> > old_to_new_Points;
    mVecPoints.clear();
    for (size_t i = 0; i < surf.mVecPoints.size(); i++) { //TODO: should it be iterator?
        mVecPoints.push_back(shared_ptr<CPoint>(new CPoint(*(surf.mVecPoints[i]))));
        old_to_new_Points[surf.mVecPoints[i]] = mVecPoints.back();
    }
    mVecFaces.clear();
    CSurfaceFace temp;
    for (auto it = (surf.mVecFaces).begin(); it != surf.mVecFaces.end(); it++) {
        temp = CSurfaceFace();
        temp.mColor = (*it).mColor;
        temp.mPairPoints = (*it).mPairPoints;
        temp.mPoints.clear();
        for (auto pIt = it->mPoints.begin(); pIt != it->mPoints.end(); pIt++){
            temp.mPoints.push_back(old_to_new_Points[*pIt]);
        }
        mVecFaces.push_back(temp);
    }
}

/*-------------------------------------------------- Public Methods --------------------------------------------------*/

void CSurface::CreateSurface() {
    Voronoi();
    CleanEdges();
    CleanFaces(mMask);
    CleanPoints();
}

void CSurface::Smooth() {
    //begin smooth part 1, collecting all the cpoints from the faces on the surf
    vector<size_t> p_out;
    vector<size_t> p_in;
    SetPinPout(p_out, p_in);
    UpdateInputPoints(p_out, p_in);
    Voronoi();
    //begin smooth part 2, adding new points between the cpoints
    UpdatePoutPin(p_out, p_in);
    Stage2AddPoints(p_out, p_in);
    //begin smooth part 3, running the model and cleaning it
    UpdateInputPoints(p_out, p_in);
    Voronoi();
    CleanEdges();
    MakeMask(p_out.size(), p_in.size());
    CleanFaces(mMask);
    CleanPoints();
    //done.
}
// TODO: Not touching this for now, but in theory we shouldn't be re-centralizing this anymore.
const CMesh CSurface::ToMesh(string aLabel, coord_t aAlpha) {
    //check input valdilty
    if(aAlpha > 1 || aAlpha < 0){
        throw "Alpha must be between 0 and 1";
    }
    vector<CPoint> points;
    size_t counter = 0;
    map < shared_ptr<CPoint>, size_t> indexes;
    for (auto it = mVecPoints.begin(); it != mVecPoints.end(); it++) {
        points.push_back(**it); // Used to be -mCenVector
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

/* ---------------------------------------------- Handle Voronoi Methods ---------------------------------------------*/

// TODO: Should be static?
vector<shared_ptr<CPoint> > ConvertFromVorn(vector<Vector3D> aVornPoints) {
    vector<shared_ptr<CPoint> > new_vec;
    for (auto it = aVornPoints.begin(); it != aVornPoints.end(); it++) {
        new_vec.push_back(shared_ptr<CPoint>(new CPoint(it->x, it->y, it->z))); //TODO y not copy c~tor?
    }
    return new_vec;
}

// TODO: This is shit name. what do?
void CSurface::Voronoi() {
    auto vorn_out = mVoronoi.RunVoronoi(mInputPoints);
    //auto vorn_out = mVoronoi.ComputeVoronoi(mInputPoints, box);
    cout << "vorn done" << endl;
    cout << mVoronoi.mData.GetTotalFacesNumber() << endl;
    //set the points
    mVecPoints = ConvertFromVorn(get<0>(vorn_out));
    //set the faces
    vector<vector<size_t> > vorn_faces = get<1>(vorn_out);
    vector<CSurfaceFace> new_faces;
    size_t c_point1;
    size_t c_point2;
    coord_t quan;
    vector<shared_ptr<CPoint> > face_points;
    for (auto face = vorn_faces.begin(); face != vorn_faces.end(); face++) {
        c_point1 = face->back();
        face->pop_back();
        c_point2 = face->back();
        face->pop_back();
        quan = 0; //quan will be defiened later in the program(clean faces) so for now this will be a place holder
        face_points = vector<shared_ptr<CPoint> >();

        for (auto point = face->begin(); point != face->end(); point++) {
            face_points.push_back(mVecPoints[*point]);
        }
        new_faces.push_back(CSurfaceFace(face_points, quan, pair<size_t, size_t>(c_point1, c_point2)));
    }
    mVecFaces = new_faces;
}

/*---------------------------------------------- Smoothing Sub-Methods -----------------------------------------------*/

bool CompPointData_t(CSurfacePoint aObj1, CSurfacePoint aObj2) { //TODO SIMILAR to THE OLDER CODE OF CompPointRD
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

void CSurface::CleanDoublePointsVorn(vector<CPoint>& arNewPoints, vector<coord_t>& arNewQuan, vector<size_t>& arNewIn, vector<size_t>& arNewOut)
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

vector<CSurfacePoint> CSurface::RemoveDoublesVornInput(vector<CSurfacePoint>& arData) {
    //sort the array
    std::sort(arData.begin(), arData.end(), CompPointData_t);
    vector<CSurfacePoint> cleaned_data;
    size_t j;
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
            //cout << "removed point" << endl;
        }
        i = j - 1; //set i to the last a duplicate (ot to i if there were no duplicates).
    }

    return cleaned_data;
}

void CSurface::AddPoints(vector<size_t> * apPVec, vector<CPoint> * apNewPoints, vector<coord_t> * apNewQuan, size_t * apNewIndex, size_t aCPoint1, size_t aCPoint2)
{
    //TODO ugly and wrong func name
    coord_t x, y, z;
    (*apPVec).push_back(*apNewIndex);
    x = (mInputPoints[aCPoint1].X() * 2 + mInputPoints[aCPoint2].X()) / 3.0;
    y = (mInputPoints[aCPoint1].Y() * 2 + mInputPoints[aCPoint2].Y()) / 3.0;
    z = (mInputPoints[aCPoint1].Z() * 2 + mInputPoints[aCPoint2].Z()) / 3.0;
    (*apNewPoints).push_back(CPoint(x, y, z));
    (*apNewQuan).push_back((mQuan[aCPoint1] + mQuan[aCPoint2]) / 2.0);
    (*apNewIndex)++;

    (*apPVec).push_back(*apNewIndex);
    x = (mInputPoints[aCPoint1].X() + mInputPoints[aCPoint2].X() * 2) / 3.0;
    y = (mInputPoints[aCPoint1].Y() + mInputPoints[aCPoint2].Y() * 2) / 3.0;
    z = (mInputPoints[aCPoint1].Z() + mInputPoints[aCPoint2].Z() * 2) / 3.0;
    (*apNewPoints).push_back(CPoint(x, y, z));
    (*apNewQuan).push_back((mQuan[aCPoint1] + mQuan[aCPoint2]) / 2.0);
    (*apNewIndex)++;
}

void CSurface::Stage2AddPoints(vector<size_t>& arPOut, vector<size_t>& arPIn) {
    size_t c_point1;
    size_t c_point2;
    size_t p_out_size = arPOut.size();
    size_t p_in_size = p_out_size + arPIn.size();
    vector<CPoint> new_points;
    vector<coord_t> new_quan;
    arPIn.clear();
    arPOut.clear();
    size_t new_index = 0; // the index for the new point to be added
    //go over pout
    for (auto it = mVecFaces.begin(); it != mVecFaces.end(); it++) {
        c_point1 = get<0>(it->mPairPoints);
        c_point2 = get<1>(it->mPairPoints);
        if (c_point1 < p_out_size && c_point2 < p_out_size) //pout - [1,2,3,4...pout.size] so we are checking if cpoint is a part of pout
        {
            AddPoints(&arPIn, &new_points, &new_quan, &new_index, c_point1, c_point2);
        }
        //go over pin
        if ((p_in_size > c_point1 && c_point1 >= p_out_size) && (p_in_size > c_point2 && c_point2 >= p_out_size)) //pin - [pout.size...pout.size+pin.size] so we are checking if cpoint is a part of pin
        {
            AddPoints(&arPOut, &new_points, &new_quan, &new_index, c_point1, c_point2);
        }
    }
    CleanDoublePointsVorn(new_points, new_quan, arPIn, arPOut);
}

//TODO:: what is this?
void CSurface::UpdatePoutPin(vector<size_t>& aPOut, vector<size_t>& aPIn) {
    for (int i = 0; (unsigned)i < aPOut.size(); i++) {
        aPOut[i] = i;
    }
    for (int i = 0; (unsigned)i < aPIn.size(); i++) {
        aPIn[i] = i + aPOut.size();
    }

}
void CSurface::SetPinPout(vector<size_t>& arPOut, vector<size_t>& arPIn) { //define pin and pout
    map<size_t, bool> p_in_map;
    map<size_t, bool> p_out_map;
    size_t c_point1;
    size_t c_point2;
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

void CSurface::UpdateInputPoints(vector<size_t>& arPOut, vector<size_t>& arPIn) {
    vector<CPoint> new_points;
    vector<coord_t> quan;
    for (auto it = arPOut.begin(); it != arPOut.end(); it++) {
        new_points.push_back(mInputPoints[*it]);
        quan.push_back(mQuan[*it]);
    }
    for (auto it = arPIn.begin(); it != arPIn.end(); it++) {
        new_points.push_back(this->mInputPoints[*it]);
        quan.push_back(this->mQuan[*it]);
    }
    mInputPoints = new_points;
    mQuan = quan;
}
void CSurface::MakeMask(size_t aPOutSize, size_t aPInSize) {
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

CPoint CSurface::FindContainingBox(const vector<CPoint>& arPoints){
    coord_t x_max = (max_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.X() < arV2.X();}))->X();
    coord_t x_min = (min_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.X() < arV2.X();}))->X();
    coord_t y_max = (max_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Y() < arV2.Y();}))->Y();
    coord_t y_min = (min_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Y() < arV2.Y();}))->Y();
    coord_t z_max = (max_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Z() < arV2.Z();}))->Z();
    coord_t z_min = (min_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Z() < arV2.Z();}))->Z();

    return CPoint((x_max+x_min)/2, (y_max+y_min)/2, (z_max+z_min)/2);
}

// TODO Y doesn't it use members?
// TODO maybe should be just cen and radius? and then we won't need mCenVector? what was that bug all about?
double CSurface::FindContainingRadius(const vector<CPoint>& arPoints){
    return (max_element(arPoints.begin(),arPoints.end(), [](const CPoint& arV1, const CPoint& arV2){return arV1.Magnitude() < arV2.Magnitude();}))->Magnitude();
}

/*----------------------------------------------- Cleaning Sub-Methods -----------------------------------------------*/

void CSurface::CleanEdges() {
    CPoint zero_point = CPoint(0, 0, 0);
    double box_R = FindContainingRadius(this->mInputPoints);
    bool is_out_of_radius = false;
    vector<CSurfaceFace> new_faces;
    for (auto face = this->mVecFaces.begin(); face != mVecFaces.end(); face++) {
        for (auto point = face->mPoints.begin(); point != face->mPoints.end(); point++) {
            if ((**point).Dist(zero_point) > box_R * 1.1) {
                is_out_of_radius = true;
            }
        }
        if (!is_out_of_radius) { //the face is inside the box Radius and should be kept
            new_faces.push_back(*face);
        }
        else
        {
            is_out_of_radius = false;
        }
    }
    this->mVecFaces = new_faces;
}

// takes the data from mVoronoi and using the mask cleans it so that only the wanted surface is left
void CSurface::CleanFaces(vector<bool>& aMask) {
    vector<CSurfaceFace> new_faces;
    size_t mask_len = aMask.size();
    size_t c_point1;
    size_t c_point2;
    for (auto it = mVecFaces.begin(); it != mVecFaces.end(); it++) {
        c_point1 = get<0>(it->mPairPoints);
        c_point2 = get<1>(it->mPairPoints);
        if (mask_len > c_point1 && mask_len > c_point2) { //the indexs are both in range and not a part of the box
            if (aMask[c_point1] != aMask[c_point2]) { //the face is a part of the surf
                new_faces.push_back(*it);
                new_faces.back().mColor = (mQuan[c_point1] + mQuan[c_point2]) / 2; //TODO 0.5* +0.5* is better
            }
        }
    }
    mVecFaces = new_faces;
}

// Do to the removal of many unneeded faces in CSurface::CleanFaces there are plenty of unused points in the data, there are also double points (two points on the exact same place),
// this function cleans both
void CSurface::CleanPoints() {
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
bool CompPointRD(const shared_ptr<CPoint>& arV1, const shared_ptr<CPoint>& arV2) {
    return ((*arV1).Dist(*arV2) <= POINT_SIMILARITY_THRESHOLD); //// TODO lambda? or something with CPoint? POINT_SIMILARITY_THRESHOLD is a CPOINT issue
}

void CSurface::CleanDoublePoints() {
    //sort the array
    std::sort(mVecPoints.begin(), mVecPoints.end(), CompPointRD);
    map < shared_ptr<CPoint>, shared_ptr<CPoint> > old_new_points; // will hold the new pointer fiting to each point
    vector<shared_ptr<CPoint> > cleaned_points;
    size_t j;                                           //TODO why size_t suddenly?
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

vector<coord_t>& CSurface::NormQuan(vector<coord_t>& arQuan, coord_t aVMin, coord_t aVMax) { //TODO should be one line with some std function
    if (arQuan.size() == 0){ //incase the user doesnt input any color
        arQuan = vector<coord_t>(mMask.size(), 1);
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
    for (vector<coord_t>::iterator it = arQuan.begin(); it != arQuan.end(); it++) {
        *it = 1- ((*it - aVMax) / (aVMin - aVMax));
        if (*it > 1) *it = 1;
        if (*it < 0) *it = 0;
    }
    return arQuan;
}


