#include "Surf.h"
using namespace std;

static const float DoublePointThreshHold = 0.0001;

Surf::Surf() {};

Surf::Surf(const Surf &surf2) {
	this->inputPoints = surf2.inputPoints;
	this->mask = surf2.mask;
	this->quan = surf2.quan;

	map < std::shared_ptr<Point>, shared_ptr<Point>> oldToNew_Points;
	this->vecPoints.clear();
	for (size_t i = 0; i < surf2.vecPoints.size(); i++) {
		this->vecPoints.push_back(std::shared_ptr<Point>(new Point(*(surf2.vecPoints[i]))));
		oldToNew_Points[surf2.vecPoints[i]] = this->vecPoints.back();
	}
	this->vecFaces.clear();
	SurfFace temp;
	for (vector<SurfFace>::const_iterator it = (surf2.vecFaces).begin(); it != surf2.vecFaces.end(); it++) {
		temp = SurfFace();
		temp.color = (*it).color;
		temp.C_points = (*it).C_points;
		temp.points.clear();
		for (vector<shared_ptr<Point>>::const_iterator pIt = it->points.begin(); pIt != it->points.end(); pIt++){
			temp.points.push_back(oldToNew_Points[*pIt]);
		}
		this->vecFaces.push_back(temp);
	}
}

Surf Surf::createSurf(vector<Point> inputpoints, vector<bool> mask, vector<float> quan, float Vmin, float Vmax) {
	Surf surf;
	surf.setInputPoints(inputpoints);
	surf.setMask(mask);
	surf.setQuan(surf.normQuan(quan, Vmin, Vmax));
	surf.runVorn();
	surf.cleanEdges();
	surf.cleanFaces(mask);
	surf.cleanPoints();
	return surf;
}

vector<Surf> Surf::createSurf(vector<Point> InputPoints, vector<vector<bool>> mask, vector<float> quan, float Vmin, float Vmax) {
	Surf temp;
	temp.setInputPoints(InputPoints);
	temp.setQuan(temp.normQuan(quan, Vmin, Vmax));
	temp.runVorn();
	temp.cleanEdges();
	vector<Surf> output;
	Surf newSurf;
	for (vector<vector<bool>>::iterator it = mask.begin(); it != mask.end(); it++) {
		newSurf = Surf(temp);
		newSurf.setMask(*it);
		newSurf.cleanFaces(*it);
		newSurf.cleanPoints();
		output.push_back(newSurf);
	}
	return output;
}

void Surf::cleanFaces(vector<bool> mask) {
	vector<SurfFace> newFaces;
	int maskLen = mask.size();
	int Cpoint1;
	int Cpoint2;
	for (vector<SurfFace>::iterator it = vecFaces.begin(); it != vecFaces.end(); it++) {
		Cpoint1 = get<0>(it->C_points);
		Cpoint2 = get<1>(it->C_points);
		if (maskLen > Cpoint1 && maskLen > Cpoint2) { //the indexs are both in range and not a part of the box
			if (mask[Cpoint1] != mask[Cpoint2]) { //the face is a part of the surf
				newFaces.push_back(*it);
				newFaces.back().color = (this->quan[Cpoint1] + this->quan[Cpoint2]) / 2;
			}
		}
	}
	this->vecFaces = newFaces;
}

void Surf::cleanPoints() {
	vector<std::shared_ptr<Point>> newPoints;
	for (vector<std::shared_ptr<Point>>::iterator it = vecPoints.begin(); it != vecPoints.end(); it++) {
		if (it->use_count() > 1) { // the points is also being called from another obj, other then this vector
			newPoints.push_back(*it);
		}
	}
	this->vecPoints = newPoints;
	RemoveDoublePoints();
}

