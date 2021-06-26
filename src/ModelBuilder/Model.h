#ifndef MODEL_H
#define MODEL_H

#include "fstream"
#include "Mesh.h"
#include "Surf.h"

class CModel;

#include "ObjExportImport.h"

using namespace std;

class CModel
{
private:
    vector<CMesh> mMeshes;

public:
	CModel() : mMeshes() {};
	inline CModel(const CMesh arMesh) {this->mMeshes = vector<CMesh>(); mMeshes.push_back(arMesh);};
	CModel(const vector<CMesh> & arMeshes) : mMeshes(arMeshes){};
	CModel(vector<CSurf> aSurfs, string aLabel, coord_t aAlpha);
	~CModel();
	void ExportToObj(string aOutput, bool WithTexture = 1);
//	static CMesh load(string inputFile); // TODO add a read obj func
	void AddMesh(CMesh aMesh);
	void AddSurf(CSurf aSurf, string aLabel, coord_t aAlpha);
	vector<CMesh> GetMeshes() {return mMeshes;}

};
#endif