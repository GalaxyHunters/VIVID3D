#ifndef VIVID_VORONOI_H
#define VIVID_VORONOI_H

#include "Point.h"
#include "Voronoi3D.hpp"


namespace vivid
{
/* Implementing surfacing by RunVorn algorithm */
class CVoronoi {
public:
    Voronoi3D mData = {Vector3D(), Vector3D()};

    CVoronoi() {}
    CVoronoi(const CVoronoi &arVoronoi) {}

    // operator=
    // set get

    //inline void SetData(const Voronoi3D &arData) { mData = arData; }

    std::vector<Vector3D> ConvertToVorn(const std::vector<CPoint>& arInputPoints);
    void ComputeVoronoi(const std::vector<CPoint>& arInputPoints, std::pair<CPoint,CPoint> Box);
    std::vector<std::vector<size_t>> GetFaces();
};



}; // namespace vivid
#endif //VIVID_VORONOI_H