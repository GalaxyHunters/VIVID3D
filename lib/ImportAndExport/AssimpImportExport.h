
#ifndef VIVID_ASSIMPIMPORTEXPORT_H
#define VIVID_ASSIMPIMPORTEXPORT_H

#include "Model.h"
#include "StopMotionAnimation.h"
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "DataToImage.h"
#include <map>
namespace vivid {
    class CAssimpExport {
    public:
        static map<string, string> TextureNameToIndex;
        static string OutputPath;

        /**
         * Assimp export. writes arModel in aFileType format at aOutputPath
         * @param[in] arModel CModel to be exported
         * @param[in] aFileType 3D filetype format to write to (out of supported options)
         * @param[in] aOutputPath Path and name for output file
         */
    public:
        static int AssimpExporter(const vivid::CModel &arModel, std::string aFileType, std::string aOutputPath); //=1
        static int AnimationExporter(vivid::CAnimation &arAnimation, std::string aFileType, std::string aOutputPath);

    private:
        CAssimpExport() { TextureNameToIndex = {}, OutputPath = ""; };

        static string AddTexture(CColorMap arMeshCLM);

        static aiNode *GenerateNode(string aNodeName, size_t aMeshIndexStart, size_t aMeshIndexEnd);

        static aiScene *GenerateScene(const vivid::CModel &model);

        static aiMaterial *GenerateMaterial(vivid::CModelComponent &mesh, string aTextureName);

        static aiMesh *GenerateMesh(vivid::CModelComponent *apMesh);

        static string GenerateTexturePNG(CColorMap &arMeshTexture, std::string &arOutputPath);

        static aiTexture *GenerateTextureEmbedded(CColorMap &arMeshTexture);


        //------------animation helper functions------------------------------------------------------
        static aiScene *GenerateAnimationScene(vivid::CAnimation &arAnimation);

        static aiAnimation *GenerateAnimation(const CAnimation &arAnimation);

        static aiNodeAnim *GenerateAnimationChannel(const CAnimation &arAnimation, size_t aIndex);

        static aiAnimation *GenerateStopMotionAnimation(CStopMotionAnimation &arSMAnimation);

        static void
        ScaleAnimation(aiNodeAnim *arAnim, duration_t aDuration, CPoint *arAnimValue, duration_t aStartTime = 0);

        static void StopMotionScaleAnimation(aiNodeAnim *arAnim, duration_t aDuration, CPoint *arAnimValue,
                                             duration_t aStartTime = 0);

        static void
        RotateAnimation(aiNodeAnim *arAnim, duration_t aDuration, CPoint *arAnimValue, duration_t aStartTime = 0);

        static void
        MoveAnimation(aiNodeAnim *arAnim, duration_t aDuration, CPoint *arAnimValue, duration_t aStartTime = 0);




        // TODO export to BLOB

        // TODO To think about some function that get rotation and model? (we need to think about it...)
        // void AnimationExporter(CModel &arModel, std::string &arFileType, std::string &arOutputPath, bool aWithTexture)
    };
}

#endif //VIVID_ASSIMPIMPORTEXPORT_H
