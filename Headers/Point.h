#pragma once
#include <cmath>

class Point
{
public:
	Point(float x, float y, float z);
	~Point();
	double distance(Point p) { return sqrt(pow(this->_x - p.getX(), 2) + pow(this->_y - p.getY(), 2) + pow(this->_z - p.getZ(), 2)); }
	float getX() { return this->_x; }
	float getY() { return this->_y; }
	float getZ() { return this->_z; }
	void setX(float x) { this->_x = x; }
	void setY(float y) { this->_y = y; }
	void setZ(float z) { this->_z = z; }
	//float getDis(Point p) { return (); }
private:
	float _x, _y, _z;
};

Point::Point(float x, float y, float z) : _x(x), _y(y), _z(z)
{
}

Point::~Point()
{
}

