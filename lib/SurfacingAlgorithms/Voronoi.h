#ifndef VIVID_VORONOI_H
#define VIVID_VORONOI_H

#include "Point.h"
#include "Voronoi3D.hpp"


namespace vivid
{
/* Implementing surfacing by Voronoi algorithm */
class CVoronoi {
public:
    Voronoi3D mData = {Vector3D(), Vector3D()};


    CVoronoi() {

    }
    CVoronoi(const CVoronoi &arVoronoi) {}
    // FindBox
    double FindContainingRadius(const vector<CPoint>& arPoints);
    // TODO: typedef pair(cpoint, cpoint) for box
    std::vector<CPoint> FindContainingBox(const vector<CPoint>& arPoints);

    vector<CPoint> PreProcessPoints(const vector<CPoint> &arPoints);
    // Create Voronoi3D and save it

    // operator=
    // set get

    //inline void SetData(const Voronoi3D &arData) { mData = arData; }

    //void ConvertToVorn(std::vector<CPoint>& arInputPoints, std::vector<Vector3D>& arNewPoints);
    std::pair<std::vector<Vector3D>, std::vector<std::vector<size_t>>> MosheVoronoi(const std::vector<CPoint>& arInputPoints);
    std::pair<std::vector<Vector3D>, std::vector<std::vector<size_t>>> ComputeVoronoi(std::vector<CPoint>& arInputPoints, std::pair<CPoint,CPoint> Box);
};



}; // namespace vivid
#endif //VIVID_VORONOI_H