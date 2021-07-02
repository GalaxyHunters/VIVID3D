#ifndef VIVID_FBXIMPORTEXPORT_H
#define VIVID_FBXIMPORTEXPORT_H

#include "fbxsdk.h"
#include "ImportAndExport/FbxCode.h"


namespace vivid
{

// TODO (TOMER) by interface should be tohether with RotateAnimTextures. Also, what is aLength? and why RotAxis is only by x,y, or z?
void RotateAnim(CModel &arModel, float aLength, float aDuration, int aRotAxis, std::string &aOutputFilePath);
/* this function takes a model, length of rotation, how long the animation should exist, a rotation axis, and an output files and exports an animation file to it. */
void RotateAnimTextures(CModel model, float length, float duration, int RotAxis, std::string &outputfile);

//TODO arInterval and not vector of intervals? aOutputFilePath should be with ref?
// TODO (TOMER) by interface should be tohether with RotateAnimTextures.
void Animate(std::vector<CModel> &arModels, double arInterval, std::string aOutputFilePath); //TODO (TOMER): that should have been string. what is this c <-> CPP mix???
/* this function takes a vector aof models and creates a frame by frame animation with every model being a frame.*/
void AnimateTextures(std::vector<CModel> & arModels, double Interval, std::string OutputFile);



// TODO (Tomer): These to seems duplicate and also inner function that should be only at CPP and not in the header interface.
void FbfAnimNodes(std::vector<FbxNode*> FbxMeshNodes, std::string outputfile, float interval);
/* this function creates the animation file and exports it. it takes a series of existing CMesh nodes and makes a frame by frame
animation in which the time interval between each frame is determined by the user. */

void FbfAnimMeshes(std::vector<FbxMesh*> FbxMeshs, char* outputfile, float interval); //TODO outputfile string!!!
/* this function creates the animation file and exports it. it takes a series of existing CMesh meshes and makes a frame by frame
animation in which the time interval between each frame is determined by the user. */


}; // namespace vivid
#endif //VIVID_FBXIMPORTEXPORT_H
