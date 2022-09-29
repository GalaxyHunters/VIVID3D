#ifndef VIVID_STOPMOTIONANIMATION_H
#define VIVID_STOPMOTIONANIMATION_H

#include "Animation.h"

namespace vivid
{

/* 3DAnimation by Model Stop Motion technic, meaning Model for each time-step */
    class CStopMotionAnimation : public CAnimation {
    private:
        double mSecondsPerFrame;
    public:
        //Constructor, Copy Constructor, Destructor
        CStopMotionAnimation() {}

        CStopMotionAnimation(const vector<CModel> &arModels, double mSecondsPerFrame) : CAnimation(
                arModels), mSecondsPerFrame(mSecondsPerFrame){}

        CStopMotionAnimation(const CAnimation &arAnim, double mSecondsPerFrame) : CAnimation(arAnim), mSecondsPerFrame(mSecondsPerFrame) {}

        CStopMotionAnimation(const CModel &arModel, double mSecondsPerFrame) : CAnimation(arModel), mSecondsPerFrame(mSecondsPerFrame) {}

        CStopMotionAnimation(const CStopMotionAnimation &arStopMotionAnim) : mSecondsPerFrame(arStopMotionAnim.mSecondsPerFrame),CAnimation(arStopMotionAnim){}

        // Operator=
        inline CStopMotionAnimation& operator= (const CStopMotionAnimation& arSMA) { mSecondsPerFrame = arSMA.mSecondsPerFrame; CAnimation::operator=(arSMA); return *this; }

        void SetSecondsPerFrame(double aFrameGap){
            mSecondsPerFrame = aFrameGap;
        }

        double GetSecondsPerFrame() const {
            return mSecondsPerFrame;
        }


        //add model, add models... look at vec operations
    };


}; // namespace vivid
#endif //VIVID_STOPMOTIONANIMATION_H