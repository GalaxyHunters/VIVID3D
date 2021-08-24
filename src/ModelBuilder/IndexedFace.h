#ifndef VIVID_INDEXEDFACE_H
#define VIVID_INDEXEDFACE_H

#include "Point.h"

namespace vivid
{

class CIndexedFace // TODO rename why not Face?
{
public:
	inline CIndexedFace(std::vector<size_t> aPoints, coord_t aColor) : mPoints(aPoints), mColor(aColor){}
	inline CIndexedFace(size_t aPoint1, size_t aPoint2, size_t aPoint3, coord_t aColor) : mPoints({aPoint1, aPoint2, aPoint3}), mColor(aColor) {} // used after triangulation and decimation //TODO should we need it?
	//operator=
	inline ~CIndexedFace(){}
	inline const std::vector<size_t> GetPoints() { return mPoints; }
	inline size_t operator[](size_t I) {return mPoints[I];}
	inline const coord_t GetColor() const { return mColor; }
	inline void SetColor(const coord_t aColor) { mColor = aColor; }

    inline void SetPoints(const std::vector<size_t> &arPoints) { mPoints = arPoints; }

private:
    std::vector<size_t> mPoints = {};
	// TODO: VERY BAD SHOULD BE mQuan BECAUSE OTHERWISE MULTIPLE FUNCTIONS SUCH AS GetColor() etc. BAD BAD BAD !!!
	coord_t mColor = 0; // TODO maybe magic num that should be defined in color... //TODO coord_t mColor shouldn't be together
};

} // namespace vivid
#endif //VIVID_INDEXEDFACE_H
