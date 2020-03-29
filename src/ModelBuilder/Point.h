
#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <iostream>
#include <vector>
#include "boost/shared_ptr.hpp"

typedef double coord_t;

class CPoint
{
public:
	inline CPoint(){}
	inline CPoint(const CPoint &aPoint2) : mX(aPoint2.mX), mY(aPoint2.mY), mZ(aPoint2.mZ) {} //copy constructor
	inline CPoint(coord_t x, coord_t y, coord_t z);
	inline CPoint(std::vector<coord_t > cords) : mX(cords[0]), mY(cords[1]), mZ(cords[2]) {}
	inline ~CPoint();
	inline coord_t CalcDistance(CPoint& aPoint2) { return sqrt(pow(this->mX - aPoint2.GetX(), 2) + pow(this->mY - aPoint2.GetY(), 2) + pow(this->mZ - aPoint2.GetZ(), 2)); }
	inline coord_t GetX() { return this->mX; }
	inline coord_t GetY() { return this->mY; }
	inline coord_t GetZ() { return this->mZ; }
	inline void SetX(coord_t aX) { this->mX = aX; }
	inline void SetY(coord_t aY) { this->mY = aY; }
	inline void SetZ(coord_t aZ) { this->mZ = aZ; }
	//float getDis(Point p) { return (); }
private:
	coord_t mX, mY, mZ;
};

CPoint::CPoint(coord_t x, coord_t y, coord_t z) : mX(x), mY(y), mZ(z)
{
}

CPoint::~CPoint()
{
}

#endif