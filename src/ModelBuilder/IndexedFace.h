#ifndef VIVID_INDEXEDFACE_H
#define VIVID_INDEXEDFACE_H

#include "Point.h"

namespace vivid
{

class CIndexedFace
{
public:
	inline CIndexedFace();
	inline CIndexedFace(std::vector<size_t> aPoints, coord_t aColor);
	inline CIndexedFace(size_t aPoint1, size_t aPoint2, size_t aPoint3, coord_t aColor); // used after triangulation and decimation
	//operator=
	inline ~CIndexedFace();
	inline const std::vector<size_t> GetPoints() { return mPoints; }
	inline size_t operator[](size_t I) {return mPoints[I];}
	inline const coord_t GetColor() const { return mColor; }
	inline void SetColor(const coord_t aColor) { mColor = aColor; }
	inline void SetPoints(const std::vector<size_t> &arPoints) { mPoints = arPoints; }

private:
    std::vector<size_t> mPoints;
	// TODO: VERY BAD SHOULD BE mQuan BECAUSE OTHERWISE MULTIPLE FUNCTIONS SUCH AS GetColor() etc. BAD BAD BAD !!!
	coord_t mColor;
};

CIndexedFace::CIndexedFace() : mColor(0) {} // TODO CPP file


CIndexedFace::CIndexedFace(std::vector<size_t> aPoints, coord_t aColor) : mColor(aColor)
{
	for (auto it = aPoints.begin(); it != aPoints.end(); it++) // TODO CPP file
	{
		mPoints.push_back(*it);
	}
}

CIndexedFace::CIndexedFace(size_t aPoint1, size_t aPoint2, size_t aPoint3, coord_t aColor) {
	mPoints.clear();
	mPoints.push_back(aPoint1);
	mPoints.push_back(aPoint2);
	mPoints.push_back(aPoint3);
	mColor = aColor;
}
CIndexedFace::~CIndexedFace()
{
}

} // namespace vivid
#endif //VIVID_INDEXEDFACE_H
