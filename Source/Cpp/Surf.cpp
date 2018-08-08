#include "surf.h"

Surf::Surf() {};


void Surf::cleanFaces(vector<bool> mask) {
		vector<SurfFace> newfaces;
		int masklen = mask.size();
		int cpoint1;
		int cpoint2;
		for (vector<SurfFace>::iterator it = vecFaces.begin(); it != vecFaces.end(); it++) {
			cpoint1 = get<0>(it->C_points);
			cpoint2 = get<1>(it->C_points);
			if (masklen > cpoint1 && masklen > cpoint2) { //the indexs are both in range and not a part of the box
				if (mask[cpoint1] != mask[cpoint2]) { //the face is a part of the surf
					newfaces.push_back(*it);
				}
			}
		}
		this->vecFaces = newfaces;
}

void Surf::cleanPoints() {
	vector<std::shared_ptr<Point>> newPoints;
	for (vector<std::shared_ptr<Point>>::iterator it = vecPoints.begin(); it != vecPoints.end(); it++) {
		if (it->use_count > 1) { // the points is also being called from another obj, other then this vector
			newPoints.push_back(*it);
		}
	}
	this->vecPoints = newPoints;
}


//smooth functions:

void _updatePoutPin(vector<int> Pout, vector<int> Pin) {
	for (int i = 0; i < Pout.size(); i++) {
		Pout[i] = i;
	}
	for (int i = 0; i < Pin.size(); i++) {
		Pin[i] = i + Pout.size();
	}
}

void Surf::_setPinPout(vector<int> pout, vector<int> pin) { //define pin and pout
	int cpoint1;
	int cpoint2;
	for (vector<SurfFace>::iterator it = this->vecFaces.begin(); it != this->vecFaces.end(); it++) {
		cpoint1 = get<0>(it->C_points);
		cpoint2 = get<1>(it->C_points);
		if (this->mask[cpoint1]) {
			pin.push_back(cpoint1);
			pout.push_back(cpoint2);
		}
		else 
		{
			pin.push_back(cpoint2);
			pout.push_back(cpoint1);
		}

	}
}

void Surf::_updateInputPoints(vector<int> pout, vector<int> pin) {
	vector<double[3]> newpoints;
	vector<float> quan;
	newpoints.resize(pout.size() + pin.size());
	quan.resize(pout.size() + pin.size());
	for (vector<int>::iterator it = pout.begin(); it != pout.end(); it++) {
		newpoints.push_back(this->inputPoints[*it]);
		quan.push_back(this->quan[*it]);
	}
	for (vector<int>::iterator it = pin.begin(); it != pin.end(); it++) {
		newpoints.push_back(this->inputPoints[*it]);
		quan.push_back(this->quan[*it]);
	}
	this->inputPoints = newpoints;
	this->quan = quan;
}
void Surf::_makeMask(int pouts, int pins) {
	vector<bool> newmask;
	newmask.resize(pouts + pins);
	for (int i; i < pouts; i++) {
		newmask.push_back(false);
	}
	for (int i; i < pins; i++) {
		newmask.push_back(true);
	}
	this->mask = mask;
}

