

#include "Point.h"
#include "Cmath"

Point::Point(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->index = 0;
	this->faces = 0;
}


float Point::find_dis(Point P) {
	float dis = pow((this->x - P.getX()), 2) + pow((this->y - P.getY()), 2) + pow((this->z - P.getZ()), 2);
	return sqrt(dis);
}


float Point::getX() {
	return this->x;
}


float Point::getY() {
	return this->y;
}


float Point::getZ() {
	return this->z;
}


int Point::getIndex() {
	return this->index;
}


int Point::getFaces() {
	return this->faces;
}


void Point::setX(float x) {
	this->x = x;
}


void Point::setY(float y) {
	this->y = y;
}


void Point::setZ(float z) {
	this->z = z;
}


void Point::setIndex(int index) {
	this->index = index;
}


void Point::facesAdd() {
	this->faces++;
}


void Point::facesSub() {
	this->faces--;
}


bool Point::isPointDel() {

	if (this->faces == 0) {
		return true;
	}
	return false;
}


Point:: ~Point() {
}