#ifndef VIVID_DECIMATE_H
#define VIVID_DECIMATE_H

#include "Point.h"
#include "Face.h"
#include "mdMeshDecimator.h"

using namespace MeshDecimation; //TODO
namespace vivid
{

// TODO should have gotten Mesh object... by ref, also return value should be changed
std::pair<std::vector<CPoint>, std::vector<CFace> > DecimateMesh(std::vector<CPoint>& aPoints, std::vector<CFace> aFaces, int aTargetVerticesN, int aTargetTrianglesN, float aMaxError);

}; // namespace vivid
#endif //VIVID_DECIMATE_H