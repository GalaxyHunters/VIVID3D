//
// Created by zorik on 15/09/2019.
//

#ifndef VIVID_FBXFUNC_H
#define VIVID_FBXFUNC_H

#include "ModelBuilder/Model.h"
#include <iostream>
#include "Utils/ReadBinFile.h"
#include <fbxsdk.h>
#include <vector>
#include "string"

void FbxMeshExport(FbxMesh* Mesh, char* output);
/* exports a FbxMesh*/

void FbxNodeExport(FbxNode* node, char* output);
/* exports a FbxNode*/

FbxNode* OneModelToFbx(CModel model);
/* this function is used to convert a model object into a FbxNode object which is animatable*/


//vector<FbxDouble3> MeshToFbxMaterials(CMesh mesh);


#endif //VIVID_FBXFUNC_H
