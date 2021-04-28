#pragma once

#ifndef INDEXEDFACE_H
#define INDEXEDFACE_H

#include <vector>
#include "Point.h"

using namespace std;

class CIndexedFace
{
public:
	inline CIndexedFace();
	inline CIndexedFace(vector<size_t> aPoints, coord_t aColor);
	inline CIndexedFace(size_t aPoint1, size_t aPoint2, size_t aPoint3, coord_t aColor); // used after triangulation and decimation
	inline ~CIndexedFace();
	inline vector<size_t> GetPoints() { return mPoints; }
	inline size_t operator[](size_t I) {return mPoints[I];}
	inline coord_t GetColor() { return mColor; }
	inline void SetColor(coord_t aColor) { mColor = aColor; }
	inline void SetPoints(vector<size_t> aPoints) { mPoints = aPoints; }

private:
	vector<size_t> mPoints;
	coord_t mColor;
};

CIndexedFace::CIndexedFace() : mColor(0) {}

CIndexedFace::CIndexedFace(vector<size_t> aPoints, coord_t aColor) : mColor(aColor)
{
	for (vector<size_t>::iterator it = aPoints.begin(); it != aPoints.end(); it++)
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
#endif
