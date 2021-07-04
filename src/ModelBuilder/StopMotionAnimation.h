#ifndef VIVID_STOPMOTIONANIMATION_H
#define VIVID_STOPMOTIONANIMATION_H

#include "Animation.h"
#include "Model.h"

namespace vivid
{

/* 3DAnimation by Model Stop Motion technic, meaning Model for each time-step */
class CStopMotionAnimation : public CAnimation{
public:
    std::vector<CModel> mModels = {};

    //add model, add models... look at vec operations
    //get, set
};


}; // namespace vivid
#endif //VIVID_STOPMOTIONANIMATION_H
