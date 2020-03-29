#ifndef MESH_H
#define MESH_H

#include "Point.h"
#include "IndexedFace.h"
#include "../Libs/External.h"
#include "Utils/ColorMap.h"

#include <iostream>
#include "boost/algorithm/string/predicate.hpp"
#include "int2str.hpp"
#include "string"
#include "fstream"

using namespace std;

class CMesh {

private:
	coord_t mAlpha;
	string mLabel;
	vector<CPoint> mPoints;
	vector<CIndexedFace> mFaces;

	//output functions
	void WriteObj(ofstream& aOBJFile, ofstream& aMTLFile, size_t * mtl_counter);
	void WriteNewMtl(ofstream& aOBJFile, ofstream& aMTLFile, size_t * mtl_counter, color_t color, coord_t aAlpha);
	void WriteNewFace(ofstream& aOBJFile, CIndexedFace aFace);

    void WriteObjTexture(ofstream& aOBJFile, ofstream& aMTLFile, string aTextureName, coord_t aTextureSize);
    void WriteMtlTexture(ofstream& aOBJFile, ofstream& aMTLFile, string aTextureName, coord_t aAlpha);
    void WriteNewFaceTexture(ofstream& aOBJFile, CIndexedFace aFace);

	void Triangulation();

public:
	CMesh() {};
    CMesh(const CMesh &mesh);
	CMesh(vector<CPoint> aPoints, vector<CIndexedFace> aFaces, string aLabel, coord_t aAlpha);
	~CMesh();
	void Decimation(coord_t aVerticlePercent, coord_t aError);
	void ExportToObj(string aOutput);
	void ExportToObjTexture(string aOutput);
	string GetLabel();
	coord_t GetAlpha();
	vector<CPoint> GetPoints();
	vector<CIndexedFace> GetFaces();
	void SetFaces(vector<CIndexedFace> aFaces);
	void SetPoints(vector<CPoint> aPoints);
	void SetLabel(string aLabel);
	void SetAlpha(coord_t aAlpha);
};


#endif

