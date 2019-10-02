#ifndef UTILS_H
#define UTILS_H

#include "Point.h"
#include "Voronoi3D.hpp"
#include "mdMeshDecimator.h"
#include "IndexedFace.h"

using namespace std;
using namespace MeshDecimation;

void ConvertToVorn(vector<CPoint>& arInputPoints, vector<Vector3D>& arNewPoints);
pair<vector<Vector3D>, vector<vector<size_t> > > compute_vornoi(vector<CPoint>& arInputPoints, double aBoxR);
pair<vector<CPoint>, vector<CIndexedFace> > DecimateMesh(vector<CPoint>& aPoints, vector<CIndexedFace>& aFaces, int aTargetVerticesN, int aTargetTrianglesN, float aMaxError);
#endif