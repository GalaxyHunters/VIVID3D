#ifndef VIVID_SURFBYFUNC_H
#define VIVID_SURFBYFUNC_H

#include "Mesh.h"
#include "ColorMap.h"

namespace vivid
{
    /* FTrans_t is a function that changes CPoint to another CPoint */
    typedef std::function<quan_t(const quan_t, const quan_t)> F3D_t; // Note: no ref use here to avoid unpredictable behavior.
    typedef std::function<CPoint(const quan_t, const quan_t)> FParametric_t; // Note: no ref use here to avoid unpredictable behavior.

    CMesh SurfByFunc(const F3D_t &func, int aBoxSize, quan_t aAlpha, const std::string &arLabel, bool RemoveFlatSurfaces = true);

    CMesh ParametricSurface(const FParametric_t &func, int aNumberOfSteps, quan_t aThetaMin, quan_t aThetaMax,
                            quan_t aPhiMin, quan_t aPhiMax, quan_t aAlpha, const std::string &arLabel);
} // namespace vivid

#endif //VIVID_SURFBYFUNC_H
