#ifndef MODEL_H
#define MODEL_H

//#include "fstream"
#include "Mesh.h"
#include "Surf.h"
#include "ObjExportImport.h"


namespace vivid
{

class CModel
{
private:
    std::vector<CMesh> mMeshes;

public:
	CModel() : mMeshes() {};
	CModel(const CMesh arMesh) {mMeshes = std::vector<CMesh>(); mMeshes.push_back(arMesh);};
	CModel(const std::vector<CMesh> & arMeshes) : mMeshes(arMeshes){};
	CModel(std::vector<CSurf> aSurfs, std::string aLabel, coord_t aAlpha);
	~CModel();

	void ExportToObj(std::string aOutput, bool WithTexture = 1);

//	static CMesh load(string inputFile); // TODO add a read obj func
	void AddMesh(CMesh aMesh);
	void AddSurf(CSurf aSurf, std::string aLabel, coord_t aAlpha);
	vector<CMesh> GetMeshes() {return mMeshes;}

};

}; // namespace vivid

#endif