#include "Surf.h"


using namespace std;
Surf::Surf() {};


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
}


//smooth functions:

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
	runVorn();
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
	_makeMask(Pout.size(), Pin.size());
	cleanFaces(this->mask);
	cleanPoints();
	//done.
}

Surf Surf::createSurf(vector<Point> inputpoints, vector<bool> mask, vector<float> quan) {
	Surf surf;
	surf.setInputPoints(inputpoints);
	surf.setMask(mask);
	surf.setQuan(quan);
	surf.runVorn();
	surf.cleanFaces(mask);
	surf.cleanPoints();
	return surf;
}

const Mesh Surf::to_mesh(string label, float alpha) {
	vector<Point> points;
	size_t counter = 0;
	map < std::shared_ptr<Point> , size_t> indexes;
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
	double max1 = max(max(abs(point1.getX()), abs(point1.getY())), max(abs(point1.getY()), abs(point1.getZ())));
	double max2 = max(max(abs(point2.getX()), abs(point2.getY())), max(abs(point2.getY()), abs(point2.getZ())));
	return (max2 > max(max1, max2));
}
static bool(*compPoint)(Point, Point) = comparePoint;

static vector<shared_ptr<Point>> convertfromvorn(vector<Vector3D> vornPoints) {
	vector<shared_ptr<Point>> newVec;
	for (vector<Vector3D>::iterator it = vornPoints.begin(); it != vornPoints.end(); it++) {
		newVec.push_back(shared_ptr<Point>(new Point(it->x, it->y, it->z)));
	}
	return newVec;
}

void Surf::runVorn() {
	//find the box_r
	Point box_r = *max_element(this->inputPoints.begin(), this->inputPoints.end(), compPoint);
	double box_R = max(max(abs(box_r.getX()), abs(box_r.getY())), max(abs(box_r.getY()), abs(box_r.getZ())));
	cout << "start vorn" << endl;
	pair<vector<Vector3D>, vector<vector<size_t>>> vornOut = compute_vornoi(this->inputPoints, box_R * 3);
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
