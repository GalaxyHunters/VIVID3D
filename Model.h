#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

using namespace std;

class CModel
{
public:
	CModel();
	CModel(vector<CMesh> aMeshes);
	~CModel();
	void operator<<(string aOutputFile);
	static CMesh load(string inputFile); // TODO add a read obj func
	void AddMesh(CMesh mesh);
private:
	vector<CMesh> mMeshes;

	//output functions
	void WriteObj(ofstream& aOBJFile, ofstream& aMTLFile, CMesh * aMesh, size_t * mtl_counter, size_t aPointsCounter);
	void WriteNewMtl(ofstream& aOBJFile, ofstream& aMTLFile, size_t * mtl_counter, Color_t color, cord_t aAlpha);
	void WriteNewFace(ofstream& aOBJFile, CIndexedFace aFace);
};

extern "C"
{
	inline CModel Model_new(vector<CMesh> meshes) { return CModel(meshes); }
	inline void Model_export(CModel model, string output) { model << output; }
	inline CMesh Model_load(CModel model, string inputFile) { return model.load(inputFile); }
	inline void Model_addMesh(CModel model, CMesh mesh) { model.AddMesh(mesh); }
}

#endif