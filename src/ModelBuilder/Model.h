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
        ~CModel();

        // TODO: Make this work
        void AddMeshes(const vector<CModelComponent> &arMeshes);
        int GetNumMeshes(){return this->mMeshes.size();}
        void AddMesh(const CModelComponent &arMesh);
        void AddModel(const CModel &arModel);
        //remove Mesh by label maybe?
        //List of meshes by label?
        inline vector<CModelComponent> GetMeshes() const {return mMeshes;}

        // Add Importers
        // void ImportModel(const std::string &arInputFilePath)
        void ExportToObj(const std::string &arOutputFilePath, bool WithTexture = 1);

        /**
        * Assimp export. writes arModel in aFileType format at aOutputPath
        * @param[in] aOutputPath Path and name for output file
        * @param[in] aFileType 3D filetype format to write to (out of supported options)
        */
        int Export(const std::string &arOutputFilePath, std::string aFileType = "obj");
//    void ExportToFBX(rotation bla bla, bool WithTexture = 1);

        //TODO export to BLOB
    };

}; // namespace vivid
#endif //VIVID_MODEL_H