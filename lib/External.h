#ifndef External_H
#define External_H

#include "Point.h"
#include "IndexedFace.h"
#include "Voronoi3D.hpp"
#include "mdMeshDecimator.h"
#include "../lib/huji-rich-Elad3DFast/source/3D/GeometryCommon/Vector3D.hpp"
#include "lodepng.h"


using namespace MeshDecimation; //TODO
using namespace vivid;
//namespace vivid
//{

void ConvertToVorn(std::vector<CPoint>& arInputPoints, std::vector<Vector3D>& arNewPoints);
std::pair<std::vector<Vector3D>, std::vector<std::vector<size_t> > > compute_vornoi(std::vector<CPoint>& arInputPoints, double aBoxR);
std::pair<std::vector<CPoint>, std::vector<CIndexedFace> > DecimateMesh(std::vector<CPoint>& aPoints, std::vector<CIndexedFace> aFaces, int aTargetVerticesN, int aTargetTrianglesN, float aMaxError);
void encodePNG(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height);

//}; // namespace vivid
#endif