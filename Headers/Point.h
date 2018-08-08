#pragma once

class Point
{
public:
	Point(float x, float y, float z);
	~Point();
	float getX() { return this->_x; }
	float getY() { return this->_y; }
	float getZ() { return this->_z; }
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

