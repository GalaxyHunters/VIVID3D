#ifndef VIVID_External_H
#define VIVID_External_H

#include "Point.h"
#include "IndexedFace.h"
#include "Voronoi3D.hpp"
#include "mdMeshDecimator.h"
#include "IndexedFace.h"


using namespace MeshDecimation; //TODO
using namespace vivid;

//namespace vivid
//{

void ConvertToVorn(std::vector<CPoint>& arInputPoints, std::vector<Vector3D>& arNewPoints);
// TODO: Alt compute_vornoi func
std::pair<std::vector<Vector3D>, std::vector<std::vector<size_t> > > compute_vornoi(std::vector<CPoint>& arInputPoints, double aBoxR);
std::pair<std::vector<Vector3D>, std::vector<std::vector<size_t> > > compute_vornoi(vector<CPoint>& arInputPoints, vector<Vector3D> Box);

std::pair<std::vector<CPoint>, std::vector<CIndexedFace> > DecimateMesh(std::vector<CPoint>& aPoints, std::vector<CIndexedFace> aFaces, int aTargetVerticesN, int aTargetTrianglesN, float aMaxError);

//}; // namespace vivid
#endif //VIVID_External_H