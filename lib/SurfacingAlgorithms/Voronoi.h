#ifndef VIVID_VORONOI_H
#define VIVID_VORONOI_H

#include "Point.h"
#include "Voronoi3D.hpp"


namespace vivid
{

//void ConvertToVorn(std::vector<CPoint>& arInputPoints, std::vector<Vector3D>& arNewPoints);

// TODO: Alt compute_vornoi func
std::pair<std::vector<Vector3D>, std::vector<std::vector<size_t> > > compute_vornoi(std::vector<CPoint>& arInputPoints, double aBoxR);
std::pair<std::vector<Vector3D>, std::vector<std::vector<size_t> > > compute_vornoi(std::vector<CPoint>& arInputPoints, std::pair<CPoint,CPoint> box);


}; // namespace vivid
#endif //VIVID_VORONOI_H