#ifndef VIVID_DECIMATE_H
#define VIVID_DECIMATE_H

#include "Point.h"
#include "Face.h"
#include "mdMeshDecimator.h"

using namespace MeshDecimation; //TODO
namespace vivid
{

/**
 * Decimate a triangle mesh (quadric edge collapse). The mesh is passed in aPoints / aFaces and is REPLACED by the
 * decimated mesh, so nothing is copied; the input is released as soon as the decimator owns its own data.
 * Faces must be triangles; a face's colour (UV coordinate) is kept.
 */
void DecimateMesh(std::vector<CPoint>& aPoints, std::vector<CFace>& aFaces, int aTargetVerticesN, int aTargetTrianglesN, float aMaxError);

}; // namespace vivid
#endif //VIVID_DECIMATE_H