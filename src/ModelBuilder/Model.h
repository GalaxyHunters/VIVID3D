#ifndef VIVID_MODEL_H
#define VIVID_MODEL_H

#include "Mesh.h"
#include "Surface.h"


namespace vivid
{

class CModel
{
private:
    std::string mLabel = "";
    std::vector<CMesh> mMeshes = {}; //TODO ModelComponent refactor

public:
    // TODO Think! should we pass param by reference? should we use const?
    // Should we have surfaces here? in what way? is there a better way that is still easy to the user?
    CModel(){};
	CModel(CMesh aMesh) {mMeshes.push_back(aMesh);};
    //operator =
	CModel(std::vector<CMesh> aMeshes) : mMeshes(aMeshes){};
	CModel(std::vector<CSurface> aSurfs, std::string aLabel, coord_t aAlpha);
	~CModel();

	void AddMesh(CMesh aMesh);
	//remove Mesh?
	void AddSurf(CSurface aSurf, std::string aLabel, coord_t aAlpha);
	vector<CMesh> GetMeshes() {return mMeshes;}

	// Add Importers
    void ExportToObj(std::string aOutput, bool WithTexture = 1); //TODO const std::string &aOutputFilePath
//    void ExportToFBX(rotation bla bla, bool WithTexture = 1);

    //TODO export to BLOB
};

}; // namespace vivid
#endif //VIVID_MODEL_H