//----------------------------------------remove doubles functions ----------------------------------------------------------------------
bool CompPoint(shared_ptr<Point> obj1, shared_ptr<Point> obj2) {
	if (abs((*obj1).getX() - (*obj2).getX()) <= DoublePointThreshHold) { //the x value is nurmallcly the same
		if (abs((*obj1).getY() - (*obj2).getY()) <= DoublePointThreshHold) { //the y value is nurmallcly the same
			if (abs((*obj1).getZ() - (*obj2).getZ()) <= DoublePointThreshHold) { //the z value is nurmallcly the same
				return true;
			}
			else
			{ // we compare the points by z to see who needs to go first
				return (*obj1).getZ() > (*obj2).getZ();
			}
		}
		else // we compare by y to see who needs to go first
		{
			return (*obj1).getY() > (*obj2).getY();
		}
	}
	else // we compare by x to see who goes first
	{
		return (*obj1).getX() > (*obj2).getX();
	}
}

void Surf::RemoveDoublePoints() {
	//sort the array
	std::sort(this->vecPoints.begin(), this->vecPoints.end(), CompPoint);
	map < std::shared_ptr<Point>, std::shared_ptr<Point>> old_new_points; // will hold the new pointer fiting to each point
	vector<shared_ptr<Point>> cleanedPoints;
	size_t j;
	for (size_t i = 0; i < this->vecPoints.size(); i++) {
		j = i + 1;
		cleanedPoints.push_back(this->vecPoints[i]); // push the point to the cleaned data
		old_new_points[this->vecPoints[i]] = this->vecPoints[i];
		while ((*this->vecPoints[i]).distance((*this->vecPoints[j])) <= DoublePointThreshHold) { //check if the point has duplicates that we need to skip
			old_new_points[this->vecPoints[j]] = this->vecPoints[i];
			j += 1;
			//cout << "removed point" << endl;
		}
		i = j - 1; //set i to the last a duplicate (ot to i if there were no duplicates).
	}
	// change the points ptrs within the faces
	for (vector<SurfFace>::iterator faceIt = this->vecFaces.begin(); faceIt != this->vecFaces.end(); faceIt++) {
		for (vector<shared_ptr<Point>>::iterator pointIt = faceIt->points.begin(); pointIt != faceIt->points.end(); pointIt++) {
			(*pointIt) = old_new_points[*pointIt];
		}
	}
	this->vecPoints = cleanedPoints;
}

//smooth functions----------------------------------------------------------------------------------------------------------

pair<vector<size_t>, vector<size_t>> _updatePoutPin(vector<size_t> Pout, vector<size_t> Pin) {
	for (int i = 0; (unsigned)i < Pout.size(); i++) {
		Pout[i] = i;
	}
	for (int i = 0; (unsigned)i < Pin.size(); i++) {
		Pin[i] = i + Pout.size();
	}
	return make_pair(Pout, Pin);
}

pair<vector<size_t>, vector<size_t>> Surf::_setPinPout() { //define pin and pout
	vector<size_t> Pout;
	vector<size_t> Pin;
	map<size_t, bool> PinMap;
	map<size_t, bool> PoutMap;
	int Cpoint1;
	int Cpoint2;
	for (vector<SurfFace>::iterator it = this->vecFaces.begin(); it != this->vecFaces.end(); it++) {
		Cpoint1 = get<0>(it->C_points);
		Cpoint2 = get<1>(it->C_points);
		if (this->mask[Cpoint1]) {
			if (PinMap.count(Cpoint1) == 0) {
				Pin.push_back(Cpoint1);
				PinMap[Cpoint1] = true;
			}
			if (PoutMap.count(Cpoint2) == 0) {
				Pout.push_back(Cpoint2);
				PoutMap[Cpoint2] = true;
			}
		}
		else
		{
			if (PinMap.count(Cpoint2) == 0) {
				Pin.push_back(Cpoint2);
				PinMap[Cpoint2] = true;
			}
			if (PoutMap.count(Cpoint1) == 0) {
				Pout.push_back(Cpoint1);
				PoutMap[Cpoint1] = true;
			}
		}
	}
	return make_pair(Pout, Pin);
}

