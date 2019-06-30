
#ifndef POINT_H
#define POINT_H

#include <cmath>

class CPoint
{
public:
	inline CPoint(){}
	inline CPoint(const CPoint &aPoint2) : mX(aPoint2.mX), mY(aPoint2.mY), mZ(aPoint2.mZ) {} //copy constructor
	inline CPoint(float x, float y, float z);
	inline ~CPoint();
	inline double CalcDistance(CPoint aPoint2) { return sqrt(pow(this->mX - aPoint2.GetX(), 2) + pow(this->mY - aPoint2.GetY(), 2) + pow(this->mZ - aPoint2.GetZ(), 2)); }
	inline float GetX() { return this->mX; }
	inline float GetY() { return this->mY; }
	inline float GetZ() { return this->mZ; }
	inline void SetX(float aX) { this->mX = aX; }
	inline void SetY(float aY) { this->mY = aY; }
	inline void SetZ(float aZ) { this->mZ = aZ; }
	//float getDis(Point p) { return (); }
private:
	float mX, mY, mZ;
};

CPoint::CPoint(float x, float y, float z) : mX(x), mY(y), mZ(z)
{
}

CPoint::~CPoint()
{
}

#endif