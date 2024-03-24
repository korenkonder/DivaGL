/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "glitter.hpp"

namespace Glitter {
    NodeX::NodeX() : translation(), rotation() {
        scale = 1.0f;
        scale_all = 1.0f;
    }

    NodeX::~NodeX() {

    }

    NodeX& NodeX::operator=(const NodeX& node) {
        translation = node.translation;
        rotation = node.rotation;
        scale = node.scale;
        scale_all = node.scale_all;
        return *this;
    }
}
