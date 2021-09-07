#include "Model.h"
#include "Decimate.h"
#include "ImportAndExport/ObjImportExport.h"

using namespace vivid;
using namespace std;


CModel::CModel(const vector<CSurface> &arSurfs, string aLabel, coord_t aAlpha){
    for (const auto & arSurf : arSurfs){
        mMeshes.push_back( arSurf.ToMesh(aLabel, aAlpha) );
    }
}
void CModel::AddMeshes(const vector<CModelComponent> &arMeshes) {
    for (const auto & arMesh : arMeshes) {
        mMeshes.push_back(arMesh);
    }
}

void CModel::AddMesh(const CModelComponent &arMesh) {
	mMeshes.push_back(arMesh);
}

void CModel::AddSurf(const CSurface &arSurf, string aLabel, coord_t aAlpha){
    mMeshes.push_back(arSurf.ToMesh(aLabel, aAlpha));
}

void CModel::AddModel(const CModel &arModel) {
    for (auto & it : arModel.GetMeshes()) {
        mMeshes.push_back(it);
    }
}

void CModel::ExportToObj(string aOutput, bool WithTexture){
    OBJExporter(*this, aOutput, WithTexture);
}

CModel::~CModel() = default;