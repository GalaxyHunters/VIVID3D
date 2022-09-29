#ifndef VIVID_MODEL_H
#define VIVID_MODEL_H

#include "Mesh.h"

namespace vivid
{

    class CModel
    {
    private:
        std::vector<CModelComponent> mMeshes = {};

    public:
        CModel(){};
        CModel(const CModelComponent& arMesh) {mMeshes.push_back(arMesh);};
        //operator =
        CModel(const std::vector<CModelComponent> &arMeshes) : mMeshes(arMeshes){};
        ~CModel();

        void AddMeshes(const vector<CModelComponent> &arMeshes);

        void AddMesh(const CModelComponent &arMesh);
        void AddModel(const CModel &arModel);
        //remove Mesh by label maybe?
        //List of meshes by label?
        inline size_t GetNumMeshes() const { return mMeshes.size(); }
        inline vector<CModelComponent> GetMeshes() const {return mMeshes;}

        // Add Importers
        // void ImportModel(const std::string &arInputFilePath)
        void ExportToObj(const std::string &arOutputFilePath, bool WithTexture = 1);

        /**
        * Assimp export. writes arModel in aFileType format at aOutputPath
        * @param[in] aOutputPath Path and name for output file
        * @param[in] aFileType 3D filetype format to write to (out of supported options)
        */
        int Export(const std::string &arOutputFilePath, const std::string& arFileType = "obj");
//    void ExportToFBX(rotation bla bla, bool WithTexture = 1);

        //TODO export to BLOB
    };

}; // namespace vivid
#endif //VIVID_MODEL_H