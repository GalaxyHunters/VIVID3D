
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
#include "Model.h"
#include <map>

namespace vivid
{
    void ModelExporter(CModel &arModel, std::string arFileType, std::string arOutputPath); //=1

    void AnimationExporter(CAnimation &arAnimation, std::string arFileType, std::string &arOutputPath, bool aWithTexture);

    aiScene * GenerateScene(CModel& model, std::string arFileType);
    aiMaterial * GenerateMaterial(vivid::CModelComponent& mesh, string aTextureName);
    aiMesh * GenerateMesh(vivid::CModelComponent * apMesh);


    vector<size_t> GetTextureIndexesList(vivid::CModelComponent* mesh); // REDACTED


    // TODO export to BLOB

    // TODO To think about some function that get rotation and model? (we need to think about it...)
    // void AnimationExporter(CModel &arModel, std::string &arFileType, std::string &arOutputPath, bool aWithTexture);


} // namespace vivid
#endif //VIVID_ASSIMPIMPORTEXPORT_H
