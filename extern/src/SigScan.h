#pragma once

#include <Psapi.h>

FORCEINLINE const MODULEINFO& getModuleInfo() {
    static MODULEINFO moduleInfo;

    if (moduleInfo.SizeOfImage)
        return moduleInfo;

    ZeroMemory(&moduleInfo, sizeof(MODULEINFO));
    GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &moduleInfo, sizeof(MODULEINFO));

    return moduleInfo;
}

// Signature scan in specified memory region
FORCEINLINE void* sigScan(const char* signature, const char* mask, size_t sigSize, void* memory, const size_t memorySize) {
    if (sigSize == 0)
        sigSize = strlen(mask);

    for (size_t i = 0; i < memorySize; i++) {
        char* currMemory = (char*)memory + i;

        size_t j;
        for (j = 0; j < sigSize; j++) {
            if (mask[j] != '?' && signature[j] != currMemory[j])
                break;
        }

        if (j == sigSize)
            return currMemory;
    }

    return nullptr;
}

// Signature scan in current process
FORCEINLINE void* sigScan(const char* signature, const char* mask, void* hint) {
    const MODULEINFO& info = getModuleInfo();
    const size_t sigSize = strlen(mask);

    // Ensure hint address is within the process memory region so there are no crashes.
    if ((hint >= info.lpBaseOfDll) && ((char*)hint + sigSize <= (char*)info.lpBaseOfDll + info.SizeOfImage))     {
        void* result = sigScan(signature, mask, sigSize, hint, sigSize);

        if (result)
            return result;
    }

    return sigScan(signature, mask, sigSize, info.lpBaseOfDll, info.SizeOfImage);
}

// Signature scan string in specified memory region
FORCEINLINE void* sigScanString(const char* signature, void* memory, const size_t memorySize) {
    const size_t sigSize = strlen(signature);

    for (size_t i = 0; i < memorySize; i++) {
        char* currMemory = (char*)memory + i;

        if (!memcmp(signature, currMemory, sigSize))
            return currMemory;
    }

    return nullptr;
}

// Signature scan string in current process
FORCEINLINE void* sigScanString(const char* signature) {
    const MODULEINFO& info = getModuleInfo();
    return sigScanString(signature, info.lpBaseOfDll, info.SizeOfImage);
}

// Automatically scanned signature
#define SIG_SCAN(x, y, ...) \
    FORCEINLINE void* x(); \
    inline bool x##Valid = true; \
    inline void* x##Addr = x(); \
    FORCEINLINE void* x() { \
        constexpr const char* x##Data[] = { __VA_ARGS__ }; \
        constexpr size_t x##Size = _countof(x##Data); \
        if (!x##Addr) { \
            if constexpr (x##Size == 2) { \
                x##Addr = sigScan(x##Data[0], x##Data[1], (void*)(y)); \
                if (x##Addr) \
                    return x##Addr; \
            } \
            else \
                for (int i = 0; i < x##Size; i += 2) { \
                    x##Addr = sigScan(x##Data[i], x##Data[i + 1], (void*)(y)); \
                    if (x##Addr) \
                        return x##Addr; \
                } \
            x##Valid = false; \
        } \
        return x##Addr; \
    }

#define SIG_SCAN_STRING(x, y) \
    FORCEINLINE void* x(); \
    inline bool x##Valid = true; \
    inline void* x##Addr = x(); \
    FORCEINLINE void* x() { \
		if (!x##Addr) { \
			x##Addr = sigScanString(y); \
			if (x##Addr) \
                return x##Addr; \
			x##Valid = false; \
		} \
		return x##Addr; \
	}