void Surf::_updateInputPoints(vector<size_t> Pout, vector<size_t> Pin) {
	vector<Point> newPoints;
	vector<float> quan;
	for (vector<size_t>::iterator it = Pout.begin(); it != Pout.end(); it++) {
		newPoints.push_back(this->inputPoints[*it]);
		quan.push_back(this->quan[*it]);
	}
	for (vector<size_t>::iterator it = Pin.begin(); it != Pin.end(); it++) {
		newPoints.push_back(this->inputPoints[*it]);
		quan.push_back(this->quan[*it]);
	}
	this->inputPoints = newPoints;
	this->quan = quan;
}
void Surf::_makeMask(size_t PoutSize, size_t PinSize) {
	vector<bool> newMask;
	for (size_t i = 0; i < PoutSize; i++) {
		newMask.push_back(false);
	}
	for (size_t i = 0; i < PinSize; i++) {
		newMask.push_back(true);
	}
	this->mask = newMask;
}

bool CompPointData_t(Point_data_t obj1, Point_data_t obj2) {
	if (abs(obj1.point.getX() - obj2.point.getX()) <= DoublePointThreshHold) { //the x value is nurmallcly the same
		if (abs(obj1.point.getY() - obj2.point.getY()) <= DoublePointThreshHold) { //the y value is nurmallcly the same
			if (abs(obj1.point.getZ() - obj2.point.getZ()) <= DoublePointThreshHold) { //the z value is nurmallcly the same
				return true;
			}
			else 
			{ // we compare the points by z to see who needs to go first
				return obj1.point.getZ() > obj2.point.getZ();
			}
		}
		else // we compare by y to see who needs to go first
		{
			return obj1.point.getY() > obj2.point.getY();
		}
	}
	else // we compare by x to see who goes first
	{
		return obj1.point.getX() > obj2.point.getX();
	}
}
vector<Point_data_t> Surf::RemoveDoublesVornIn(vector<Point_data_t> data) {
	//sort the array
	std::sort(data.begin(), data.end(), CompPointData_t);
	vector<Point_data_t> cleanedData;
	size_t j;
	for (size_t i = 0; i < data.size(); i++) {
		j = i+1;
		cleanedData.push_back(data[i]); // push the point to the cleaned data
		while (data[i].point.distance(data[j].point) <= DoublePointThreshHold) { //check if the point has duplicates that we need to skip
			j += 1;
			//cout << "removed point" << endl;
		}
		i = j - 1; //set i to the last a duplicate (ot to i if there were no duplicates).
	}
	
	return cleanedData;
}

