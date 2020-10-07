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
	CModel(vector<CSurf> aSurfs, string aLabel, coord_t aAlpha);
	~CModel();
	void ExportToObj(string aOutput);
	void ExportToObjTexture(string aOutput);
	int ExportToGlb();
	static CMesh load(string inputFile); // TODO add a read obj func
	void AddMesh(CMesh aMesh);
	void AddSurf(CSurf aSurf, string aLabel, coord_t aAlpha);
	vector<CMesh> GetMeshes() {return this->mMeshes;}
private:
	vector<CMesh> mMeshes;

	//output functions
	void WriteObj(ofstream& aOBJFile, ofstream& aMTLFile, CMesh * aMesh, size_t * mtl_counter, size_t aPointsCounter);
	void WriteNewMtl(ofstream& aOBJFile, ofstream& aMTLFile, size_t * mtl_counter, color_t color, coord_t aAlpha);
//	void WriteNewFace(ofstream& aOBJFile, CIndexedFace aFace);
    void WriteNewFace(ofstream &aOBJFile, CIndexedFace aFace, size_t aPointsCounter);

    void WriteObjTexture(ofstream& aOBJFile, ofstream& aMTLFile, CMesh * aMesh, size_t * mtl_counter, string aTextureName, coord_t aTextureSize, size_t aPointsCounter);
    void WriteMtlTexture(ofstream& aOBJFile, ofstream& aMTLFile, size_t * mtl_counter, string aTextureName, coord_t aAlpha);
    void WriteNewFaceTexture(ofstream& aOBJFile, CIndexedFace aFace, size_t aPointsCounter);
};
#endif