#ifndef VIVID_FBXCODE_H
#define VIVID_FBXCODE_H

#include "Model.h"
#include "Mesh.h"
#include <fbxsdk.h>
#include <sys/stat.h>

// TODO FIX with ZOHAR


using namespace vivid;

void FbxSceneExport(FbxScene* scene, const std::string& outputfile);

void FbxMeshExport(FbxMesh* Mesh, char* output); //TODO (TOMER): that should have been string. what is this c <-> CPP mix???
/* exports a FbxMesh*/

void FbxNodeExport(FbxNode* node, char* output); //TODO (TOMER): that should have been string. what is this c <-> CPP mix???
/* exports a FbxNode*/

FbxNode* OneMeshToFbxTextures(CMesh mesh, FbxScene* scene);

FbxNode* OneModelToFbxTextures(CModel model, FbxScene* scene, const std::string& outputpath);

int GetCPSize(CMesh mesh);

FbxNode* OneModelToFbx(CModel model);

FbxNode* OneMeshToFbx(CMesh CMesh); //TODO DELETE THIS LATER
/* this function is used to convert a model object into a FbxNode object which is animatable*/

inline bool CheckTexture(const std::string& texture){ // TODO: (TOMER) WTF?
    struct stat buffer;
    return (stat (texture.c_str(), &buffer) == 0);
}

//vector<FbxDouble3> MeshToFbxMaterials(CMesh mesh);


#endif //VIVID_FBXCODE_H
