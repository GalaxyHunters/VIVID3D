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

    //get, set

};

}; // namespace vivid
#endif //VIVID_ANIMATION_H
