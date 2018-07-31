
#include "Face.h"

Face::Face(vector<Point> points, float quan, int Cpoints[]){
	this->points = points;
	this->quan = quan;
	memcpy(this->Cpoints, Cpoints, 2);
}


void Face::removeFace() {
	for (vector<Point>::iterator it = this->points.begin(); it != this->points.end(); it++) {
		(*it).facesSub();
	}
}


float Face::getQuan() {
	return this->quan;
}


void Face::setQuan(float quan) {
	this->quan = quan;
}


Point Face::getPoint(int index) {
	return this->points[index];
}


int* Face::getCpoints() {
	return this->Cpoints;
}