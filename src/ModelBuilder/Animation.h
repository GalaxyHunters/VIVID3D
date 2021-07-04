#ifndef VIVID_ANIMATION_H
#define VIVID_ANIMATION_H

#include <string>


namespace vivid
{

typedef double duration_t;

/* Abstract Class defining 3DAnimation Models */
class CAnimation {
private:
    std::string mLabel = "";
    duration_t mDuration = 0;
    // Camera route (location, angel etc)
public:
    //Constructor and Copy Constructor
    CAnimation(){}
    CAnimation(std::string aLabel, duration_t &aDuration) : mLabel(aLabel), mDuration(aDuration){}
    CAnimation(const CAnimation &aAnim) : mLabel(aAnim.mLabel), mDuration(aAnim.mDuration) {}

    // Set and Get
    inline const std::string Label() const{ return mLabel; }
    inline void Label(std::string &aLabel) { mLabel = aLabel; }

    inline const duration_t Duration() const{ return mDuration; }
    inline void Duration(duration_t &aDuration) { mDuration = aDuration; }

};

}; // namespace vivid
#endif //VIVID_ANIMATION_H
