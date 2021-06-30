#ifndef VIVID_MODEL_H
#define VIVID_MODEL_H

#include "Mesh.h"
#include "Surface.h"


namespace vivid
{

class CModel
{
private:
    std::vector<CMesh> mMeshes;

public:
	CModel() : mMeshes() {}; // TODO YYY?
	CModel(const CMesh arMesh) {mMeshes = std::vector<CMesh>(); mMeshes.push_back(arMesh);};
	CModel(const std::vector<CMesh> & arMeshes) : mMeshes(arMeshes){};
	CModel(std::vector<CSurface> aSurfs, std::string aLabel, coord_t aAlpha); // TODO YYY?
	~CModel();

	void ExportToObj(std::string aOutput, bool WithTexture = 1);

//	static CMesh load(string inputFile); // TODO add a read obj func
	void AddMesh(CMesh aMesh);
	void AddSurf(CSurface aSurf, std::string aLabel, coord_t aAlpha);
	vector<CMesh> GetMeshes() {return mMeshes;}

};

}; // namespace vivid
#endif //VIVID_MODEL_H