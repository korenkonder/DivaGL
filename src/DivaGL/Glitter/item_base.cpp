/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

namespace Glitter {
    ItemBaseX::ItemBaseX() {

    }

    ItemBaseX::~ItemBaseX() {

    }

    ItemBaseX& ItemBaseX::operator=(const ItemBaseX& item_base) {
        animation = item_base.animation;
        return *this;
    }
}
