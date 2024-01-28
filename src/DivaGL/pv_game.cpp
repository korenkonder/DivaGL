/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "pv_game.hpp"
#include <Helpers.h>

void pv_game_load_state_6_tail_impl(size_t pv_game) {
    *(uint32_t*)(pv_game + 0x08) = 7;
}

void pv_game_load_state_8_tail_impl(size_t pv_game) {
    *(uint32_t*)(pv_game + 0x08) = 9;
}

bool pv_game_load_state_9_mid_impl() {
    return false;
}

void pv_game_load_state_13_tail_impl(size_t pv_game) {
    *(uint32_t*)(pv_game + 0x08) = 14;
}

HOOK(void, FASTCALL, pv_game_load_state_6_tail, 0x00000001401011CE);
HOOK(void, FASTCALL, pv_game_load_state_8_tail, 0x00000001401024E1);
HOOK(void, FASTCALL, pv_game_load_state_9_mid, 0x000000014010280D);
HOOK(void, FASTCALL, pv_game_load_state_13_tail, 0x000000014010386E);

void pv_game_patch() {
    INSTALL_HOOK(pv_game_load_state_6_tail);
    INSTALL_HOOK(pv_game_load_state_8_tail);
    INSTALL_HOOK(pv_game_load_state_9_mid);
    INSTALL_HOOK(pv_game_load_state_13_tail);

    WRITE_MEMORY_STRING(0x00000001401027D8, "\x41\x0F\x45\xDF\x75\x2F", 0x06);
}
