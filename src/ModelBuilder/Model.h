#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "Surf.h"

namespace vivid
{

class CModel
{
private:
    std::vector<CMesh> mMeshes;

    //output functions
    void WriteObj(std::ofstream &aOBJFile, std::ofstream &aMTLFile, CMesh *aMesh, size_t *mtl_counter, size_t aPointsCounter);

    void WriteNewMtl(std::ofstream &aOBJFile, std::ofstream &aMTLFile, size_t *mtl_counter, color_t color, coord_t aAlpha);

//	void WriteNewFace(ofstream& aOBJFile, CIndexedFace aFace);
    void WriteNewFace(std::ofstream &aOBJFile, CIndexedFace aFace, size_t aPointsCounter);

    void WriteObjTexture(std::ofstream &aOBJFile, std::ofstream &aMTLFile, CMesh *aMesh, size_t *mtl_counter, std::string aTextureName,
                    coord_t aTextureSize, size_t aPointsCounter);

    void WriteMtlTexture(std::ofstream &aOBJFile, std::ofstream &aMTLFile, size_t *mtl_counter, std::string aTextureName,
                         coord_t aAlpha);

    void WriteNewFaceTexture(std::ofstream &aOBJFile, CIndexedFace aFace, size_t aPointsCounter);

public:
    CModel() : mMeshes() {};

    CModel(const vector<CMesh> &arMeshes) : mMeshes(arMeshes) {};

    CModel(std::vector<CSurf> aSurfs, std::string aLabel, coord_t aAlpha);

    ~CModel();

    void ExportToObj(std::string aOutput);

    void ExportToObjTexture(std::string aOutput);

//	static CMesh load(string inputFile); // TODO add a read obj func
    void AddMesh(CMesh aMesh);

    void AddSurf(CSurf aSurf, std::string aLabel, coord_t aAlpha);

    std::vector<CMesh> GetMeshes() { return mMeshes; }

};

}; // namespace vivid

#endif