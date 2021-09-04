#ifndef VIVID_SHAPES_H
#define VIVID_SHAPES_H

#include "Mesh.h"
#include "Line.h"
#include "ColorMap.h"

//#include <functional>

namespace vivid
{

// TODO: get ride of size_t

// TODO These functions should be static and go by the naming conventions (and better naming, consistant naming...)
// TODO also, use Doxygen commenting standards

CMesh CreateCubeMesh(const CPoint &arCenter, quan_t aSize, quan_t aColor, quan_t aAlpha, const std::string &arLabel);

CMesh CreateBoxMesh(const CPoint &arCenter, const CPoint &arSize, quan_t aColor, quan_t aAlpha, const std::string &arLabel);

CMesh CreateSphereMesh(const CPoint &arCenter, quan_t aRadius, std::size_t aNumOfMeridians, std::size_t aNumOfParallels,
                       quan_t aColor, quan_t aAlpha, const std::string &arLabel);

CMesh CreateEllipsoidMesh(const CPoint &arCenter, const CPoint &arScaleVec, size_t aNumOfMeridians, size_t aNumOfParallels,
                          const CPoint &arMajorAxis, const CPoint &arMiddleAxis, const CPoint &arMinorAxis,
                          quan_t aColor, quan_t aAlpha, const std::string &arLabel);

CMesh CreateArrowMesh(const CPoint &arCenter, const CPoint &arDirVec, quan_t aWidth, quan_t aPCRatio,
                      quan_t aColor, quan_t aAlpha, const std::string &arLabel);

std::pair<CLine, CLine> CreateGrid(const size_t aDimensions);

} // namespace vivid
#endif //VIVID_SHAPES_H