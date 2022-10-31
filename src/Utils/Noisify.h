#ifndef VIVID_NOISIFY_H
#define VIVID_NOISIFY_H

#include "Point.h"

namespace vivid {
    std::vector<CPoint> NoisifyPoints(const std::vector<CPoint>& arPoints, coord_t aDistance);
}

#endif //VIVID_NOISIFY_H
