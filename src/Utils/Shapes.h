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
// TODO make sure all the code parts here is consistent, names of param are sometime capitalized, some not... the order of actions in the algorithm, the inner naming etc...

CMesh CreateBoxMesh(double sizeX, double sizeY, double sizeZ, coord_t color, coord_t alpha, vector<double> position);
/*Creates a cube*/

CMesh CreateSphereMesh(size_t num_of_meridians, size_t num_of_parallels, double radius, vector<double> CenterPoint, coord_t Color, coord_t Alpha, string Label);
/*Creates a sphere*/

CMesh CreateEllipsoidMesh(size_t NumOfMeridians, size_t NumOfParallels, vector<double> Radii, vector<double> CenterPoint, vector<double> MajorAxis, vector<double> MiddleAxis, vector<double> MinorAxis, coord_t Color, coord_t Alpha, string Label);

CMesh CreateArrowMesh(double Width, double PCRatio, vector<double> aPos, vector<double> DirVec, double Color, double Alpha, string Label);
/*Creates a arrow*/


#endif