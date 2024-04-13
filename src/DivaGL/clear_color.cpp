/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "clear_color.hpp"
#include "wrap.hpp"

color4u8& clear_color = *(color4u8*)0x0000000140EC57C8;

void clear_color_set_gl() {
    vec4 color = clear_color;
    glClearColorDLL(color.x, color.y, color.z, color.w);
}
