
#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <iostream>
#include <vector>
#include <string>

typedef double coord_t; //TODO: float64_t and rename!
using namespace std;

// TODO CPoint should be typedef of vector calculation object by size 3 of maximum float.
class CPoint
{
public:
	inline CPoint(){}
	inline CPoint(const CPoint &aPoint2) : mX(aPoint2.mX), mY(aPoint2.mY), mZ(aPoint2.mZ) {} //copy constructor
	inline CPoint(coord_t x, coord_t y, coord_t z): mX(x), mY(y), mZ(z) {}
	inline CPoint(std::vector<coord_t > cords) : mX(cords[0]), mY(cords[1]), mZ(cords[2]) {}
	inline ~CPoint(){}
	inline coord_t CalcDistance(CPoint& aPoint2) { return sqrt(pow(mX - aPoint2.GetX(), 2) + pow(mY - aPoint2.GetY(), 2) + pow(mZ - aPoint2.GetZ(), 2)); }
	inline coord_t GetX() { return mX; }
	inline coord_t GetY() { return mY; }
	inline coord_t GetZ() { return mZ; }
	inline void SetX(coord_t aX) { mX = aX; }
	inline void SetY(coord_t aY) { mY = aY; }
	inline void SetZ(coord_t aZ) { mZ = aZ; }
	inline CPoint& operator += (const CPoint& apPoint2) {mX += apPoint2.mX; mY += apPoint2.mY; mZ += apPoint2.mZ; return *this;}
    inline CPoint& operator -= (const CPoint& apPoint2) {mX -= apPoint2.mX; mY -= apPoint2.mY; mZ -= apPoint2.mZ; return *this;}
    inline CPoint& operator = (const CPoint& apPoint2) {mX = apPoint2.mX; mY = apPoint2.mY; mZ = apPoint2.mZ; return *this;}
    inline CPoint  operator + (const CPoint& apPoint2) {return CPoint (mX + apPoint2.mX, mY + apPoint2.mY, mZ + apPoint2.mZ); }













    // TODO NAFTALI: This is VERY_BAD_PRACTICE as there can be other formats instead of obj, hence printing to OBJ format should be done in a different place.
    friend inline ostream& operator << (ostream& os, const CPoint& point) {os << "v " << point.mX  << " " << point.mY << " " << point.mZ << "\n" ; return os;}

private:
	coord_t mX, mY, mZ;
};

//inline ostream& operator<<(ostream &out, const CPoint &p)
//{
//    out << '(' << p.mX << ',' << p.mY << ',' << p.mZ << ')';
//    return out;
//}

#endif