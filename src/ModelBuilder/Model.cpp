#include "Model.h"
#include "Decimate.h"
#include "ImportAndExport/ObjImportExport.h"
#include "ImportAndExport/AssimpImportExport.h"

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

int CModel::Export(const std::string &arOutputFilePath, std::string aFileType){
    return CAssimpExport::AssimpExporter(*this, aFileType, arOutputFilePath);
}
CModel::~CModel() = default;