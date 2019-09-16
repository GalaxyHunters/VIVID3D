#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "Surf.h"
using namespace std;

class CModel
{
public:
	CModel();
	CModel(vector<CMesh> aMeshes);
	CModel(vector<CSurf> aSurfs, string aLabel, cord_t aAlpha);
	~CModel();
	void ExportToObj(string aOutput);
	static CMesh load(string inputFile); // TODO add a read obj func
	void AddMesh(CMesh aMesh);
	void AddSurf(CSurf aSurf, string aLabel, cord_t aAlpha);
private:
	vector<CMesh> mMeshes;

	//output functions
	void WriteObj(ofstream& aOBJFile, ofstream& aMTLFile, CMesh * aMesh, size_t * mtl_counter, size_t aPointsCounter);
	void WriteNewMtl(ofstream& aOBJFile, ofstream& aMTLFile, size_t * mtl_counter, Color_t color, cord_t aAlpha);
	void WriteNewFace(ofstream& aOBJFile, CIndexedFace aFace);
};
#endif