#pragma once

#ifndef MESH_H
#define MESH_H

#include "Point.h"
#include "IndexedFace.h"
#include <boost/python.hpp>
#include <iostream>
#include "boost/algorithm/string/predicate.hpp"
#include "../../misc/int2str.hpp"
#include "string"
#include "fstream"

using namespace std;
using namespace boost::python;

class Mesh {

private:
	float alpha;
	string label;
	vector<Point> points;
	vector<IndexedFace> faces;

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
	Mesh Mesh_new() { return Mesh(); }
	Mesh Mesh_new(vector<Point> points, vector<IndexedFace> faces, string label, float alpha) { return Mesh(points, faces, label, alpha); }
	void Mesh_simplify(Mesh mesh, float verticlePercent, float error) { mesh.simplify(verticlePercent, error); }
	void Mesh_export(Mesh mesh, string output) { mesh << output; }
	string Mesh_getLabel(Mesh mesh) { return mesh.getLabel(); }
	float Mesh_getAlpha(Mesh mesh) { return mesh.getAlpha(); }
	void Mesh_setLabel(Mesh mesh, string label) { mesh.setLabel(label); }
	void Mesh_setAlpha(Mesh mesh, float alpha) { mesh.setAlpha(alpha); }


}

#endif



//BOOST_PYTHON_MODULE(mesh)
//{
//	class_<Mesh>("Mesh")
//		.def("simplify", &Mesh::simplify)
//		.def("<<", &Mesh::operator<<)
//		.def("getLabel", &Mesh::getLabel)
//		.def("getAlpha", &Mesh::getAlpha)
//		.def("getPoints", &Mesh::getPoints)
//		.def("getFaces", &Mesh::getFaces)
//		.def("setLabel", &Mesh::setLabel)
//		.def("setAlpha", &Mesh::setAlpha)
//		;
//}
//protected:
//	vector<Face> vecFaces;
//	vector<Point> vecPoints;
//	string label;
//	float alpha;
//public:
//	Mesh();
//	Mesh(vector<Face> vecFaces, vector<Point> vecPoints, string label, float alpha);
//	void exportObj(string output);
//	Mesh reduce();
//	//Mesh merge(Mesh mesh); for now this function is cancelld
//	void removePoints(); //clears all the points who arent used from the vecPoints


