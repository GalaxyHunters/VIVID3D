#include "Animation.h"
#include "AssimpImportExport.h"
namespace vivid {

    void CAnimation::Export(const std::string &arOutputFilePath, std::string aFileType) {
        AssimpExport::AnimationExporter(*this, aFileType, arOutputFilePath);
    }

    void CAnimation::AddModels(const std::vector<CModel> &arModels) {
        mModels.insert(mModels.end(), arModels.begin(), arModels.end());
        for (int i = 0; i != arModels.size(); i++) {
            mScaleAnim.push_back(CPoint());
            mMoveAnim.push_back(CPoint());
            mRotateAnim.push_back(CPoint());
        }
    }

    void CAnimation::AddModels(const CModel &arModels) {
        AddModels(vector<CModel>(1, arModels));
    }

}