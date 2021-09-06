#ifndef VIVID_MODEL_H
#define VIVID_MODEL_H

#include "Mesh.h"
#include "Surface.h"


namespace vivid
{

class CModel
{
private:
    std::vector<CModelComponent> mMeshes = {};

public:
    // Should we have surfaces here? in what way? is there a better way that is still easy to the user?
    CModel(){};
    CModel(const CModelComponent &arMesh) {mMeshes.push_back(arMesh);};
    //operator =
    // TODO: Make this work
    CModel(const std::vector<CModelComponent> &arMeshes) : mMeshes(arMeshes){};
    //TODO: Remove this when Mesh/Surface refactor done
    CModel(const std::vector<CSurface> &arSurfs, std::string aLabel, coord_t aAlpha);
	~CModel();

    // TODO: Make this work
	void AddMeshes(const vector<CModelComponent> &arMeshes);
	void AddMesh(const CModelComponent &arMesh);
	//remove Mesh by label maybe?
    //List of meshes by label?
	void AddSurf(const CSurface &arSurf, std::string aLabel, coord_t aAlpha);
	inline const vector<CModelComponent> GetMeshes() {return mMeshes;}

	// Add Importers
    void ExportToObj(std::string aOutput, bool WithTexture = 1); //TODO const std::string &aOutputFilePath
//    void ExportToFBX(rotation bla bla, bool WithTexture = 1);

    //TODO export to BLOB
};

}; // namespace vivid
#endif //VIVID_MODEL_H