#include "Model.h"
#include "Decimate.h"
#include "ImportAndExport/ObjImportExport.h"

using namespace vivid;
using namespace std;


CModel::CModel(vector<CSurface> aSurfs, string aLabel, coord_t aAlpha){
    for (auto it = aSurfs.begin(); it != aSurfs.end(); it++){ //TODO should be for each.
        mMeshes.push_back( (*it).ToMesh(aLabel, aAlpha) );
    }
}

void CModel::AddMesh(CMesh aMesh) {
	mMeshes.push_back(aMesh);
}

void CModel::AddSurf(CSurface aSurf, string aLabel, coord_t aAlpha){
    mMeshes.push_back(aSurf.ToMesh(aLabel, aAlpha));
}


void CModel::ExportToObj(string aOutput, bool WithTexture){
    OBJExporter(*this, aOutput, WithTexture);
}

CModel::~CModel() = default;