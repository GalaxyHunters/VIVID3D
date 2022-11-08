#include "Animation.h"
#include "AssimpImportExport.h"
namespace vivid {

    void CAnimation::Export(const std::string &arOutputFilePath, std::string aFileType) {
        AssimpExport::AnimationExporter(*this, aFileType, arOutputFilePath);
    }
    CBlobData CAnimation::ExportToBlob(const std::string& arFileType) {
        return AssimpExport::AnimationExporter(*this, arFileType);
    }

    void CAnimation::AddModels(const std::vector<CModel> &arModels) {
        for (auto model: arModels) {
            mFrames.emplace_back(CFrame(model));
        }
    }

    void CAnimation::AddModel(const CModel &arModel) {
        mFrames.emplace_back(CFrame(arModel));
    }

}