#include "Model.h"
#include "External.h"
#include "ImportAndExport/ObjExportImport.h"

using namespace vivid;
using namespace std;


CModel::CModel(std::vector<CSurface> aSurfs, std::string aLabel, coord_t aAlpha){
    for (auto it = aSurfs.begin(); it != aSurfs.end(); it++){ //TODO should be for each.
        mMeshes.push_back( (*it).ToMesh(aLabel, aAlpha) );
    }
}

void CModel::AddMesh(CMesh aMesh) {
	mMeshes.push_back(aMesh);
}

void CModel::AddSurf(CSurface aSurf, std::string aLabel, coord_t aAlpha){
    mMeshes.push_back(aSurf.ToMesh(aLabel, aAlpha));
}


void CModel::ExportToObj(string aOutput, bool WithTexture){
    OBJExporter(*this, aOutput, WithTexture);
}

CModel::~CModel() = default;