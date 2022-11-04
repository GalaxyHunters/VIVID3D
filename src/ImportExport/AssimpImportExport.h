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
#include "BlobData.h"
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
        void AssimpExporter(vivid::CModel &arModel, const std::string &arFileType, std::string aOutputPath);
        /**
         * Assimp export. writes arModel in aFileType format and returns as blob
         * @param[in] arAnimation CModel to be exported
         * @param[in] aFileType 3D filetype format to write to (out of supported options)
         * @param[in] aOutputPath Path and name for output file
         */
        void AnimationExporter(vivid::CAnimation &arAnimation, const std::string &arFileType, std::string aOutputPath);

        // TODO: CModel AssimpImport(const std::string &arInputPath)
    };
}

#endif //VIVID_ASSIMPIMPORTEXPORT_H