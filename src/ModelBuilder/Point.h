#ifndef VIVID_POINT_H
#define VIVID_POINT_H

#include <cmath>
#include <iostream>
#include <vector>
#include <string>

typedef double coord_t;

constexpr coord_t POINT_SIMILARITY_THRESHOLD = 0.0001;

namespace vivid
{
    class CPoint
    {
    private:
        coord_t mX=0, mY=0, mZ=0;

    public:
        inline CPoint(){}
        inline CPoint(const CPoint &arV) : mX(arV.mX), mY(arV.mY), mZ(arV.mZ) {}
        inline CPoint(coord_t x, coord_t y, coord_t z): mX(x), mY(y), mZ(z) {}
        inline CPoint(std::vector<coord_t>  aV) : mX(aV[0]), mY(aV[1]), mZ(aV[2]) {}

        inline ~CPoint(){}

        inline coord_t X() const{ return mX; }
        inline coord_t Y() const{ return mY; }
        inline coord_t Z() const{ return mZ; }
        inline void X(coord_t aX) { mX = aX; }
        inline void Y(coord_t aY) { mY = aY; }
        inline void Z(coord_t aZ) { mZ = aZ; }

        inline CPoint& operator+= (const CPoint& arV) { mX += arV.mX; mY += arV.mY; mZ += arV.mZ; return *this; }
        inline CPoint& operator-= (const CPoint& arV) { mX -= arV.mX; mY -= arV.mY; mZ -= arV.mZ; return *this; }
        inline CPoint& operator=  (const CPoint& arV) { mX =  arV.mX; mY =  arV.mY; mZ =  arV.mZ; return *this; }
        inline CPoint  operator+  (const CPoint& arV) const{ return {mX + arV.mX, mY + arV.mY, mZ + arV.mZ}; }
        inline CPoint  operator-  (const CPoint& arV) const{ return {mX - arV.mX, mY - arV.mY, mZ - arV.mZ}; }
        inline CPoint  operator*  (const coord_t aSc) const{ return {aSc * mX, aSc * mY, aSc * mZ}; }
        inline CPoint  operator/  (const coord_t aSc) const{ return {mX / aSc, mY / aSc, mZ / aSc}; }
        inline bool operator== (const CPoint& arV) const {return mX == arV.mX && mY == arV.mY && mZ == arV.mZ;}
        inline bool operator!= (const CPoint& arV) const {return mX != arV.mX || mY != arV.mY || mZ != arV.mZ;}
        inline bool operator < (const CPoint& arV) const {
            if (std::abs(mX - arV.mX) <= POINT_SIMILARITY_THRESHOLD) {
                if (std::abs(mY - arV.mY) <= POINT_SIMILARITY_THRESHOLD) {
                    if (std::abs(mZ - arV.mZ) <= POINT_SIMILARITY_THRESHOLD) {return false;}
                        // we compare the points by z to see who needs to go first
                    else{return mZ > arV.mZ;}}
                    // we compare by y to see who needs to go first
                else{return mY > arV.mY;}}
                // we compare by x to see who goes first
            else{return mX > arV.mX;}
        }

        inline coord_t Dist(const CPoint& arV) const{ return sqrt(pow(mX - arV.X(), 2) + pow(mY - arV.Y(), 2) + pow(mZ - arV.Z(), 2)); }
        inline CPoint& Scale(const CPoint& arV) { mX *= arV.mX; mY *= arV.mY; mZ *= arV.mZ; return *this; }
        inline coord_t Dot  (const CPoint& arV) const{ return (mX * arV.X() + mY * arV.Y() + mZ * arV.Z()); }
        inline CPoint  Cross(const CPoint& arV) const{
            return {mY * arV.Z() - mZ * arV.Y(),
                    mZ * arV.X() - mX * arV.Z(),
                    mX * arV.Y() - mY * arV.X()}; }
        inline coord_t Magnitude() const { return sqrt(Dot(*this)) ;}
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