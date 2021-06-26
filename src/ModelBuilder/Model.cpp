#include "Model.h"


using namespace boost::algorithm;


CModel::CModel(vector<CSurf> aSurfs, string aLabel, coord_t aAlpha){
    mMeshes = vector<CMesh>();
    for (auto it = aSurfs.begin(); it != aSurfs.end(); it++){
        mMeshes.push_back((*it).ToMesh(aLabel, aAlpha));
    }
}

void CModel::ExportToObj(string aOutput, bool WithTexture){
    ::ExportToObj(this, aOutput, WithTexture);
}

void CModel::AddMesh(CMesh aMesh) {
	mMeshes.push_back(aMesh);
}

void CModel::AddSurf(CSurf aSurf, string aLabel, coord_t aAlpha){
    mMeshes.push_back(aSurf.ToMesh(aLabel, aAlpha));
}


CModel::~CModel() = default;