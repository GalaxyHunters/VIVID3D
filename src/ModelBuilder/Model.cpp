#include "Model.h"
#include "Decimate.h"
#include "ObjImportExport.h"
#include "AssimpImportExport.h"

using namespace vivid;
using namespace std;

void CModel::AddMeshes(const vector<CModelComponent> &arMeshes) {
    for (const auto & arMesh : arMeshes) {
        mMeshes.push_back(arMesh);
    }
}

void CModel::AddMesh(const CModelComponent &arMesh) {
    mMeshes.push_back(arMesh);
}

void CModel::AddModel(const CModel &arModel) {
    for (auto & it : arModel.GetMeshes()) {
        mMeshes.push_back(it);
    }
}

void CModel::ExportToObj(const string &arOutputFilePath, bool WithTexture){
    OBJExporter(*this, arOutputFilePath, WithTexture);
}

void CModel::Export(const std::string &arOutputFilePath, const std::string& arFileType){
    AssimpExport::AssimpExporter(*this, arFileType, arOutputFilePath);
}

CBlobData CModel::ExportToBlob(const std::string& arFileType) {
    return AssimpExport::AssimpExporter(*this, arFileType);
}

CModel::~CModel() = default;