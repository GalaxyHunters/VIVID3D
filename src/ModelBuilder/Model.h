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
        /**
        * OBJ export. writes arModel to arOutputFilePath
        * @param[in] arOutputFilePath Path and name for output file
        * @param[in] WithTexture Whether to include Texture Data or not
        */
        void ExportToObj(const std::string &arOutputFilePath, bool WithTexture = 1);

        /**
        * Assimp export. writes arModel in aFileType format at arOutputFilePath
        * @param[in] arOutputFilePath Path and name for output file
        * @param[in] aFileType 3D filetype format to write to (out of supported options)
        */
        int Export(const std::string &arOutputFilePath, const std::string& arFileType = "gltf2");
        /**
        * Assimp export. returns arModel in aFileType format
        * @param[in] aFileType 3D filetype format to write to (out of supported options)
        * @returns void* file blob encoded in provided aFileType
        */
        CBlobData ExportToBlob(const std::string& arFileType = "glb");
    };

}; // namespace vivid
#endif //VIVID_MODEL_H