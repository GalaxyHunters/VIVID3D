#ifndef SURFFACE_H
#define SURFFACE_H

#include <memory>
#include <vector>
#include <iostream>
#include "Point.h"
using namespace std;

class SurfFace
{
public:
	vector<shared_ptr<Point>> points;
	float color;
	pair<size_t, size_t> C_points;
	inline SurfFace(vector<shared_ptr<Point>> points, float color, pair<size_t, size_t> C_points);
	inline ~SurfFace();

private:

};

SurfFace::SurfFace(vector<shared_ptr<Point>> points, float color, pair<size_t, size_t> C_points) {
	this->points = points;
	this->color = color;
	this->C_points = C_points;
}

SurfFace::~SurfFace()
{
}

#endif
