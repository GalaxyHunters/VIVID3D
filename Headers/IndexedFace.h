#pragma once
#include <vector>

using namespace std;

class IndexedFace
{
public:
	IndexedFace();
	IndexedFace(vector<size_t> points, float color);
	~IndexedFace();
	vector<size_t> getPoints() { return this->_points; }
	float getColor() { return this->_color; }
	void setColor(float color) { this->_color = color; }
	void setPoints(vector<size_t> points) { this->_points = points; }

private:
	vector<size_t> _points;
	float _color;
};

IndexedFace::IndexedFace() : _color(0) {}

IndexedFace::IndexedFace(vector<size_t> points, float color) : _color(color)
{
	for (vector<size_t>::iterator it = points.begin(); it != points.end(); it++)
	{
		this->_points.push_back(*it);
	}
}

IndexedFace::~IndexedFace()
{
}