void Surf::_S2addPoints(vector<int> pout, vector<int> pin) {
	int cpoint1;
	int cpoint2;
	int sizePout = pout.size();
	double newPoint[3];
	int newIndex = this->inputPoints.size(); // the index for the new point to be added
	//go over pout
	for (vector<SurfFace>::iterator it = this->vecFaces.begin(); it != this->vecFaces.end(); it++) {
		cpoint1 = get<0>(it->C_points);
		cpoint2 = get<1>(it->C_points);
		if (cpoint1 < sizePout && cpoint2 < sizePout) { //pout - [1,2,3,4...pout.size] so we are checking if cpoint is a part of pout
			pout.push_back(newIndex);
			newPoint[0] = (this->inputPoints[cpoint1][0] * 2 + this->inputPoints[cpoint2][0]) / 3.0;
			newPoint[1] = (this->inputPoints[cpoint1][1] * 2 + this->inputPoints[cpoint2][1]) / 3.0;
			newPoint[2] = (this->inputPoints[cpoint1][2] * 2 + this->inputPoints[cpoint2][2]) / 3.0;
			this->inputPoints.push_back(newPoint);
			this->quan.push_back((this->quan[cpoint1] + this->quan[cpoint2]) / 2.0);
			newIndex++;

			pout.push_back(newIndex);
			newPoint[0] = (this->inputPoints[cpoint1][0] + this->inputPoints[cpoint2][0] * 2) / 3.0;
			newPoint[1] = (this->inputPoints[cpoint1][1] + this->inputPoints[cpoint2][1] * 2) / 3.0;
			newPoint[2] = (this->inputPoints[cpoint1][2] + this->inputPoints[cpoint2][2] * 2) / 3.0;
			this->inputPoints.push_back(newPoint);
			this->quan.push_back((this->quan[cpoint1] + this->quan[cpoint2]) / 2.0);
			newIndex++;
		}
	}
	int sizePin = sizePout + pin.size();
	//go over pin
	for (vector<SurfFace>::iterator it = this->vecFaces.begin(); it != this->vecFaces.end(); it++) {
		cpoint1 = get<0>(it->C_points);
		cpoint2 = get<1>(it->C_points);
		if (sizePin > cpoint1 > sizePout && sizePin > cpoint2 > sizePout) { //pin - [pout.size...pout.size+pin.size] so we are checking if cpoint is a part of pin
			pin.push_back(newIndex);
			newPoint[0] = (this->inputPoints[cpoint1][0] * 2 + this->inputPoints[cpoint2][0]) / 3.0;
			newPoint[1] = (this->inputPoints[cpoint1][1] * 2 + this->inputPoints[cpoint2][1]) / 3.0;
			newPoint[2] = (this->inputPoints[cpoint1][2] * 2 + this->inputPoints[cpoint2][2]) / 3.0;
			this->inputPoints.push_back(newPoint);
			this->quan.push_back((this->quan[cpoint1] + this->quan[cpoint2]) / 2.0);
			newIndex++;

			pin.push_back(newIndex);
			newPoint[0] = (this->inputPoints[cpoint1][0] + this->inputPoints[cpoint2][0] * 2) / 3.0;
			newPoint[1] = (this->inputPoints[cpoint1][1] + this->inputPoints[cpoint2][1] * 2) / 3.0;
			newPoint[2] = (this->inputPoints[cpoint1][2] + this->inputPoints[cpoint2][2] * 2) / 3.0;
			this->inputPoints.push_back(newPoint);
			this->quan.push_back((this->quan[cpoint1] + this->quan[cpoint2]) / 2.0);
			newIndex++;
		}
	}
}


//public functions

void Surf::smoothSurf() {
	vector<int> Pout;
	vector<int> Pin;
	//begin smooth part 1, collecting all the cpoints from the faces on the surf
	_setPinPout(Pout, Pin);
	_updateInputPoints(Pout, Pin);
	runvorn(); //need to import vorn from utils.h
	//begin smooth part 2, adding new points between the cpoints
	_updatePoutPin(Pout, Pin);
	_S2addPoints(Pout, Pin);
	//begin smooth part 3, running the model, clearing it anf finished
	_updateInputPoints(Pout, Pin);
	runvorn(); //need to import vorn from utils.h
	_makeMask(Pout.size(), Pin.size());
	cleanFaces(this->mask);
	cleanPoints();
	//done.
}

Surf Surf::createSurf(vector<double[3]> inputpoints, vector<bool> mask, vector<float> quan) {
	Surf surf;
	this->inputPoints = inputpoints;
	this->mask = mask;
	this->quan = quan;
	surf.runvorn(); //need to import vorn from utils.h
	surf.cleanFaces(mask);
	surf.cleanPoints();
	return surf;
}

const Mesh Surf::to_mesh(string label, float alpha) {
	vector<Point> points;
	size_t counter = 0;
	map < std::shared_ptr<Point > , size_t > indexes;
	for (vector<std::shared_ptr<Point>>::iterator it = this->vecPoints.begin(); it != this->vecPoints.end(); it++) {
		points.push_back(**it);
		indexes[*it] = counter;

	}
	vector<IndexedFace> faces;
	vector<size_t> facePoints;
	for (vector<SurfFace>::iterator it = this->vecFaces.begin(); it != this->vecFaces.end(); it++) {
		for (vector<shared_ptr<Point>>::iterator point = it->points.begin(); point != it->points.end(); it++) {
			facePoints.push_back(indexes[*point]);
		}
		faces.push_back(IndexedFace(facePoints, it->color));
	}
	return Mesh(points, faces, label, alpha);
}

