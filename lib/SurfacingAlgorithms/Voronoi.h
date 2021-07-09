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
    std::pair<CPoint, CPoint> FindContainingBox(const vector<CPoint>& arPoints);
    // Create Voronoi3D and save it

    // operator=
    // set get

    //inline void SetData(const Voronoi3D &arData) { mData = arData; }

    //void ConvertToVorn(std::vector<CPoint>& arInputPoints, std::vector<Vector3D>& arNewPoints);
    void MosheVoronoi(std::vector<CPoint>& arInputPoints);
    std::pair<std::vector<Vector3D>, std::vector<std::vector<size_t> > > ComputeVoronoi(std::vector<CPoint>& arInputPoints, std::pair<CPoint,CPoint> Box);
};



}; // namespace vivid
#endif //VIVID_VORONOI_H