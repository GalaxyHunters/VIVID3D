#ifndef MESH_H
#define MESH_H

#include "Point.h"
#include "IndexedFace.h"
#include <iostream>
#include "boost/algorithm/string/predicate.hpp"
#include "../../misc/int2str.hpp"
#include "string"
#include "fstream"

using namespace std;


class Mesh {

private:
	vector<Point> points;
	vector<IndexedFace> faces;
        string label;
	float alpha;

public:
	Mesh();
	Mesh(vector<Point> points, vector<IndexedFace> faces, string label, float alpha);
	~Mesh();
	void simplify(float verticlePercent, float error);
	void operator<<(string output);
	string getLabel();
	float getAlpha();
	vector<Point> getPoints();
	vector<IndexedFace> getFaces();
	void setLabel(string label);
	void setAlpha(float alpha);
};

extern "C"
{
//	inline Mesh Mesh_new() { return Mesh(); }
	inline Mesh Mesh_new(vector<Point> points, vector<IndexedFace> faces, string label, float alpha) { return Mesh(points, faces, label, alpha); }
	inline void Mesh_simplify(Mesh mesh, float verticlePercent, float error) { mesh.simplify(verticlePercent, error); }
	inline void Mesh_export(Mesh mesh, string output) { mesh << output; }
	inline string Mesh_getLabel(Mesh mesh) { return mesh.getLabel(); }
	inline float Mesh_getAlpha(Mesh mesh) { return mesh.getAlpha(); }
	inline void Mesh_setLabel(Mesh mesh, string label) { mesh.setLabel(label); }
	inline void Mesh_setAlpha(Mesh mesh, float alpha) { mesh.setAlpha(alpha); }


}

#endif




