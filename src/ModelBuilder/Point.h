#ifndef VIVID_POINT_H
#define VIVID_POINT_H

#include <cmath>
#include <iostream>
#include <vector>
#include <string>

typedef double quan_t; //TODO: float64_t and rename!

constexpr quan_t POINT_SIMILARITY_THRESHOLD = 0.0001;

// TODO const and & and inl file

namespace vivid
{

class CPoint
{
private:
    quan_t mX=0, mY=0, mZ=0;

public:
	inline CPoint(){}
	inline CPoint(const CPoint &aV) : mX(aV.mX), mY(aV.mY), mZ(aV.mZ) {}
	inline CPoint(quan_t x, quan_t y, quan_t z): mX(x), mY(y), mZ(z) {}
//	inline CPoint(std::vector<quan_t>  aV) : mX(aV[0]), mY(aV[1]), mZ(aV[2]) {}
    inline CPoint(std::vector<double> aV) : mX(quan_t(aV[0])), mY(quan_t(aV[1])), mZ(quan_t(aV[2])) {}
	inline ~CPoint(){}

	inline const quan_t X() const{ return mX; }
	inline const quan_t Y() const{ return mY; }
	inline const quan_t Z() const{ return mZ; }
	inline void X(quan_t aX) { mX = aX; }
	inline void Y(quan_t aY) { mY = aY; }
	inline void Z(quan_t aZ) { mZ = aZ; }

	inline CPoint& operator+= (const CPoint& apV) { mX += apV.mX; mY += apV.mY; mZ += apV.mZ; return *this; }
    inline CPoint& operator-= (const CPoint& apV) { mX -= apV.mX; mY -= apV.mY; mZ -= apV.mZ; return *this; }
    inline CPoint& operator=  (const CPoint& apV) { mX =  apV.mX; mY =  apV.mY; mZ =  apV.mZ; return *this; }
    inline CPoint  operator+  (const CPoint& apV) const{ return CPoint (mX + apV.mX, mY + apV.mY, mZ + apV.mZ); }
    inline CPoint  operator-  (const CPoint& apV) const{ return CPoint (mX - apV.mX, mY - apV.mY, mZ - apV.mZ); }
    inline CPoint  operator*  (const quan_t aSc) const{ return CPoint (aSc * mX, aSc * mY, aSc * mZ); }
    inline CPoint  operator/  (const quan_t aSc) const{ return CPoint (mX / aSc, mY / aSc, mZ / aSc); }

    inline quan_t Dist(const CPoint& arV) const{ return sqrt(pow(mX - arV.X(), 2) + pow(mY - arV.Y(), 2) + pow(mZ - arV.Z(), 2)); }
    inline CPoint& Scale(const CPoint& apV) { mX *= apV.mX; mY *= apV.mY; mZ *= apV.mZ; return *this; }
    inline quan_t Dot  (const CPoint& arV) const{ return (mX * arV.X() + mY * arV.Y() + mZ * arV.Z()); }
    inline CPoint  Cross(const CPoint& arV) const{
        return CPoint(mY * arV.Z() - mZ * arV.Y(),
                      mZ * arV.X() - mX * arV.Z(),
                      mX * arV.Y() - mY * arV.X()); }
    inline quan_t Magnitude() const { return sqrt(Dot(*this)) ;}
    inline CPoint  Normalize() const { return (*this / Magnitude() ); }
    inline bool    Orthogonal(const CPoint& arV) const{ return (POINT_SIMILARITY_THRESHOLD > (*this).Dot(arV)); }


    friend std::ostream& operator<<(std::ostream &out, const CPoint& p)
    {
        out << '(' << p.mX << ',' << p.mY << ',' << p.mZ << ')';
        return out;
    }

};

}; // namespace vivid
#endif //VIVID_POINT_H