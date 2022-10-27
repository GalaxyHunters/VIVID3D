#ifndef VIVID_ASSIMPIMPORTEXPORT_H
#define VIVID_ASSIMPIMPORTEXPORT_H

#include "Model.h"
#include "StopMotionAnimation.h"
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ColorMapToPng.h"
#include "src/ImportExport/BlobData.h"
#include <map>
namespace vivid {
    namespace AssimpExport {
        /**
         * Assimp export. writes arModel in aFileType format and returns as blob
         * @param[in] arModel CModel to be exported
         * @param[in] aFileType 3D filetype format to write to (out of supported options)
         * @return CBlobData files as blobs
         */
        CBlobData AssimpExporter(vivid::CModel &arModel, const std::string &arFileType);
        /**
         * Assimp export. writes arModel in aFileType format and returns as blob
         * @param[in] arAnimation CModel to be exported
         * @param[in] aFileType 3D filetype format to write to (out of supported options)
         * @return CBlobData files as blobs
         */
        CBlobData AnimationExporter(vivid::CAnimation &arAnimation, const std::string &arFileType);
        /**
         * Assimp export. writes arModel in aFileType format at aOutputPath
         * @param[in] arModel CModel to be exported
         * @param[in] aFileType 3D filetype format to write to (out of supported options)
         * @param[in] aOutputPath Path and name for output file
         */
        bool AssimpExporter(vivid::CModel &arModel, const std::string &arFileType, std::string aOutputPath);
        bool AnimationExporter(vivid::CAnimation &arAnimation, const std::string &arFileType, std::string aOutputPath);
        // TODO: CModel AssimpImport(const std::string &arInputPath)
        // TODO: All of these should be cpp only code, none of them need to be in the header
        string AddTexture(CColorMap arMeshCLM);

        aiNode *GenerateNode(string aNodeName, size_t aMeshIndexStart, size_t aMeshIndexEnd);

        aiScene *GenerateScene(const vivid::CModel &model);

        aiMaterial *GenerateMaterial(const vivid::CMaterial &arMaterial, const string& aTextureName, size_t mat_index);

        aiMesh *GenerateMesh(vivid::CModelComponent *apMesh);

        string GenerateTexturePNG(CColorMap &arMeshTexture, std::string &arOutputPath);

        aiTexture *GenerateTextureEmbedded(CColorMap &arMeshTexture);


        //------------animation helper functions------------------------------------------------------
        aiScene *GenerateAnimationScene(vivid::CAnimation &arAnimation);

        aiAnimation *GenerateAnimation(CAnimation &arAnimation);

        aiNodeAnim *GenerateAnimationChannel(CAnimation &arAnimation, size_t aIndex);

        aiAnimation *GenerateStopMotionAnimation(CStopMotionAnimation &arSMAnimation);

        void ScaleAnimation(aiNodeAnim *arAnim, coord_t aDuration, CPoint *arAnimValue, coord_t aStartTime = 0);

        void StopMotionScaleAnimation(aiNodeAnim *arAnim, coord_t aDuration, CPoint *arAnimValue, coord_t aStartTime = 0);

        void RotateAnimation(aiNodeAnim *arAnim, coord_t aDuration, CPoint *arAnimValue, coord_t aStartTime = 0);

        void MoveAnimation(aiNodeAnim *arAnim, coord_t aDuration, CPoint *arAnimValue, coord_t aStartTime = 0);
    };
}

#endif //VIVID_ASSIMPIMPORTEXPORT_H