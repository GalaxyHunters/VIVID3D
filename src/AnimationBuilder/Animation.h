#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include "../ModelBuilder/Model.h"
#include <iostream>
#include "fbxsdk.h"
#include "FbxFunc.h"

//#include "Surf.h"
#include <vector>
#include "string"


using namespace std;


void FbfAnimNodes(vector<FbxNode*> FbxMeshNodes, char* outputfile, float interval);
/* this function creates the animation file and exports it. it takes a series of existing CMesh nodes and makes a frame by frame
animation in which the time interval between each frame is determined by the user. */

void FbfAnimMeshes(vector<FbxMesh*> FbxMeshs, char* outputfile, float interval);
/* this function creates the animation file and exports it. it takes a series of existing CMesh meshes and makes a frame by frame
animation in which the time interval between each frame is determined by the user. */

void RotateAnim(CModel model, float length, float interval, int RotAxis, char* outputfile);
/* this function takes a model, length of rotation, how long the animation should exist, a rotation axis, and an output files and exports an animation file to it. */

void Animate(vector<CModel>, double interval, char* outputfile);
/* this function takes a vector aof models and creates a frame by frame animation with every model being a frame.*/

void RotateAnimTextures(CModel model, float length, float duration, int RotAxis, const string& outputfile);

void AnimateTextures(vector<CModel> Models, double Interval, const string& OutputFile);





#endif
