//
// Created by zorik on 15/09/2019.
//

#ifndef VIVID_FBXFUNC_H
#define VIVID_FBXFUNC_H

#include "../ModelBuilder/Model.h"
#include <iostream>
//#include "ReadBinFile.h"
#include <fbxsdk.h>
#include <vector>
#include <sys/stat.h>
#include "string"
//#include <direct.h>


void FbxSceneExport(FbxScene* scene, const string& outputfile);

void FbxMeshExport(FbxMesh* Mesh, char* output);
/* exports a FbxMesh*/

void FbxNodeExport(FbxNode* node, char* output);
/* exports a FbxNode*/

FbxNode* OneMeshToFbxTextures(CMesh mesh, FbxScene* scene);

FbxNode* OneModelToFbxTextures(CModel model, FbxScene* scene, const string& outputpath);

int GetCPSize(CMesh mesh);

FbxNode* OneModelToFbx(CModel model);

FbxNode* OneMeshToFbx(CMesh CMesh); //TODO DELETE THIS LATER
/* this function is used to convert a model object into a FbxNode object which is animatable*/

inline bool CheckTexture(const string& texture){
    struct stat buffer;
    return (stat (texture.c_str(), &buffer) == 0);
}

//vector<FbxDouble3> MeshToFbxMaterials(CMesh mesh);


#endif //VIVID_FBXFUNC_H
