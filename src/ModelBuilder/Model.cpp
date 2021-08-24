#include "Model.h"
#include "Decimate.h"
#include "ImportAndExport/ObjImportExport.h"

using namespace vivid;
using namespace std;


CModel::CModel(const vector<CSurface> &arSurfs, string aLabel, coord_t aAlpha){
    for (auto it = arSurfs.begin(); it != arSurfs.end(); it++){ //TODO should be for each.
        mMeshes.push_back( (*it).ToMesh(aLabel, aAlpha) );
    }
}
void CModel::AddMeshes(const vector<CModelComponent> &arMeshes) {
    for (auto it = arMeshes.begin(); it != arMeshes.end(); it ++) {
        mMeshes.push_back(*it);
    }
}

void CModel::AddMesh(const CModelComponent &arMesh) {
	mMeshes.push_back(arMesh);
}

void CModel::AddSurf(const CSurface &arSurf, string aLabel, coord_t aAlpha){
    mMeshes.push_back(arSurf.ToMesh(aLabel, aAlpha));
}


void CModel::ExportToObj(string aOutput, bool WithTexture){
    OBJExporter(*this, aOutput, WithTexture);
}

CModel::~CModel() = default;