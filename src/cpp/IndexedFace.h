#ifndef INDEXEDFACE_H
#define INDEXEDFACE_H

#include <vector>

using namespace std;

class IndexedFace
{
public:
	inline IndexedFace();
	inline IndexedFace(vector<size_t> points, float color);
	inline ~IndexedFace();
	inline vector<size_t> getPoints() { return this->_points; }
	inline float getColor() { return this->_color; }
	inline void setColor(float color) { this->_color = color; }
	inline void setPoints(vector<size_t> points) { this->_points = points; }

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
#endif
