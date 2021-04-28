#pragma once

#ifndef SURFFACE_H
#define SURFFACE_H

#include <vector>
#include <iostream>
#include "Point.h"

using namespace std;

class CSurfFace
{
public:
	vector<shared_ptr<CPoint> > mPoints;
	coord_t mColor;
	pair<size_t, size_t> mPairPoints;

	inline CSurfFace(vector<shared_ptr<CPoint>> pPoints, coord_t pColor, pair<size_t, size_t> pPairPoints) : mPoints(pPoints), mColor(pColor), mPairPoints(pPairPoints){};
	inline CSurfFace() {};
	inline ~CSurfFace() {};

private:

};

#endif