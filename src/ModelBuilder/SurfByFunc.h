#ifndef VIVID_SURFBYFUNC_H
#define VIVID_SURFBYFUNC_H

#include "Mesh.h"
#include "ColorMap.h"

namespace vivid
{
    // Change names;
    typedef std::function<coord_t(const coord_t, const coord_t)> F3D_t; // Note: no ref use here to avoid unpredictable behavior.
    typedef std::function<CPoint(const coord_t, const coord_t)> FParametric_t; // Note: no ref use here to avoid unpredictable behavior.

    CMesh SurfByFunc(const F3D_t &func, int aBoxSize, coord_t aAlpha, const std::string &arLabel, bool RemoveFlatSurfaces = true);

    CMesh ParametricSurface(const FParametric_t &func, int aNumberOfSteps, coord_t aThetaMin, coord_t aThetaMax,
                            coord_t aPhiMin, coord_t aPhiMax, coord_t aAlpha, const std::string &arLabel);
} // namespace vivid

#endif //VIVID_SURFBYFUNC_H
