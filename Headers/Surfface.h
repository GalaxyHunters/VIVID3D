#pragma once
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
	SurfFace(vector<shared_ptr<Point>> points, float color);
	~SurfFace();

private:

};

SurfFace::SurfFace(vector<shared_ptr<Point>> points, float color) {
	this->points = points;
	this->color = color;
}

SurfFace::~SurfFace()
{
}
