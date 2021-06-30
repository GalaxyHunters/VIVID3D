#include "Surface.h"
#include "External.h"

#include <map>

using namespace vivid;
using namespace std;

const coord_t DoublePointThreshHold = 0.0001;

//CSurface() {};

CSurface::CSurface(std::vector<std::vector<double>> aInputPoints, std::vector<bool> aMask, std::vector<coord_t> aQuan,
                   coord_t aVMin, coord_t aVMax) {
    //check for input valdidlty
    if((aInputPoints.size() != aMask.size()) || (aQuan.size() != aInputPoints.size()) || (aQuan.size() != aMask.size())){
        if(aQuan.size() != 0) { // use empty()
            throw "Input error, inputted vectors (points, mask, quan) must all be the same size";
        }
    }
    //check if aMask has both False and True values
    bool is_containing_true = false;
    bool is_containing_false = false;
    for(vector<bool>::iterator it = aMask.begin(); it != aMask.end(); it++){
        if(*it == true){
            is_containing_true = true;
        }
        if(*it == false){
            is_containing_false = true;
        }
    }
    if(!is_containing_false || !is_containing_true){
        throw "Input error, mask must have both True and false values";
    }
    //model centralization
    mCenVector = FindCenPoint(aInputPoints);
    //code
    vector<CPoint> temp;
    // Why dont we set temp to be the size of aInputPoints ahead of time?
    for (auto it = aInputPoints.begin(); it != aInputPoints.end(); it++){
        temp.push_back(CPoint(*it) -= mCenVector);
    }
    temp.resize(temp.size());
	SetInputPoints(temp);
	SetMask(aMask);
	SetQuan(NormQuan(aQuan, aVMin, aVMax));

	// freeing up aInputPoints from memory, at this point it's not needed.
	aInputPoints.clear();
	aInputPoints.shrink_to_fit();

	RunVorn();
	CleanEdges();
	CleanFaces(aMask);
	CleanPoints();
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
// ---------------------------------------------------Centralization functions ---------------------------------------------------------------
// when the data is entered its not insured to be centered around 0, this function insures that it is (the model will be returned to his original axis upon export)
inline bool ComparePointX(const vector<double> &aPoint1, const vector<double> &aPoint2){return (aPoint1[0] < aPoint2[0]);}
inline bool ComparePointY(const vector<double> &aPoint1, const vector<double> &aPoint2){return (aPoint1[1] < aPoint2[1]);}
inline bool ComparePointZ(const vector<double> &aPoint1, const vector<double> &aPoint2){return (aPoint1[2] < aPoint2[2]);}

// TODO: Why doesn't operator overload work here??
inline bool CompareCPointX(CPoint &aPoint1, CPoint &aPoint2){return (aPoint1.X() < aPoint2.X());}
inline bool CompareCPointY(CPoint &aPoint1, CPoint &aPoint2){return (aPoint1.Y() < aPoint2.Y());}
inline bool CompareCPointZ(CPoint &aPoint1, CPoint &aPoint2){return (aPoint1.Z() < aPoint2.Z());}

//define function that finds model center point and radius of points.
CPoint CSurface::FindCenPoint(const vector<vector<double>> &aInputPoints){
    coord_t MaxX = (*max_element(aInputPoints.begin(), aInputPoints.end(), *ComparePointX))[0];
    coord_t MinX = (*min_element(aInputPoints.begin(), aInputPoints.end(), *ComparePointX))[0];

    coord_t MaxY = (*max_element(aInputPoints.begin(), aInputPoints.end(), *ComparePointY))[1];
    coord_t MinY = (*min_element(aInputPoints.begin(), aInputPoints.end(), *ComparePointY))[1];

    coord_t MaxZ = (*max_element(aInputPoints.begin(), aInputPoints.end(), *ComparePointZ))[2];
    coord_t MinZ = (*min_element(aInputPoints.begin(), aInputPoints.end(), *ComparePointZ))[2];

    CPoint CenPoint((MaxX + MinX)/2, (MaxY + MinY)/2, (MaxZ + MinZ)/2);
    return CenPoint;
}


// ------------------------------------------------------clean data----------------------------------------------------------------------

// takes the data from voronoi and using the mask cleans it so that only the wanted surface is left
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
	RemoveDoublePoints();
}