void Surf::exportToObj(string output, string label, float alpha) {
	Mesh mesh = to_mesh(label, alpha);
	mesh.operator<< (output);
}

static bool comparePoint(double point1[3], double point2[3]) {
	double max1 = max(max(abs(point1[0]), abs(point1[1])), max(abs(point1[1]), abs(point1[2])));
	double max2 = max(max(abs(point2[0]), abs(point2[1])), max(abs(point2[1]), abs(point2[2])));
	return (max(max1, max2));
}
bool(*compPoint)(double*, double*) = comparePoint;

static vector<shared_ptr<Point>> convertfromvorn(vector<Vector3D> vornPoints) {
	vector<shared_ptr<Point>> newVec;
	for (vector<Vector3D>::iterator it = vornPoints.begin(); it != vornPoints.end(); it++) {;
		newVec.push_back(shared_ptr<Point> (Point(it->x, it->y, it->z)));
	}
	return newVec;
}

void Surf::runVorn() {
	//find the box_r
	double * box_r = { *max_element(this->inputPoints.begin(), this->inputPoints.end(), compPoint) };
	double box_R = max(max(abs(box_r[0]), abs(box_r[1])), max(abs(box_r[1]), abs(box_r[2])));
	pair<vector<Vector3D>, vector<vector<size_t>>> vornOut = compute_vornoi(this->inputPoints, box_R * 3);
	this->vecPoints = convertfromvorn(get<0>(vornOut));
}
//surf::surf(vector<double[3]> inputpoints, vector<polyface> vecfaces, vector<point> vecpoints, string label, float alpha) {
//	this->inputpoints = inputpoints;
//	this->vecfaces = vecfaces;
//	this->vecpoints = vecpoints;
//	this->label = label;
//	this->alpha = alpha;
//}
//
//void surf::cleanfaces(vector<bool> mask) { //todo add clearedges
//	vector<polyface> newfaces;
//	int masklen = mask.size();
//	int cpoint1;
//	int cpoint2;
//	for (vector<polyface>::iterator it = vecfaces.begin(); it != vecfaces.end(); it++) {
//		cpoint1 = it->getcpoints()[0];
//		cpoint2 = it->getcpoints()[1];
//		if (masklen > cpoint1 && masklen > cpoint2) { //the indexs are both in range and not a part of the box
//			if (mask[cpoint1] != mask[cpoint2]) { //the face is a part of the surf
//				newfaces.push_back(*it);
//			}
//		}
//	}
//	this->vecfaces = newfaces;
//}
//
//string surf::getlabel() {
//	return this->label;
//}
//
//void surf::setlabel(string label) {
//	this->label = label;
//}
//
//float surf::getalpha() {
//	return this->alpha;
//}
//
//void surf::setalpha(float alpha) {
//	this->alpha = alpha;
//}
//
//static bool comparepoint(double point1[3], double point2[3]) {
//	double max1 = max(max(abs(point1[0]), abs(point1[1])), max(abs(point1[1]), abs(point1[2])));
//	double max2 = max(max(abs(point2[0]), abs(point2[1])), max(abs(point2[1]), abs(point2[2])));
//	return (max(max1, max2));
//}
//bool(*comppoint)(double*, double*) = comparepoint;
//
//
//static vector<vector3d> converttovorn(vector<double[3]> inputpoints) {
//	vector<vector3d> newvec;
//	for (vector<double[3]>::iterator it = inputpoints.begin(); it != inputpoints.end(); it++) {
//		newvec.push_back(vector3d(*it[0], *it[1], *it[2]));
//	}
//	return newvec;
//}
//
//static vector<point> convertfromvorn(vector<vector3d> vornpoints) {
//	vector<point> newvec;
//	for (vector<vector3d>::iterator it = vornpoints.begin(); it != vornpoints.end(); it++) {
//		newvec.push_back(point(it->x, it->y, it->z));
//	}
//	return newvec;
//}
//
//void surf::runvorn() {
//	//find the box_r
//	double * box_r = { *max_element(this->inputpoints.begin(), this->inputpoints.end(), comppoint) };
//	double box_r = max(max(abs(box_r[0]), abs(box_r[1])), max(abs(box_r[1]), abs(box_r[2])));
//	//runvorn
//	voronoi3d temp(vector3d(box_r, box_r, box_r), vector3d(-box_r, -box_r, -box_r));//define vornoi
//	vector<vector3d> vornpoints = converttovorn(this->inputpoints);
//	temp.build(vornpoints);
//	//define vecpoints
//	vornpoints = temp.getfacepoints();
//	this->vecpoints = convertfromvorn(vornpoints);
//	//define vecfaces(dont forget quan)
//	size_t totalcells = temp.getallcellfaces().size();
//	vector<size_t> cell;
//	vector<polyface> newfaces;
//	for (size_t i = 0; i < totalcells; i++) {
//		cell = temp.getcellfaces(i);
//		for (vector<size_t>::iterator face = cell.begin(); face != cell.end(); face++) {
//			size_t cpoint1 = get<0>(temp.getfaceneighbors(*face));
//			size_t cpoint2 = get<1>(temp.getfaceneighbors(*face));
//			if (!(cpoint1 < i) && !(cpoint2 < i)) { //the face doent belong to a cell we read already
//				vector<point> points;
//				vector<size_t> vornpoints = temp.getpointsinface(*face);
//				for (vector<size_t>::iterator point = cell.begin(); point != cell.end(); point++) {
//					points.push_back(this->vecpoints[*point]);
//				}
//				float quan = this->quan[cpoint1] + this->quan[cpoint2] / 2;
//				size_t cpoints[2] = { cpoint1, cpoint2 };
//				newfaces.push_back(polyface(points, quan, cpoints));
//			}
//		}
//	}
//	this->vecfaces = newfaces;
//
//}
//
//surf surf::createsurf(vector<double[3]> inputpoints, vector<bool> mask, vector<float> quan, string label, float alpha) {
//	surf surf;
//	this->inputpoints = inputpoints;
//	this->mask = mask;
//	this->quan = quan;
//	surf.runvorn();
//	surf.cleanfaces(mask);
//	surf.removepoints();
//	surf.setlabel(label);
//	surf.setalpha(alpha);
//	return surf;
//}
//
//void _updatepoutpin(vector<int> pout, vector<int> pin) {
//	for (int i = 0; i < pout.size(); i++) {
//		pout[i] = i;
//	}
//	for (int i = 0; i < pin.size(); i++) {
//		pin[i] = i + pout.size();
//	}
//}
//
//void surf::smoothsurf() {
//	vector<int> pout;
//	vector<int> pin;
//	//begin smooth part 1, collecting all the cpoints from the faces on the surf
//	_setpinpout(pout, pin);
//	_updateinputpoints(pout, pin);
//	runvorn();
//	//begin smooth part 2, adding new points between the cpoints
//	_updatepoutpin(pout, pin);
//	_s2addpoints(pout, pin);
//	//begin smooth part 3, running the model, clearing it anf finished
//	_updateinputpoints(pout, pin);
//	runvorn();
//	_makemask(pout.size(), pin.size());
//	cleanfaces(this->mask);
//	removepoints();
//	//done.
//}
//
//void surf::_setpinpout(vector<int> pout, vector<int> pin) { //define pin and pout
//	int cpoint1;
//	int cpoint2;
//	for (vector<polyface>::iterator it = this->vecfaces.begin(); it != this->vecfaces.end(); it++) {
//		cpoint1 = it->getcpoints()[0];
//		cpoint2 = it->getcpoints()[1];
//		if (this->mask[cpoint1]) {
//			pin.push_back(cpoint1);
//			pout.push_back(cpoint2);
//		}
//		else 
//		{
//			pin.push_back(cpoint2);
//			pout.push_back(cpoint1);
//		}
//
//	}
//}
//
//void surf::_updateinputpoints(vector<int> pout, vector<int> pin) {
//	vector<double[3]> newpoints;
//	vector<float> quan;
//	newpoints.resize(pout.size() + pin.size());
//	quan.resize(pout.size() + pin.size());
//	for (vector<int>::iterator it = pout.begin(); it != pout.end(); it++) {
//		newpoints.push_back(this->inputpoints[*it]);
//		quan.push_back(this->quan[*it]);
//	}
//	for (vector<int>::iterator it = pin.begin(); it != pin.end(); it++) {
//		newpoints.push_back(this->inputpoints[*it]);
//		quan.push_back(this->quan[*it]);
//	}
//	this->inputpoints = newpoints;
//	this->quan = quan;
//}
//void surf::_makemask(int pouts, int pins) {
//	vector<bool> newmask;
//	newmask.resize(pouts + pins);
//	for (int i; i < pouts; i++) {
//		newmask.push_back(false);
//	}
//	for (int i; i < pins; i++) {
//		newmask.push_back(true);
//	}
//	this->mask = mask;
//}
//
//void surf::_s2addpoints(vector<int> pout, vector<int> pin) {
//	int cpoint1;
//	int cpoint2;
//	int sizepout = pout.size();
//	double newpoint[3];
//	int newindex = this->inputpoints.size(); // the index for the new point to be added
//	//go over pout
//	for (vector<polyface>::iterator it = this->vecfaces.begin(); it != this->vecfaces.end(); it++) {
//		cpoint1 = it->getcpoints()[0];
//		cpoint2 = it->getcpoints()[1];
//		if (cpoint1 < sizepout && cpoint2 < sizepout) { //pout - [1,2,3,4...pout.size] so we are checking if cpoint is a part of pout
//			pout.push_back(newindex);
//			newpoint[0] = (this->inputpoints[cpoint1][0] * 2 + this->inputpoints[cpoint2][0]) / 3.0;
//			newpoint[1] = (this->inputpoints[cpoint1][1] * 2 + this->inputpoints[cpoint2][1]) / 3.0;
//			newpoint[2] = (this->inputpoints[cpoint1][2] * 2 + this->inputpoints[cpoint2][2]) / 3.0;
//			this->inputpoints.push_back(newpoint);
//			this->quan.push_back((this->quan[cpoint1] + this->quan[cpoint2]) / 2.0);
//			newindex++;
//
//			pout.push_back(newindex);
//			newpoint[0] = (this->inputpoints[cpoint1][0] + this->inputpoints[cpoint2][0] * 2) / 3.0;
//			newpoint[1] = (this->inputpoints[cpoint1][1] + this->inputpoints[cpoint2][1] * 2) / 3.0;
//			newpoint[2] = (this->inputpoints[cpoint1][2] + this->inputpoints[cpoint2][2] * 2) / 3.0;
//			this->inputpoints.push_back(newpoint);
//			this->quan.push_back((this->quan[cpoint1] + this->quan[cpoint2]) / 2.0);
//			newindex++;
//		}
//	}
//	int sizepin = sizepout + pin.size();
//	//go over pin
//	for (vector<polyface>::iterator it = this->vecfaces.begin(); it != this->vecfaces.end(); it++) {
//		cpoint1 = it->getcpoints()[0];
//		cpoint2 = it->getcpoints()[1];
//		if (sizepin > cpoint1 > sizepout && sizepin > cpoint2 > sizepout) { //pin - [pout.size...pout.size+pin.size] so we are checking if cpoint is a part of pin
//			pin.push_back(newindex);
//			newpoint[0] = (this->inputpoints[cpoint1][0] * 2 + this->inputpoints[cpoint2][0]) / 3.0;
//			newpoint[1] = (this->inputpoints[cpoint1][1] * 2 + this->inputpoints[cpoint2][1]) / 3.0;
//			newpoint[2] = (this->inputpoints[cpoint1][2] * 2 + this->inputpoints[cpoint2][2]) / 3.0;
//			this->inputpoints.push_back(newpoint);
//			this->quan.push_back((this->quan[cpoint1] + this->quan[cpoint2]) / 2.0);
//			newindex++;
//
//			pin.push_back(newindex);
//			newpoint[0] = (this->inputpoints[cpoint1][0] + this->inputpoints[cpoint2][0] * 2) / 3.0;
//			newpoint[1] = (this->inputpoints[cpoint1][1] + this->inputpoints[cpoint2][1] * 2) / 3.0;
//			newpoint[2] = (this->inputpoints[cpoint1][2] + this->inputpoints[cpoint2][2] * 2) / 3.0;
//			this->inputpoints.push_back(newpoint);
//			this->quan.push_back((this->quan[cpoint1] + this->quan[cpoint2]) / 2.0);
//			newindex++;
//		}
//	}
//}