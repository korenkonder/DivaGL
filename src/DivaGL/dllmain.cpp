/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include <Windows.h>
#include "../KKdLib/default.hpp"
#include "hook.hpp"
#include "print.hpp"
#include "wrap.hpp"
#include <intrin.h>
#include <knownfolders.h>
#include <shlobj_core.h>

size_t diva_handle;
size_t glut_handle;
size_t opengl32_handle;
size_t dll_handle;
bool cpu_caps_aes_ni;
bool cpu_caps_f16c;

void FASTCALL divagl_main() {
    int32_t cpuid_data[4] = {};
    __cpuid(cpuid_data, 1);
    cpu_caps_aes_ni = (cpuid_data[2] & (1 << 25)) ? true : false;
    cpu_caps_f16c = (cpuid_data[2] & (1 << 29)) ? true : false;

    glut_handle = (size_t)GetModuleHandleW(L"glut32.dll");
    opengl32_handle = (size_t)GetModuleHandleW(L"opengl32.dll");

    if (memcmp((void*)(glut_handle + 0x0004CA68),
        "D:\\F\\DIVA\\DIVAAC\\package\\glut-3.7.6\\vc11\\lib\\x64\\Release\\glut32.pdb", 0x43)) {
        printf_divagl("Error! Unsupported glut32.dll.");
        printf_divagl("Please use original glut32.dll.");
        MessageBoxA(0, "Unsupported glut32.dll.\r\nPlease use original glut32.dll.", 0, MB_OK);
        return;
    }

    wrap_addresses();

    printf_divagl("Patching memory");
    hook_funcs();
    wrap_patch();
}

bool APIENTRY DllMain(HMODULE handle, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        dll_handle = (size_t)handle;
        printf_divagl("Current version - v0.7.0.1");
        printf_divagl("Attach");
        divagl_main();
        break;
    }
    return true;
}

extern "C" __declspec(dllexport) LPCWSTR GetPluginName(void) {
    return L"DivaGL";
}

extern "C" __declspec(dllexport) LPCWSTR GetPluginDescription(void) {
    return L"DivaGL Plugin by korenkonder";
}

extern "C" __declspec(dllexport) LPCWSTR GetBuildDate(void) {
    return L"v0.7.0.1";
}
