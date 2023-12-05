#pragma once

#include "detours.h"

#define _CONCAT2(x, y) x##y
#define CONCAT2(x, y) _CONCAT(x, y)
#define INSERT_PADDING(length) \
    uint8_t CONCAT2(pad, __LINE__)[length]

#define ASSERT_OFFSETOF(type, field, offset) \
    static_assert(offsetof(type, field) == offset, "offsetof assertion failed")

#define ASSERT_SIZEOF(type, size) \
    static_assert(sizeof(type) == size, "sizeof assertion failed")

#ifdef BASE_ADDRESS
const HMODULE MODULE_HANDLE = GetModuleHandle(nullptr);

#define ASLR(address) \
    ((size_t)MODULE_HANDLE + (size_t)address - (size_t)BASE_ADDRESS)
#endif

#define FUNCTION_PTR(returnType, callingConvention, function, location, ...) \
    returnType (callingConvention *function)(__VA_ARGS__) = (returnType(callingConvention*)(__VA_ARGS__))(location)

#define FUNCTION_PTR_H(returnType, callingConvention, function, ...) \
    extern returnType (callingConvention *function) (__VA_ARGS__)

#define PROC_ADDRESS(libraryName, procName) \
    GetProcAddress(LoadLibrary(TEXT(libraryName)), procName)

#define HOOK(returnType, callingConvention, functionName, location, ...) \
    typedef returnType callingConvention _##functionName(__VA_ARGS__); \
    _##functionName* original##functionName = (_##functionName*)(location); \
    returnType callingConvention implOf##functionName(__VA_ARGS__)

#define INSTALL_HOOK(functionName) \
    { \
        DetourTransactionBegin(); \
        DetourUpdateThread(GetCurrentThread()); \
        DetourAttach((void**)&original##functionName, implOf##functionName); \
        DetourTransactionCommit(); \
    }

#define VTABLE_HOOK(returnType, callingConvention, className, functionName, ...) \
    typedef returnType callingConvention className##functionName(className* This, __VA_ARGS__); \
    className##functionName* original##className##functionName; \
    returnType callingConvention implOf##className##functionName(className* This, __VA_ARGS__)

#define INSTALL_VTABLE_HOOK(className, object, functionName, functionIndex) \
    { \
        void** addr = &(*(void***)object)[functionIndex]; \
        if (*addr != implOf##className##functionName) \
        { \
            original##className##functionName = (className##functionName*)*addr; \
            DWORD oldProtect; \
            VirtualProtect(addr, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect); \
            *addr = implOf##className##functionName; \
            VirtualProtect(addr, sizeof(void*), oldProtect, &oldProtect); \
        } \
    }

#define READ_MEMORY(location, type) \
    *(type *)location

#define WRITE_MEMORY(location, type, ...) \
    { \
        const type data[] = { __VA_ARGS__ }; \
        DWORD oldProtect; \
        VirtualProtect((void*)(location), sizeof(data), PAGE_EXECUTE_READWRITE, &oldProtect); \
        memcpy((void*)(location), data, sizeof(data)); \
        VirtualProtect((void*)(location), sizeof(data), oldProtect, &oldProtect); \
    }

#define WRITE_MEMORY_STRING(location, data, length) \
	{ \
		DWORD oldProtect; \
		VirtualProtect ((void *)(location), length, PAGE_EXECUTE_READWRITE, &oldProtect); \
		memcpy ((void *)(location), data, length); \
		VirtualProtect ((void *)(location), length, oldProtect, &oldProtect); \
	}

#define WRITE_JUMP(location, function) \
    { \
        WRITE_MEMORY(location, uint8_t, 0xE9); \
        WRITE_MEMORY(location + 1, uint32_t, (uint32_t)(function - location - 5)); \
    }
	
#define WRITE_CALL(location, function) \
    { \
        WRITE_MEMORY(location, uint8_t, 0xE8); \
        WRITE_MEMORY(location + 1, uint32_t, (uint32_t)(function - location - 5)); \
    }

#define WRITE_NOP(location, count) \
    { \
        DWORD oldProtect; \
        VirtualProtect((void*)(location), (size_t)(count), PAGE_EXECUTE_READWRITE, &oldProtect); \
        for (size_t i = 0; i < (size_t)(count); i++) \
            *((uint8_t*)(location) + i) = 0x90; \
        VirtualProtect((void*)(location), (size_t)(count), oldProtect, &oldProtect); \
    }

#define WRITE_NOP_1(location) \
    { \
        WRITE_MEMORY(location, uint8_t, 0x90); \
    }

#define WRITE_NOP_2(location) \
    { \
        WRITE_MEMORY(location, uint8_t, 0x66, 0x90); \
    }

#define WRITE_NOP_3(location) \
    { \
        WRITE_MEMORY(location, uint8_t, 0x0F, 0x1F, 0x00); \
    }

#define WRITE_NOP_4(location) \
    { \
        WRITE_MEMORY(location, uint8_t, 0x0F, 0x1F, 0x40, 0x00); \
    }

#define WRITE_NOP_5(location) \
    { \
        WRITE_MEMORY(location, uint8_t, 0x0F, 0x1F, 0x44, 0x00, 0x00); \
    }

#define WRITE_NOP_6(location) \
    { \
        WRITE_MEMORY(location, uint8_t, 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00); \
    }

#define WRITE_NOP_7(location) \
    { \
        WRITE_MEMORY(location, uint8_t, 0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00); \
    }

#define WRITE_NOP_8(location) \
    { \
        WRITE_MEMORY(location, uint8_t, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00); \
    }

#define WRITE_NOP_9(location) \
    { \
        WRITE_MEMORY(location, uint8_t, 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00); \
    }

#define WRITE_NOP_10(location) \
    { \
        WRITE_MEMORY(location, uint8_t, 0x66, 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00); \
    }

#define WRITE_NOP_11(location) \
    { \
        WRITE_MEMORY(location, uint8_t, 0x66, 0x66, 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00); \
    }

#define WRITE_NULL(location, count) \
    { \
        DWORD oldProtect; \
        VirtualProtect((void*)(location), (size_t)(count), PAGE_EXECUTE_READWRITE, &oldProtect); \
        for (size_t i = 0; i < (size_t)(count); i++) \
            *((uint8_t*)(location) + i) = 0x00; \
        VirtualProtect((void*)(location), (size_t)(count), oldProtect, &oldProtect); \
    }

#define COUNTOFARR(arr) \
    sizeof(arr) / sizeof(arr[0])
