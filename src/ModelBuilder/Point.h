#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <iostream>
#include <vector>
#include <string>

typedef double coord_t; //TODO: float64_t and rename!
using namespace std;

//TODO TODO TODO struct MathVector{double size;
//    vector<double> direction;};


// TODO CPoint should be typedef of vector calculation object by size 3 of maximum float.
// TODO const and & and inl file
class CPoint
{
private:
    coord_t mX, mY, mZ;

public:
	inline CPoint(){}
	inline CPoint(const CPoint &aV) : mX(aV.mX), mY(aV.mY), mZ(aV.mZ) {} //copy constructor
	inline CPoint(coord_t x, coord_t y, coord_t z): mX(x), mY(y), mZ(z) {}
	inline CPoint(std::vector<coord_t>  aV) : mX(aV[0]), mY(aV[1]), mZ(aV[2]) {}
	inline ~CPoint(){}
	inline coord_t Dist(CPoint& aVec) { return sqrt(pow(mX - aVec.X(), 2) + pow(mY - aVec.Y(), 2) + pow(mZ - aVec.Z(), 2)); }
	inline coord_t X() { return mX; }
	inline coord_t Y() { return mY; }
	inline coord_t Z() { return mZ; }
	inline void X(coord_t aX) { mX = aX; }
	inline void Y(coord_t aY) { mY = aY; }
	inline void Z(coord_t aZ) { mZ = aZ; }
	inline CPoint& operator += (const CPoint& apV) { mX += apV.mX; mY += apV.mY; mZ += apV.mZ; return *this; }
    inline CPoint& operator -= (const CPoint& apV) { mX -= apV.mX; mY -= apV.mY; mZ -= apV.mZ; return *this; }
    inline CPoint& operator =  (const CPoint& apV) { mX =  apV.mX; mY =  apV.mY; mZ =  apV.mZ; return *this; }
    inline CPoint  operator +  (const CPoint& apV) { return CPoint (mX + apV.mX, mY + apV.mY, mZ + apV.mZ); }
    inline CPoint  operator -  (const CPoint& apV) { return CPoint (mX - apV.mX, mY - apV.mY, mZ - apV.mZ); }
    inline CPoint  operator *  (const coord_t aSc) { return CPoint (aSc * mX, aSc * mY, aSc * mZ); }
//    inline CPoint  operator *  (const coord_t aSc, const CPoint& apV) { return CPoint (aSc * apV.X(), aSc * apV.Y(), aSc * apV.Z()); }
    inline CPoint  operator /  (const coord_t aSc) {
        if (0 == aSc)
        {
            return CPoint(0,0,0);
        }
	    return CPoint (mX / aSc, mY / aSc, mZ / aSc);
	}


    inline coord_t Dot  (CPoint aVec){ return (mX * aVec.X() + mY * aVec.Y() + mZ * aVec.Z()); }
    inline CPoint  Cross(CPoint aVec){
        return CPoint(mY * aVec.Z() - mZ * aVec.Y(),
                      mZ * aVec.X() - mX * aVec.Z(),
                      mX * aVec.Y() - mY * aVec.X()); }

    inline coord_t Norm(){ return sqrt(Dot(*this)) ;}
    inline CPoint  Normalize()
    {
        return (*this / this->Norm() );
    }
    inline bool Orthogonal(CPoint aV1, CPoint aV2)
    {
        if (0 == aV1.Dot(aV2)) {
            return true;
        }
        return false;
    }


    // TODO NAFTALI: This is VERY_BAD_PRACTICE as there can be other formats instead of obj, hence printing to OBJ format should be done in a different place.
    friend inline ostream& operator << (ostream& os, const CPoint& point) {os << "v " << point.mX  << " " << point.mY << " " << point.mZ << "\n" ; return os;}


};

//inline ostream& operator<<(ostream &out, const CPoint &p)
//{
//    out << '(' << p.mX << ',' << p.mY << ',' << p.mZ << ')';
//    return out;
//}

#endif