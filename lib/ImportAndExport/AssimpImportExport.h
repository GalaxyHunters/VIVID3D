#ifndef VIVID_ASSIMPIMPORTEXPORT_H
#define VIVID_ASSIMPIMPORTEXPORT_H

#include "Model.h"
#include "Animation.h"

namespace vivid
{

    void ModelExporter(CModel &arModel, std::string &arFileType, std::string &arOutputPath, bool aWithTexture); //=1

    void AnimationExporter(CAnimation &arAnimation, std::string &arFileType, std::string &arOutputPath, bool aWithTexture);


//    TODO export to BLOB

    // TODO To think about some function that get rotation and model? (we need to think about it...)
//    void AnimationExporter(CModel &arModel, std::string &arFileType, std::string &arOutputPath, bool aWithTexture);


} // namespace vivid
#endif //VIVID_ASSIMPIMPORTEXPORT_H
