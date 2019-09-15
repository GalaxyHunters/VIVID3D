
#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <iostream>
#include <vector>
typedef double cord_t;

class CPoint
{
public:
	inline CPoint(){}
	inline CPoint(const CPoint &aPoint2) : mX(aPoint2.mX), mY(aPoint2.mY), mZ(aPoint2.mZ) {} //copy constructor
	inline CPoint(cord_t x, cord_t y, cord_t z);
	inline CPoint(std::vector<cord_t > cords) : mX(cords[0]), mY(cords[1]), mZ(cords[2]) {}
	inline ~CPoint();
	inline cord_t CalcDistance(CPoint aPoint2) { return sqrt(pow(this->mX - aPoint2.GetX(), 2) + pow(this->mY - aPoint2.GetY(), 2) + pow(this->mZ - aPoint2.GetZ(), 2)); }
	inline cord_t GetX() { return this->mX; }
	inline cord_t GetY() { return this->mY; }
	inline cord_t GetZ() { return this->mZ; }
	inline void SetX(cord_t aX) { this->mX = aX; }
	inline void SetY(cord_t aY) { this->mY = aY; }
	inline void SetZ(cord_t aZ) { this->mZ = aZ; }
	//float getDis(Point p) { return (); }
private:
	cord_t mX, mY, mZ;
};

CPoint::CPoint(cord_t x, cord_t y, cord_t z) : mX(x), mY(y), mZ(z)
{
}

CPoint::~CPoint()
{
}

#endif