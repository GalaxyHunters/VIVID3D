#ifndef VIVID_LINE_H
#define VIVID_LINE_H

#include "ModelComponent.h"

#include <vector>
#include "Point.h"


namespace vivid
{

/* 3D Line class */
class CLine : public CModelComponent{
    /* mPoints - Points in which the line pass through */
    std::vector<CPoint> mPoints = {};

    // Set and Get
};

}; // namespace vivid
#endif //VIVID_LINE_H
