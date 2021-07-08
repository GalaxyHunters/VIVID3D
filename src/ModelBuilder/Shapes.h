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


CMesh CreateCubeMesh(const CPoint &arCenter, coord_t aSize, coord_t aColor, coord_t aAlpha, const std::string &arLabel);
/*Creates a Cube*/

CMesh CreateBoxMesh(const CPoint &arCenter, const CPoint &arSize, coord_t aColor, coord_t aAlpha, const std::string &arLabel);
/*Creates a Box*/

CMesh CreateSphereMesh(const CPoint &arCenter, coord_t aRadius, std::size_t aNumOfMeridians, std::size_t aNumOfParallels, coord_t aColor, coord_t aAlpha, const std::string &arLabel);
/*Creates a sphere*/

CMesh CreateEllipsoidMesh(std::size_t aNumOfMeridians, std::size_t aNumOfParallels, std::vector<coord_t> aRadiusVec,
                          const CPoint &arCenter, std::vector<coord_t> aMajorAxis, std::vector<coord_t> aMiddleAxis, std::vector<double> aMinorAxis,
                          coord_t aColor, coord_t aAlpha, std::string aLabel);

CMesh CreateEllipsoidByTransformMesh(const CPoint &arCenter, const CPoint &arScaleVec, size_t aNumOfMeridians, size_t aNumOfParallels,
                                     vector<coord_t> aMajorAxis, vector<coord_t> aMiddleAxis, vector<coord_t> aMinorAxis,
                                     coord_t aColor, coord_t aAlpha, const std::string &arLabel);

CMesh CreateArrowMesh(const CPoint &arCenter, const CPoint &arDirVec, coord_t aWidth, coord_t aPCRatio, coord_t aColor, coord_t aAlpha, const std::string &arLabel);
/*Creates a arrow*/

} // namespace vivid
#endif //VIVID_SHAPES_H