pair<vector<size_t>, vector<size_t>> Surf::_S2addPoints(vector<size_t> Pout, vector<size_t> Pin) { //TODO seprate this to a few difrrent function(its way too long)
	size_t Cpoint1;
	size_t Cpoint2;
	size_t PoutSize = Pout.size();
	size_t PinSize = PoutSize + Pin.size();
	float x, y, z;
	vector<Point> newPoints;
	vector<size_t> newIn;
	vector<size_t> newOut;
	vector<float> newQuan;
	size_t newIndex = 0; // the index for the new point to be added
	//go over pout
	for (vector<SurfFace>::iterator it = this->vecFaces.begin(); it != this->vecFaces.end(); it++) {
		Cpoint1 = get<0>(it->C_points);
		Cpoint2 = get<1>(it->C_points);
		if (Cpoint1 < PoutSize && Cpoint2 < PoutSize) { //pout - [1,2,3,4...pout.size] so we are checking if cpoint is a part of pout
			newOut.push_back(newIndex);
			x = (this->inputPoints[Cpoint1].getX() * 2 + this->inputPoints[Cpoint2].getX()) / 3.0;
			y = (this->inputPoints[Cpoint1].getY() * 2 + this->inputPoints[Cpoint2].getY()) / 3.0;
			z = (this->inputPoints[Cpoint1].getZ() * 2 + this->inputPoints[Cpoint2].getZ()) / 3.0;
			newPoints.push_back(Point(x, y, z));
			newQuan.push_back((this->quan[Cpoint1] + this->quan[Cpoint2]) / 2.0);
			newIndex++;

			newOut.push_back(newIndex);
			x = (this->inputPoints[Cpoint1].getX() + this->inputPoints[Cpoint2].getX() * 2) / 3.0;
			y = (this->inputPoints[Cpoint1].getY() + this->inputPoints[Cpoint2].getY() * 2) / 3.0;
			z = (this->inputPoints[Cpoint1].getZ() + this->inputPoints[Cpoint2].getZ() * 2) / 3.0;
			newPoints.push_back(Point(x, y, z));
			newQuan.push_back((this->quan[Cpoint1] + this->quan[Cpoint2]) / 2.0);
			newIndex++;
		}
		//go over pin
		if ((PinSize > Cpoint1 && Cpoint1 >= PoutSize) && (PinSize > Cpoint2 && Cpoint2 >= PoutSize)) { //pin - [pout.size...pout.size+pin.size] so we are checking if cpoint is a part of pin
			newIn.push_back(newIndex);
			x = (this->inputPoints[Cpoint1].getX() * 2 + this->inputPoints[Cpoint2].getX()) / 3.0;
			y = (this->inputPoints[Cpoint1].getY() * 2 + this->inputPoints[Cpoint2].getY()) / 3.0;
			z = (this->inputPoints[Cpoint1].getZ() * 2 + this->inputPoints[Cpoint2].getZ()) / 3.0;
			newPoints.push_back(Point(x, y, z));
			newQuan.push_back((this->quan[Cpoint1] + this->quan[Cpoint2]) / 2.0);
			newIndex++;

			newIn.push_back(newIndex);
			x = (this->inputPoints[Cpoint1].getX() + this->inputPoints[Cpoint2].getX() * 2) / 3.0;
			y = (this->inputPoints[Cpoint1].getY() + this->inputPoints[Cpoint2].getY() * 2) / 3.0;
			z = (this->inputPoints[Cpoint1].getZ() + this->inputPoints[Cpoint2].getZ() * 2) / 3.0;
			newPoints.push_back(Point(x, y, z));
			newQuan.push_back((this->quan[Cpoint1] + this->quan[Cpoint2]) / 2.0);
			newIndex++;
		}
	}
	vector<Point_data_t> data;
	data.clear();
	for (vector<size_t>::iterator it = newIn.begin(); it != newIn.end(); it++) {
		data.push_back(Point_data_t(newPoints[*it], newQuan[*it], true));
	}
	for (vector<size_t>::iterator it = newOut.begin(); it != newOut.end(); it++) {
		data.push_back(Point_data_t(newPoints[*it], newQuan[*it], false));
	}
	data = RemoveDoublesVornIn(data);
	newPoints.clear();
	newQuan.clear();
	newIn.clear();
	newOut.clear();
	for (size_t i = 0; i < data.size(); i++) {
		newPoints.push_back(data[i].point);
		newQuan.push_back(data[i].quan);
		if (data[i].isIn) {
			newIn.push_back(i);
		}
		else
		{
			newOut.push_back(i);
		}
	}
	this->inputPoints = newPoints;
	this->quan = newQuan;
	return make_pair(newOut, newIn);
}


//public functions

void Surf::smoothSurf() {
	//begin smooth part 1, collecting all the cpoints from the faces on the surf
	pair<vector<size_t>, vector<size_t>> pair = _setPinPout();
	vector<size_t> Pout = get<0>(pair);
	vector<size_t> Pin = get<1>(pair);
	_updateInputPoints(Pout, Pin);
	this->runVorn();
	//begin smooth part 2, adding new points between the cpoints
	pair = _updatePoutPin(Pout, Pin);
	Pout = get<0>(pair);
	Pin = get<1>(pair);
	pair = _S2addPoints(Pout, Pin);
	Pout = get<0>(pair);
	Pin = get<1>(pair);
	//begin smooth part 3, running the model and cleaning it
	_updateInputPoints(Pout, Pin);
	runVorn();
	cleanEdges();
	_makeMask(Pout.size(), Pin.size());
	cleanFaces(this->mask);
	cleanPoints();
	//done.
}

vector<float> Surf::normQuan(vector<float> quan, float Vmin, float Vmax) {
	if (Vmin == Vmax) { //in case where Vmin-Vmax == 0
		quan = vector<float>(quan.size(), 0.5);
		return quan;
	}
	for (vector<float>::iterator it = quan.begin(); it != quan.end(); it++) {
		*it = 1- ((*it - Vmax) / (Vmin - Vmax));
		if (*it > 1) *it = 1;
		if (*it < 0) *it = 0;
	}
	return quan;
}


