#ifndef VIVID_SHAPES_H
#define VIVID_SHAPES_H

#include "Mesh.h"
#include "ColorMap.h"

//#include <functional>


namespace vivid
{

// TODO: get ride of size_t

// TODO These functions should be static and go by the naming conventions (and better naming, consistant naming...)
// TODO also, use Doxygen commenting standards
// TODO make sure all the code parts here is consistent, names of param are sometime capitalized, some not... the order of actions in the algorithm, the inner naming etc...
// all 3 vec should be passed with CPoint const&


CMesh CreateCubeMesh(double aSize, coord_t aColor, coord_t aAlpha, const CPoint &arCenter);
/*Creates a Cube*/

CMesh CreateBoxMesh(double sizeX, double aSizeY, double aSizeZ, coord_t aColor, coord_t aAlpha, const CPoint &arCenter);
/*Creates a Box*/

CMesh CreateSphereMesh(std::size_t aNumOfMeridians, std::size_t aNumOfParallels, double aRadius, const CPoint &arCenter, coord_t aColor, coord_t aAlpha, const std::string aLabel);
/*Creates a sphere*/

CMesh CreateEllipsoidMesh(std::size_t aNumOfMeridians, std::size_t aNumOfParallels, std::vector<double> aRadiusVec,
                          const CPoint &arCenter, std::vector<double> aMajorAxis, std::vector<double> aMiddleAxis, std::vector<double> aMinorAxis,
                          coord_t aColor, coord_t aAlpha, std::string aLabel);

CMesh CreateEllipsoidByTransformMesh(size_t aNumOfMeridians, size_t aNumOfParallels, vector<double> aRadiusVec,
                                     const CPoint &arCenter, vector<double> aMajorAxis, vector<double> aMiddleAxis,
                                     vector<double> aMinorAxis, coord_t aColor, coord_t aAlpha, string aLabel);

CMesh CreateArrowMesh(double aWidth, double aPCRatio, const CPoint &arCenter, std::vector<double> aDirVec, double aColor, double aAlpha, std::string aLabel);
/*Creates a arrow*/

} // namespace vivid
#endif //VIVID_SHAPES_H