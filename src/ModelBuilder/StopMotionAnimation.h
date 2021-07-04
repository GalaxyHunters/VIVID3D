#ifndef VIVID_STOPMOTIONANIMATION_H
#define VIVID_STOPMOTIONANIMATION_H

#include "Animation.h"
#include "Model.h"

namespace vivid
{

/* 3DAnimation by Model Stop Motion technic, meaning Model for each time-step */
class CStopMotionAnimation : public CAnimation{
private:
    std::vector<CModel> mModels = {};
public:
    //Constructor and Copy Constructor
    CStopMotionAnimation(){}
    CStopMotionAnimation(std::vector<CModel> &aModels) : mModels(aModels){}
    CStopMotionAnimation(const CStopMotionAnimation &aSMA) : mModels(aSMA.mModels) {}

    // Set and Get
    inline const std::vector<CModel> Models() const{ return mModels; }
    inline void Models(std::vector<CModel> &aModels) { mModels = aModels; }

    // Add
    inline void AddModels(std::vector<CModel> &aModels) { mModels.insert(mModels.end(), aModels.begin(), aModels.end()); }

    //add model, add models... look at vec operations
};


}; // namespace vivid
#endif //VIVID_STOPMOTIONANIMATION_H
