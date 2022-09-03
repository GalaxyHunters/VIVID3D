#ifndef VIVID_ANALYSIS_H
#define VIVID_ANALYSIS_H

#include "Surface.h"

//namespace vivid {

// Plan: we will use surf functionality, so also surf will have this functionality ?
using namespace vivid;

// neighbours...

vector<double> GetParticlesVolume(const CSurface &arSurf);
vector<double> GetParticlesDensity(const CSurface &arSurf, const std::vector<double> &arMassField);
vector<double> GetSurfaceAreas(const CSurface &arSurf);
vector<double> CalculateFlux(const CSurface &arSurf, const CPoint &arField);

//} // namespace vivid
#endif //VIVID_ANALYSIS_H
