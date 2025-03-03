/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "sprite.hpp"
#include <stdio.h>
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

bool APIENTRY DllMain(HMODULE handle, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        printf("[Sprite Draw Limit] Current version - v0.1.0.0\n");
        printf("[Sprite Draw Limit] Attach\n");
        printf("[Sprite Draw Limit] Patching memory\n");
        sprite_draw_limit_patch();
        break;
    }
    return true;
}

extern "C" __declspec(dllexport) LPCWSTR GetPluginName(void) {
    return L"Sprite Draw Limit";
}

extern "C" __declspec(dllexport) LPCWSTR GetPluginDescription(void) {
    return L"Sprite Draw Limit Plugin by korenkonder";
}

extern "C" __declspec(dllexport) LPCWSTR GetBuildDate(void) {
    return L"v0.1.0.0";
}
