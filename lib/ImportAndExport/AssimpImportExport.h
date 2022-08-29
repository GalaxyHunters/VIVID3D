
#ifndef VIVID_ASSIMPIMPORTEXPORT_H
#define VIVID_ASSIMPIMPORTEXPORT_H

#include "Model.h"
#include "Animation.h"
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "DataToImage.h"
#include <map>

namespace vivid
{
    /**
     * Assimp export. writes arModel in aFileType format at aOutputPath
     * @param[in] arModel CModel to be exported
     * @param[in] aFileType 3D filetype format to write to (out of supported options)
     * @param[in] aOutputPath Path and name for output file
     */
    int AssimpExporter(CModel &arModel, std::string aFileType, std::string aOutputPath); //=1

    void AnimationExporter(CAnimation &arAnimation, std::string aFileType, std::string &arOutputPath, bool aWithTexture);

    aiScene * GenerateScene(CModel& model, std::string &arOutputPath, bool aImbeddedTexture = false);
    aiMaterial * GenerateMaterial(vivid::CModelComponent& mesh, string aTextureName);
    aiMesh * GenerateMesh(vivid::CModelComponent * apMesh);
    string GenerateTexturePNG(CColorMap &arMeshTexture, std::string &arOutputPath);
    aiTexture *GenerateTextureEmbedded(CColorMap &arMeshTexture);




    // TODO export to BLOB

    // TODO To think about some function that get rotation and model? (we need to think about it...)
    // void AnimationExporter(CModel &arModel, std::string &arFileType, std::string &arOutputPath, bool aWithTexture);


} // namespace vivid
#endif //VIVID_ASSIMPIMPORTEXPORT_H
