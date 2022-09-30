#ifndef VIVID_SHAPES_H
#define VIVID_SHAPES_H

#include "Mesh.h"
#include "Line.h"

namespace vivid
{
CMesh CreatePlaneMesh(const CPoint &arCenter, coord_t aSize, const string& arColor, normal_float aOpacity, const std::string &arLabel);

CMesh CreateCubeMesh(const CPoint &arCenter, coord_t aSize, const string& arColor, normal_float aOpacity, const std::string &arLabel);

CMesh CreateBoxMesh(const CPoint &arCenter, const CPoint &arSize, const string& arColor, normal_float aOpacity, const std::string &arLabel);

CMesh CreateSphereMesh(const CPoint &arCenter, coord_t aRadius, std::size_t aNumOfMeridians, std::size_t aNumOfParallels,
                       const string& arColor, normal_float aOpacity, const std::string &arLabel);

CMesh CreateEllipsoidMesh(const CPoint &arCenter, const CPoint &arScaleVec, size_t aNumOfMeridians, size_t aNumOfParallels,
                          const CPoint &arMajorAxis, const CPoint &arMiddleAxis, const CPoint &arMinorAxis,
                          const string& arColor, normal_float aOpacity, const std::string &arLabel);

CMesh CreateArrowMesh(const CPoint &arCenter, const CPoint &arDirVec, coord_t aWidth, coord_t aPCRatio,
                      const string& arColor, normal_float aOpacity, const std::string &arLabel);

std::pair<CLines, CLines> CreateGrid(coord_t aScale, size_t aNumOfTicks, coord_t aTickSize=2.);

} // namespace vivid
#endif //VIVID_SHAPES_H