#ifndef POINT_H
#define POINT_H

#include <cmath>

class Point
{
public:
	inline Point(){}
	inline Point(float x, float y, float z);
	inline ~Point();
	inline double distance(Point p) { return sqrt(pow(this->_x - p.getX(), 2) + pow(this->_y - p.getY(), 2) + pow(this->_z - p.getZ(), 2)); }
	inline float getX() { return this->_x; }
	inline float getY() { return this->_y; }
	inline float getZ() { return this->_z; }
	inline void setX(float x) { this->_x = x; }
	inline void setY(float y) { this->_y = y; }
	inline void setZ(float z) { this->_z = z; }
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

#endif
