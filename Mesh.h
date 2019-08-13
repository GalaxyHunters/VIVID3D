#ifndef MESH_H
#define MESH_H

#include "Point.h"
#include "IndexedFace.h"
#include "External.h"
#include "ColorMap.h"

#include <iostream>
#include "boost/algorithm/string/predicate.hpp"
#include "../../misc/int2str.hpp"
#include "string"
#include "fstream"

using namespace std;

class CMesh {

private:
	cord_t mAlpha;
	string mLabel;
	vector<CPoint> mPoints;
	vector<CIndexedFace> mFaces;

	//output functions
	void WriteObj(ofstream& aOBJFile, ofstream& aMTLFile, size_t * mtl_counter);
	void WriteNewMtl(ofstream& aOBJFile, ofstream& aMTLFile, size_t * mtl_counter, Color_t color, cord_t aAlpha);
	void WriteNewFace(ofstream& aOBJFile, CIndexedFace aFace);

	void Triangulation();

public:
	CMesh() {};
	CMesh(vector<CPoint> aPoints, vector<CIndexedFace> aFaces, string aLabel, cord_t aAlpha);
	~CMesh();
	void Decimation(cord_t aVerticlePercent, cord_t aError);
	void operator<<(string aOutputFile);
	string GetLabel();
	cord_t GetAlpha();
	vector<CPoint> GetPoints();
	vector<CIndexedFace> GetFaces();
	void SetFaces(vector<CIndexedFace> aFaces);
	void SetPoints(vector<CPoint> aPoints);
	void SetLabel(string aLabel);
	void SetAlpha(cord_t aAlpha);
};

extern "C"
{
	//Mesh Mesh_new() { return Mesh(); }
	inline CMesh Mesh_new(vector<CPoint> points, vector<CIndexedFace> faces, string label, cord_t alpha) { return CMesh(points, faces, label, alpha); }
	inline void Mesh_decimation(CMesh mesh, cord_t verticlePercent, cord_t error) { mesh.Decimation(verticlePercent, error); }
	inline void Mesh_export(CMesh mesh, string output) { mesh << output; }
	inline string Mesh_getLabel(CMesh mesh) { return mesh.GetLabel(); }
	inline cord_t Mesh_getAlpha(CMesh mesh) { return mesh.GetAlpha(); }
	inline void Mesh_setLabel(CMesh mesh, string label) { mesh.SetLabel(label); }
	inline void Mesh_setAlpha(CMesh mesh, cord_t alpha) { mesh.SetAlpha(alpha); }
}

#endif


//protected:
//	vector<Face> vecFaces;
//	vector<Point> vecPoints;
//	string label;
//	cord_t alpha;
//public:
//	Mesh();
//	Mesh(vector<Face> vecFaces, vector<Point> vecPoints, string label, cord_t alpha);
//	void exportObj(string output);
//	Mesh reduce();
//	//Mesh merge(Mesh mesh); for now this function is cancelld
//	void removePoints(); //clears all the points who arent used from the vecPoints


