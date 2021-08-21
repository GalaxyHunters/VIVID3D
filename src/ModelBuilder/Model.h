#ifndef VIVID_MODEL_H
#define VIVID_MODEL_H

#include "Mesh.h"
#include "Surface.h"


namespace vivid
{

class CModel
{
private:
    //std::vector<CMesh> mMeshes = {}; //TODO ModelComponent refactor
    std::vector<CModelComponent> mMeshes = {};

public:
    // TODO Think! should we pass param by reference? should we use const?
    // Should we have surfaces here? in what way? is there a better way that is still easy to the user?
    CModel(){};
    CModel(const CModelComponent &arMesh) {mMeshes.push_back(arMesh);};
    //operator =
    CModel(const std::vector<CModelComponent> &arMeshes) : mMeshes(arMeshes){};
    CModel(const std::vector<CSurface> &arSurfs, std::string aLabel, coord_t aAlpha);
	~CModel();

	void AddMeshes(const vector<CModelComponent> &arMeshes);
	void AddMesh(const CModelComponent &arMesh);
	//remove Mesh?
	void AddSurf(const CSurface &arSurf, std::string aLabel, coord_t aAlpha);
	inline const vector<CModelComponent> GetMeshes() {return mMeshes;}

	// Add Importers
    void ExportToObj(std::string aOutput, bool WithTexture = 1); //TODO const std::string &aOutputFilePath
//    void ExportToFBX(rotation bla bla, bool WithTexture = 1);

    //TODO export to BLOB
};

}; // namespace vivid
#endif //VIVID_MODEL_H