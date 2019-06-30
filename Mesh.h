#ifndef MESH_H
#define MESH_H

#include "Point.h"
#include "IndexedFace.h"
#include "Utils.h"
#include "ColorMap.h"

#include <iostream>
#include "boost/algorithm/string/predicate.hpp"
#include "../../misc/int2str.hpp"
#include "string"
#include "fstream"

using namespace std;

class CMesh {

private:
	float mAlpha;
	string mLabel;
	vector<CPoint> mPoints;
	vector<CIndexedFace> mFaces;

	//output functions
	void WriteObj(ofstream& aOBJFile, ofstream& aMTLFile, size_t * mtl_counter);
	void WriteNewMtl(ofstream& aOBJFile, ofstream& aMTLFile, size_t * mtl_counter, Color_t color, float aAlpha);
	void WriteNewFace(ofstream& aOBJFile, CIndexedFace aFace);

	void Triangulation();

public:
	CMesh() {};
	CMesh(vector<CPoint> aPoints, vector<CIndexedFace> aFaces, string aLabel, float aAlpha);
	~CMesh();
	void Decimation(float aVerticlePercent, float aError);
	void operator<<(string aOutputFile);
	string GetLabel();
	float GetAlpha();
	vector<CPoint> GetPoints();
	vector<CIndexedFace> GetFaces();
	void SetLabel(string aLabel);
	void SetAlpha(float aAlpha);
};

extern "C"
{
	//Mesh Mesh_new() { return Mesh(); }
	inline CMesh Mesh_new(vector<CPoint> points, vector<CIndexedFace> faces, string label, float alpha) { return CMesh(points, faces, label, alpha); }
	inline void Mesh_decimation(CMesh mesh, float verticlePercent, float error) { mesh.Decimation(verticlePercent, error); }
	inline void Mesh_export(CMesh mesh, string output) { mesh << output; }
	inline string Mesh_getLabel(CMesh mesh) { return mesh.GetLabel(); }
	inline float Mesh_getAlpha(CMesh mesh) { return mesh.GetAlpha(); }
	inline void Mesh_setLabel(CMesh mesh, string label) { mesh.SetLabel(label); }
	inline void Mesh_setAlpha(CMesh mesh, float alpha) { mesh.SetAlpha(alpha); }
}

#endif


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


