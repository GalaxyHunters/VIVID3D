#ifndef VIVID_ANALYSIS_H
#define VIVID_ANALYSIS_H

#include "VoronoiVolume.h"

//namespace vivid {

// Plan: we will use surf functionality, so also surf will have this functionality ?
using namespace vivid;

// neighbours...

vector<double> GetParticlesVolume(const CVoronoiVolume &arSurf);
vector<coord_t> GetParticlesDensity(const CVoronoiVolume &arSurf, const std::vector<double> &arMassField);
vector<coord_t> GetSurfaceAreas(const CVoronoiVolume &arSurf);
vector<coord_t> CalculateFlux(const CVoronoiVolume &arSurf, const CPoint &arField);

//} // namespace vivid
#endif //VIVID_ANALYSIS_H
