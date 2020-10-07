#ifndef  SHAPES_H
#define SHAPES_H

#include <iostream>
#include <fbxsdk.h>
#include "../ModelBuilder/Mesh.h"
#include "../Utils/ColorMap.h"
#include <functional>

using namespace std;


// TODO add cubeMesh (use boxMesh with sizeX... are all one param
// TODO These functions should be static and go by the naming conventions (and better naming, consistant naming...)
// TODO also, use Doxygen commenting standards

CMesh CreateBoxMesh(double sizeX, double sizeY, double sizeZ, coord_t color, coord_t alpha, vector<double> position);
/*Creates a cube*/

CMesh CreateSphereMesh(size_t num_of_meridians, size_t num_of_parallels, double radius, vector<double> CenterPoint, coord_t Color, coord_t Alpha, string Label);
/*Creates a sphere*/

CMesh CreateEllipsoidMesh(size_t NumOfMeridians, size_t NumOfParallels, vector<double> Radii, vector<double> CenterPoint, vector<double> MajorAxis, vector<double> MiddleAxis, vector<double> MinorAxis, coord_t Color, coord_t Alpha, string Label);

CMesh CreateArrowMesh(double Length, double Width, double PCRatio, vector<double> Base, vector<double> DirVec, double Color, double Alpha, string Label);
/*Creates a arrow*/


// TODO Zohar why does it do here?
FbxMesh* createCube(int sizeX, int sizeY, int sizeZ);
/* this function creates a CCmesh object containing a sphere, the sphere is created in the position that the user dictates and has a radius also according to the users input.*/



#endif