const Mesh Surf::to_mesh(string label, float alpha) {
	vector<Point> points;
	size_t counter = 0;
	map < std::shared_ptr<Point>, size_t> indexes;
	for (vector<std::shared_ptr<Point>>::iterator it = this->vecPoints.begin(); it != this->vecPoints.end(); it++) {
		points.push_back(**it);
		indexes[*it] = counter;
		counter++;
	}
	vector<IndexedFace> faces;
	vector<size_t> facePoints;
	for (vector<SurfFace>::iterator it = this->vecFaces.begin(); it != this->vecFaces.end(); it++) {
		for (vector<shared_ptr<Point>>::iterator point = it->points.begin(); point != it->points.end(); point++) {
			facePoints.push_back(indexes[*point]);
		}
		faces.push_back(IndexedFace(facePoints, it->color));
		facePoints = {};
	}
	return Mesh(points, faces, label, alpha);
}

void Surf::exportToObj(string output, string label, float alpha) {
	Mesh mesh = to_mesh(label, alpha);
	mesh.operator<< (output);
}

static bool comparePoint(Point point1, Point point2) {
	double dis1 = point1.distance(Point(0, 0, 0));
	double dis2 = point2.distance(Point(0, 0, 0));
	return (dis2 > dis1);
}
static bool(*compPoint)(Point, Point) = comparePoint;

static vector<shared_ptr<Point>> convertfromvorn(vector<Vector3D> vornPoints) {
	vector<shared_ptr<Point>> newVec;
	for (vector<Vector3D>::iterator it = vornPoints.begin(); it != vornPoints.end(); it++) {
		newVec.push_back(shared_ptr<Point>(new Point(it->x, it->y, it->z)));
	}
	return newVec;
}

static double _findBoxR(vector<Point> inputPoints) {
	Point box_r = *max_element(inputPoints.begin(), inputPoints.end(), compPoint);
	return box_r.distance(Point(0, 0, 0));
}
void Surf::runVorn() {
	//find the box_r
	double box_R = _findBoxR(this->inputPoints);
	cout << "start vorn" << endl;
	pair<vector<Vector3D>, vector<vector<size_t>>> vornOut = compute_vornoi(this->inputPoints, box_R * 2);
	cout << "vorn done" << endl;
	//set the points
	this->vecPoints = convertfromvorn(get<0>(vornOut));
	//set the faces
	vector<vector<size_t>> vornFaces = get<1>(vornOut);
	vector<SurfFace> newFaces;
	size_t Cpoint1;
	size_t Cpoint2;
	float quan;
	vector<shared_ptr<Point>> facePoints;
	for (vector<vector<size_t>>::iterator face = vornFaces.begin(); face != vornFaces.end(); face++) {
		Cpoint1 = face->back();
		face->pop_back();
		Cpoint2 = face->back();
		face->pop_back();
		quan = 0; //quan will be defiened later in the program(clean faces) so for now this will be a place holder
		facePoints = vector<shared_ptr<Point>>();
		for (vector<size_t>::iterator point = face->begin(); point != face->end(); point++) {
			facePoints.push_back(this->vecPoints[*point]);
		}
		newFaces.push_back(SurfFace(facePoints, quan, pair<size_t, size_t>(Cpoint1, Cpoint2)));
	}
	this->vecFaces = newFaces;

}

void Surf::cleanEdges() {
	double box_R = _findBoxR(this->inputPoints);
	bool isOutOfRadius = false;
	vector<SurfFace> newFaces;
	for (vector<SurfFace>::iterator face = this->vecFaces.begin(); face != vecFaces.end(); face++) {
		for (vector<shared_ptr<Point>>::iterator point = face->points.begin(); point != face->points.end(); point++) {
			if ((**point).distance(Point(0, 0, 0)) > box_R * 1.1) {
				isOutOfRadius = true;
			}
		}
		if (isOutOfRadius == false) { //the face is inside the box Radius and should be kept
			newFaces.push_back(*face);
		}
		else
		{
			isOutOfRadius = false;
		}
	}
	this->vecFaces = newFaces;
 }
