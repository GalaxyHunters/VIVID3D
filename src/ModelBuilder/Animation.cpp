#include "Animation.h"

using namespace vivid;

CAnimation::~CAnimation() {}

int CAnimation::GetTicksPerSecond() const {
    return mTicksPerSecond;
}

void CAnimation::SetTicksPerSecond(int mTicksPerSecond) {
    CAnimation::mTicksPerSecond = mTicksPerSecond;
}
