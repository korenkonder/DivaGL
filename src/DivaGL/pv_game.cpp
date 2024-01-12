/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "pv_game.hpp"
#include <Helpers.h>

void pv_game_load_state_13_tail_impl(size_t pv_game) {
    *(uint32_t*)(pv_game + 0x08) = 14;
}

HOOK(void, FASTCALL, pv_game_load_state_13_tail, 0x000000014010386E);

void pv_game_patch() {
    INSTALL_HOOK(pv_game_load_state_13_tail);
}
