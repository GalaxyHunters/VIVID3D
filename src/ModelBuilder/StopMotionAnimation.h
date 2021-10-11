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
    //Constructor, Copy Constructor, Destructor
    CStopMotionAnimation(){}
    CStopMotionAnimation(const std::vector<CModel> &arModels, const std::string &arLabel, const duration_t &arDuration) :
        mModels(arModels), CAnimation(arLabel, arDuration){}
    CStopMotionAnimation(const CStopMotionAnimation &arSMA) : mModels(arSMA.mModels), CAnimation(arSMA) {}
    ~CStopMotionAnimation(){}

    // Operator=
    inline CStopMotionAnimation& operator= (const CStopMotionAnimation& arSMA) { mModels = arSMA.mModels; CAnimation::operator=(arSMA); return *this; }

    // Set and Get
    inline std::vector<CModel> GetModels() const{ return mModels; }
    inline void SetModels(const std::vector<CModel> &arModels) { mModels = arModels; }

    // Add
    inline void AddModels(const std::vector<CModel> &arModels) { mModels.insert(mModels.end(), arModels.begin(), arModels.end()); }

    //add model, add models... look at vec operations
};


}; // namespace vivid
#endif //VIVID_STOPMOTIONANIMATION_H
