/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/
#include "glitter.hpp"

namespace Glitter {
    AnimationX::AnimationX() {
        curves.reserve(0x80);
    }

    AnimationX::~AnimationX() {
        for (CurveX*& i : curves)
            delete i;
    }

    AnimationX& AnimationX::operator=(const AnimationX& anim) {
        for (CurveX*& i : curves)
            delete i;
        curves.clear();

        curves.reserve(anim.curves.size());
        for (CurveX* i : anim.curves) {
            CurveX* curv = new CurveX;
            *curv = *i;
            curves.push_back(curv);
        }
        return *this;
    }
}
