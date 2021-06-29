#include "Model.h"
#include "External.h"

using namespace vivid;

using namespace boost::algorithm;
using namespace std;

CModel::CModel(vector<CSurface> aSurfs, string aLabel, coord_t aAlpha){
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

void CModel::AddSurf(CSurface aSurf, string aLabel, coord_t aAlpha){
    mMeshes.push_back(aSurf.ToMesh(aLabel, aAlpha));
}


CModel::~CModel() = default;