//----------------------------------------remove double points (two points on the exact same place) functions ----------------------------------------------------------------------
bool CompPointRD(shared_ptr<CPoint> aObj1, shared_ptr<CPoint> aObj2) {
	if (abs((*aObj1).X() - (*aObj2).X()) <= DoublePointThreshHold) { //the x value is nurmallcly the same
		if (abs((*aObj1).Y() - (*aObj2).Y()) <= DoublePointThreshHold) { //the y value is nurmallcly the same
			if (abs((*aObj1).Z() - (*aObj2).Z()) <= DoublePointThreshHold) { //the z value is nurmallcly the same
				return false;
			}
			else
			{ // we compare the points by z to see who needs to go first
				return (*aObj1).Z() > (*aObj2).Z();
			}
		}
		else // we compare by y to see who needs to go first
		{
			return (*aObj1).Y() > (*aObj2).Y();
		}
	}
	else // we compare by x to see who goes first
	{
		return (*aObj1).X() > (*aObj2).X();
	}
}

void CSurface::RemoveDoublePoints() {
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
		while ((*mVecPoints[i]).Dist((*mVecPoints[j])) <= DoublePointThreshHold) { //check if the point has duplicates that we need to skip
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

//--------------------------------------------------------------------smooth functions----------------------------------------------------------------------------------------------------------
//TODO write smooth algorithem
void UpdatePoutPin(vector<size_t>& aPOut, vector<size_t>& aPIn) {
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
	this->mInputPoints = new_points;
	this->mQuan = quan;
}
void CSurface::MakeMask(size_t aPOutSize, size_t aPInSize) {
	vector<bool> new_mask;
	for (size_t i = 0; i < aPOutSize; i++) {
		new_mask.push_back(false);
	}
	for (size_t i = 0; i < aPInSize; i++) {
		new_mask.push_back(true);
	}
	this->mMask = new_mask;
}

bool CompPointData_t(CSurfacePoint aObj1, CSurfacePoint aObj2) {
	if (abs(aObj1.mPoint.X() - aObj2.mPoint.X()) <= DoublePointThreshHold) { //the x value is nurmallcly the same
		if (abs(aObj1.mPoint.Y() - aObj2.mPoint.Y()) <= DoublePointThreshHold) { //the y value is nurmallcly the same
			if (abs(aObj1.mPoint.Z() - aObj2.mPoint.Z()) <= DoublePointThreshHold) { //the z value is nurmallcly the same
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
	this->mInputPoints = arNewPoints;
	this->mQuan = arNewQuan;
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
		while (arData[i].mPoint.Dist(arData[j].mPoint) <= DoublePointThreshHold) { //check if the point has duplicates that we need to skip
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
	coord_t x, y, z;
	(*apPVec).push_back(*apNewIndex);
	x = (this->mInputPoints[aCPoint1].X() * 2 + this->mInputPoints[aCPoint2].X()) / 3.0;
	y = (this->mInputPoints[aCPoint1].Y() * 2 + this->mInputPoints[aCPoint2].Y()) / 3.0;
	z = (this->mInputPoints[aCPoint1].Z() * 2 + this->mInputPoints[aCPoint2].Z()) / 3.0;
	(*apNewPoints).push_back(CPoint(x, y, z));
	(*apNewQuan).push_back((this->mQuan[aCPoint1] + this->mQuan[aCPoint2]) / 2.0);
	(*apNewIndex)++;

	(*apPVec).push_back(*apNewIndex);
	x = (this->mInputPoints[aCPoint1].X() + this->mInputPoints[aCPoint2].X() * 2) / 3.0;
	y = (this->mInputPoints[aCPoint1].Y() + this->mInputPoints[aCPoint2].Y() * 2) / 3.0;
	z = (this->mInputPoints[aCPoint1].Z() + this->mInputPoints[aCPoint2].Z() * 2) / 3.0;
	(*apNewPoints).push_back(CPoint(x, y, z));
	(*apNewQuan).push_back((this->mQuan[aCPoint1] + this->mQuan[aCPoint2]) / 2.0);
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


//public functions

void CSurface::SmoothSurf() {
	//begin smooth part 1, collecting all the cpoints from the faces on the surf
    vector<size_t> p_out;
    vector<size_t> p_in;
    SetPinPout(p_out, p_in);
	UpdateInputPoints(p_out, p_in);
	RunVorn();
	//begin smooth part 2, adding new points between the cpoints
	UpdatePoutPin(p_out, p_in);
	Stage2AddPoints(p_out, p_in);
	//begin smooth part 3, running the model and cleaning it
	UpdateInputPoints(p_out, p_in);
	RunVorn();
	CleanEdges();
	MakeMask(p_out.size(), p_in.size());
	CleanFaces(mMask);
	CleanPoints();
	//done.
}

vector<coord_t>& CSurface::NormQuan(vector<coord_t>& arQuan, coord_t aVMin, coord_t aVMax) {
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


const CMesh CSurface::ToMesh(string aLabel, coord_t aAlpha) {
    //check input valdilty
    if(aAlpha > 1 || aAlpha < 0){
        throw "Alpha must be between 0 and 1";
    }
	vector<CPoint> points;
	size_t counter = 0;
	map < shared_ptr<CPoint>, size_t> indexes;
	for (auto it = this->mVecPoints.begin(); it != this->mVecPoints.end(); it++) {
		points.push_back(**it);
		indexes[*it] = counter;
		counter++;
	}
	vector<CIndexedFace> faces;
	vector<size_t> face_points;
	set<size_t> set_points;
	for (auto it = this->mVecFaces.begin(); it != this->mVecFaces.end(); it++) {
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
	return CMesh(points, faces, aLabel, aAlpha, this->mCenVector);
}

void CSurface::ExportToObj(string aOutput, string aLabel, coord_t aAlpha) {
	CMesh mesh = ToMesh(aLabel, aAlpha);
	mesh.ExportToObj(aOutput);
}

bool ComparePoint(CPoint &aPoint1, CPoint &aPoint2) { //TODO (TOMER) Move to CPoint
    CPoint zeroPoint(0, 0, 0);
	double dis1 = aPoint1.Dist(zeroPoint);
	double dis2 = aPoint2.Dist(zeroPoint);
	return (dis2 > dis1);
}

//static bool(*CompPoint)(&CPoint, &CPoint) = ComparePoint;

vector<shared_ptr<CPoint> > ConvertFromVorn(vector<Vector3D> aVornPoints) {
	vector<shared_ptr<CPoint> > new_vec;
	for (auto it = aVornPoints.begin(); it != aVornPoints.end(); it++) {
		new_vec.push_back(shared_ptr<CPoint>(new CPoint(it->x, it->y, it->z)));
	}
	return new_vec;
}

// TODO: Add alt Box func
static double FindBoxR(vector<CPoint>& aInputPoints) { //TODO shouldn't be static!!!
    CPoint zeroPoint(0, 0, 0);
	CPoint box_r = *max_element(aInputPoints.begin(), aInputPoints.end(), *ComparePoint);
	return box_r.Dist(zeroPoint);
}

static vector<Vector3D> FindBox(vector<CPoint> aInputPoints) {
    coord_t xmax = (*max_element(aInputPoints.begin(), aInputPoints.end(), *CompareCPointX)).X();
    coord_t xmin = (*min_element(aInputPoints.begin(), aInputPoints.end(), *CompareCPointX)).X();

    coord_t ymax = (*max_element(aInputPoints.begin(), aInputPoints.end(), *CompareCPointY)).Y();
    coord_t ymin = (*min_element(aInputPoints.begin(), aInputPoints.end(), *CompareCPointY)).Y();

    coord_t zmax = (*max_element(aInputPoints.begin(), aInputPoints.end(), *CompareCPointZ)).Z();
    coord_t zmin = (*min_element(aInputPoints.begin(), aInputPoints.end(), *CompareCPointZ)).Z();

    vector<Vector3D> Box (2);

    const double expandFactor = 1;

    Vector3D ll (xmin - (xmax - xmin) * expandFactor, ymin - (ymax - ymin) * expandFactor, zmin - (zmax - zmin) * expandFactor);
    Vector3D ur (xmax + (xmax - xmin) * expandFactor, ymax + (ymax - ymin) * expandFactor, zmax + (zmax - zmin) * expandFactor);

    Box.at(0) = ll;
    Box.at(1) = ur;

    return Box;
}

// TODO: Implement alt box func and alt compute vornoi func
void CSurface::RunVorn() {
	//find the box_r
	double box_R = FindBoxR(mInputPoints);
	cout << "BoxR = " << box_R << endl;
	// New version:
	vector<Vector3D> Box = FindBox(this->mInputPoints);
	cout << "Box[0] Elad = " << Box.at(0).x << " " << Box.at(0).y << " " << Box.at(0).z << endl;
    cout << "Box[1] Elad = " << Box.at(1).x << " " << Box.at(1).y << " " << Box.at(1).z << endl;

	cout << "start vorn" << endl;
    //auto vorn_out = compute_vornoi(this->mInputPoints, box_R*2);
    auto vorn_out = compute_vornoi(mInputPoints, 2*box_R );
	cout << "vorn done" << endl;
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


void CSurface::CleanEdges() {
    CPoint zero_point = CPoint(0, 0, 0);
	double box_R = FindBoxR(this->mInputPoints);
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

