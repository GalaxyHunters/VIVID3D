#ifndef VIVID_SHAPES_H
#define VIVID_SHAPES_H

#include "Mesh.h"
#include "ColorMap.h"

//#include <functional>


namespace vivid
{

// TODO: get ride of size_t

// TODO add cubeMesh (use boxMesh with sizeX... are all one param
// TODO These functions should be static and go by the naming conventions (and better naming, consistant naming...)
// TODO also, use Doxygen commenting standards
// TODO make sure all the code parts here is consistent, names of param are sometime capitalized, some not... the order of actions in the algorithm, the inner naming etc...

CMesh CreateBoxMesh(double sizeX, double sizeY, double sizeZ, coord_t color, coord_t alpha, std::vector<double> position);
/*Creates a cube*/

CMesh CreateSphereMesh(std::size_t num_of_meridians, std::size_t num_of_parallels, double radius, const CPoint &arCenter, coord_t Color, coord_t Alpha, const std::string Label);
/*Creates a sphere*/

CMesh CreateEllipsoidMesh(std::size_t NumOfMeridians, std::size_t NumOfParallels, std::vector<double> aRadiusVec,
                                 std::vector<double> CenterPoint, std::vector<double> MajorAxis, std::vector<double> MiddleAxis, std::vector<double> MinorAxis,
                                 coord_t Color, coord_t Alpha, std::string Label);

CMesh CreateArrowMesh(double Width, double PCRatio, std::vector<double> aPos, std::vector<double> DirVec, double Color, double Alpha, std::string Label);
/*Creates a arrow*/

} // namespace vivid
#endif //VIVID_SHAPES_H