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
bool aes_ni;
bool f16c;

#if defined(DINPUT8_DLL)
struct dinput8 {
    HMODULE dll;
    HRESULT(*DirectInput8Create)(HINSTANCE hinst,
        DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

    void LoadOriginalLibrary(HMODULE module) {
        dll = module;
        DirectInput8Create = (HRESULT(*)(HINSTANCE hinst, DWORD dwVersion,
            REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter))GetProcAddress(dll, "DirectInput8Create");
    }
} dinput8;

extern "C" __declspec(dllexport) HRESULT DirectInput8Create(HINSTANCE hinst,
    DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter) {
    if (!dinput8.DirectInput8Create)
        return (HRESULT)0x80000002;
    return dinput8.DirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

void load_original_dll() {
    wchar_t dll_file_path[MAX_PATH];
    GetModuleFileNameW((HMODULE)dll_handle, dll_file_path, MAX_PATH);

    wchar_t* dll_file_name = wcsrchr(dll_file_path, '\\');
    if (!dll_file_name)
        dll_file_name = wcsrchr(dll_file_path, '/');

    if (!dll_file_name)
        return;

    dll_file_name++;

    _wcslwr_s(dll_file_name, MAX_PATH - (dll_file_name - dll_file_path));

    wchar_t* sys_folder;
    if (dll_file_name && SUCCEEDED(SHGetKnownFolderPath(FOLDERID_System, 0, 0, &sys_folder))) {
        size_t dll_path_len = wcslen(sys_folder) + wcslen(dll_file_name) + 2;
        wchar_t* dll_path = (wchar_t*)malloc(sizeof(wchar_t) * dll_path_len);
        if (dll_path) {
            wcscpy_s(dll_path, dll_path_len, sys_folder);
            wcscat_s(dll_path, dll_path_len, L"\\");
            wcscat_s(dll_path, dll_path_len, dll_file_name);
            if (!wcscmp(dll_file_name, L"dinput8.dll")) {
                HMODULE dinput8_handle = LoadLibraryW(dll_path);
                if (dinput8_handle)
                    dinput8.LoadOriginalLibrary(dinput8_handle);
            }
            free(dll_path);
        }
        CoTaskMemFree((LPVOID)sys_folder);
    }
}
#endif

void FASTCALL divagl_main() {
    int32_t cpuid_data[4] = {};
    __cpuid(cpuid_data, 1);
    aes_ni = (cpuid_data[2] & (1 << 25)) ? true : false;
    f16c = (cpuid_data[2] & (1 << 29)) ? true : false;

    glut_handle = (size_t)GetModuleHandleW(L"glut32.dll");
    opengl32_handle = (size_t)GetModuleHandleW(L"opengl32.dll");

    if (memcmp((void*)(glut_handle + 0x0004CA68),
        "D:\\F\\DIVA\\DIVAAC\\package\\glut-3.7.6\\vc11\\lib\\x64\\Release\\glut32.pdb", 0x43)) {
        MessageBoxA(0, "Unsupported glut32.dll.\r\nPlease use original glut32.dll.", 0, MB_OK);
        ExitProcess(0);
        return;
    }

#if defined(DINPUT8_DLL)
    load_original_dll();
    LoadLibraryW(L"plugins\\DivaSound.dva");
    LoadLibraryW(L"plugins\\Patches.dva");
    LoadLibraryW(L"plugins\\TLAC.dva");
#endif
    //Sleep(5000);
    //return;

    wrap_addresses();

    printf_divagl("Patching memory");
    hook_funcs();
    wrap_patch();
}

bool APIENTRY DllMain(HMODULE handle, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        /*if (!GetConsoleWindow())
            AllocConsole();

        FILE* f_stdin = 0;
        FILE* f_stdout = 0;
        freopen_s(&f_stdin, "CONIN$", "r", stdin);
        freopen_s(&f_stdout, "CONOUT$", "w", stdout);*/

        dll_handle = (size_t)handle;
        printf_divagl("Current version - v0.5.2.0");
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
    return L"v0.5.21.0";
}
