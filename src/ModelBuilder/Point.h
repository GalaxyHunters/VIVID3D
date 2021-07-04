#ifndef VIVID_POINT_H
#define VIVID_POINT_H

#include <cmath>
#include <iostream>
#include <vector>
#include <string>

typedef double coord_t; //TODO: float64_t and rename!

// TODO const and & and inl file

namespace vivid
{

class CPoint
{
private:
    coord_t mX=0, mY=0, mZ=0;

public:
	inline CPoint(){}
	inline CPoint(const CPoint &aV) : mX(aV.mX), mY(aV.mY), mZ(aV.mZ) {} //copy constructor
	inline CPoint(coord_t x, coord_t y, coord_t z): mX(x), mY(y), mZ(z) {}
	inline CPoint(std::vector<coord_t>  aV) : mX(aV[0]), mY(aV[1]), mZ(aV[2]) {}
	inline ~CPoint(){}
	inline coord_t Dist(const CPoint& arV) const{ return sqrt(pow(mX - arV.X(), 2) + pow(mY - arV.Y(), 2) + pow(mZ - arV.Z(), 2)); }
	inline const coord_t X() const{ return mX; }
	inline const coord_t Y() const{ return mY; }
	inline const coord_t Z() const{ return mZ; }
	inline void X(coord_t aX) { mX = aX; }
	inline void Y(coord_t aY) { mY = aY; }
	inline void Z(coord_t aZ) { mZ = aZ; }
	inline CPoint& operator+= (const CPoint& apV) { mX += apV.mX; mY += apV.mY; mZ += apV.mZ; return *this; }
    inline CPoint& operator-= (const CPoint& apV) { mX -= apV.mX; mY -= apV.mY; mZ -= apV.mZ; return *this; }
    inline CPoint& operator=  (const CPoint& apV) { mX =  apV.mX; mY =  apV.mY; mZ =  apV.mZ; return *this; }
    inline CPoint  operator+  (const CPoint& apV) const{ return CPoint (mX + apV.mX, mY + apV.mY, mZ + apV.mZ); }
    inline CPoint  operator-  (const CPoint& apV) const{ return CPoint (mX - apV.mX, mY - apV.mY, mZ - apV.mZ); }
    inline CPoint  operator*  (const coord_t aSc) const{ return CPoint (aSc * mX, aSc * mY, aSc * mZ); }
    inline CPoint  operator/  (const coord_t aSc) const{ return CPoint (mX / aSc, mY / aSc, mZ / aSc); }

    inline CPoint& Scale(const CPoint& apV) { mX *= apV.mX; mY *= apV.mY; mZ *= apV.mZ; return *this; }
    inline coord_t Dot  (const CPoint& arV) const{ return (mX * arV.X() + mY * arV.Y() + mZ * arV.Z()); }
    inline CPoint  Cross(const CPoint& arV) const{
        return CPoint(mY * arV.Z() - mZ * arV.Y(),
                      mZ * arV.X() - mX * arV.Z(),
                      mX * arV.Y() - mY * arV.X()); }
    inline coord_t Magnitude() const { return sqrt(Dot(*this)) ;}
    inline CPoint  Normalize() const { return (*this / Magnitude() ); }
    inline bool    Orthogonal(const CPoint& arV1, const CPoint& arV2) const{ return (0 == arV1.Dot(arV2)); }

    friend std::ostream& operator<<(std::ostream &out, const CPoint& p)
    {
        out << '(' << p.mX << ',' << p.mY << ',' << p.mZ << ')';
        return out;
    }


};

}; // namespace vivid
#endif //VIVID_POINT_H