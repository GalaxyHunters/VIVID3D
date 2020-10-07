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

    vector<CIndexedFace> GetFacesAsTriangles();

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


    /**
     * Rotate the Mesh around a normal vector by an angel, counterclockwise
     * @param[in] aNormalVec the x,y.z normal to rotate around.
     * @param[in] aRadianAngel the angel to rotate by (in radians).
     */
    void rotatewMesh(CPoint aNormalVec, double aRadianAngel);
    /**
     * Change Mesh location by addition of x,y,z.
     * @param[in] aDirectionVec the x,y.z direction to move by it.
     */
    void moveMesh(CPoint aDirectionVec);
    /**
     * Change Mesh scale in x,y,z direction by x,y,z of the scale vector
     * @param[in] aScaleVec the x,y.z direction to move by it.
     */
    void scaleMesh(CPoint aScaleVec);

};


#endif

