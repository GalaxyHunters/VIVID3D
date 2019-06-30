#pragma once

#ifndef INDEXEDFACE_H
#define INDEXEDFACE_H

#include <vector>

using namespace std;

class CIndexedFace
{
public:
	inline CIndexedFace();
	inline CIndexedFace(vector<size_t> aPoints, float aColor);
	inline CIndexedFace(size_t aPoint1, size_t aPoint2, size_t aPoint3, float aColor); // used after triangulation and decimation
	inline ~CIndexedFace();
	inline vector<size_t> GetPoints() { return this->mPoints; }
	inline size_t operator[](size_t I) {return this->mPoints[I];}
	inline float GetColor() { return this->mColor; }
	inline void SetColor(float aColor) { this->mColor = aColor; }
	inline void SetPoints(vector<size_t> aPoints) { this->mPoints = aPoints; }

private:
	vector<size_t> mPoints;
	float mColor;
};

CIndexedFace::CIndexedFace() : mColor(0) {}

CIndexedFace::CIndexedFace(vector<size_t> aPoints, float aColor) : mColor(aColor)
{
	for (vector<size_t>::iterator it = aPoints.begin(); it != aPoints.end(); it++)
	{
		this->mPoints.push_back(*it);
	}
}

CIndexedFace::CIndexedFace(size_t aPoint1, size_t aPoint2, size_t aPoint3, float aColor) {
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
