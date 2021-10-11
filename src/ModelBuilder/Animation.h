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
protected:
    //Constructor, Copy Constructor, Destructor
    CAnimation(){}
    CAnimation(const std::string &arLabel, const duration_t &arDuration) : mLabel(arLabel), mDuration(arDuration){}
    CAnimation(const CAnimation &arAnim) : mLabel(arAnim.mLabel), mDuration(arAnim.mDuration) {}
    virtual ~CAnimation() = 0;

    // Operator=
    inline CAnimation& operator= (const CAnimation& arAnim) { mLabel = arAnim.mLabel; mDuration = arAnim.mDuration; return *this; }

    // Set and Get
    inline const std::string GetLabel() const{ return mLabel; }
    inline void SetLabel(const std::string &arLabel) { mLabel = arLabel; }

    inline const duration_t GetDuration() const{ return mDuration; }
    inline void SetDuration(const duration_t &arDuration) { mDuration = arDuration; }

};

}; // namespace vivid
#endif //VIVID_ANIMATION_H
