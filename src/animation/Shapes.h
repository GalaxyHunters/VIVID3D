#include <iostream>
#include <fbxsdk.h>
#include "Mesh.h"
#include "ColorMap.h"

#ifndef  SHAPES_H
#define SHAPES_H

using namespace std;

class Shapes
{
public:
	Shapes();
	~Shapes();
	
	static CMesh CreateCubeMesh(double sizeX, double sizeY, double sizeZ, cord_t color, cord_t alpha, vector<double> position);
	/*Creates a cube*/

    static CMesh CreateSphereMesh(size_t num_of_meridians, size_t num_of_parallels, double radius, cord_t Color, cord_t Alpha, vector<double> CenterPoint);
    /*Creates a sphere*/

    static CMesh CreateArrowMesh(double x, double y, double z, double chest_length, double pointer_length, double width, cord_t color);
    /*Creates a arrow*/

    static FbxMesh* Shapes::createCube(int sizeX, int sizeY, int sizeZ);
	/* this function creates a CCmesh object containing a sphere, the sphere is created in the position that the user dictates and has a radius also according to the users input.*/

};


#endif