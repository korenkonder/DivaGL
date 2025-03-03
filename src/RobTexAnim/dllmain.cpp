/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "rob/rob.hpp"
#include <stdio.h>
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

bool APIENTRY DllMain(HMODULE handle, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        printf("[Rob Tex Anim] Current version - v0.1.0.0\n");
        printf("[Rob Tex Anim] Attach\n");
        printf("[Rob Tex Anim] Patching memory\n");
        rob_patch();
        break;
    }
    return true;
}

extern "C" __declspec(dllexport) LPCWSTR GetPluginName(void) {
    return L"Rob Tex Anim";
}

extern "C" __declspec(dllexport) LPCWSTR GetPluginDescription(void) {
    return L"Rob Tex Anim Plugin by korenkonder";
}

extern "C" __declspec(dllexport) LPCWSTR GetBuildDate(void) {
    return L"v0.1.0.0";
}
