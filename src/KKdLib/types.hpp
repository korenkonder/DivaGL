/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include <float.h>
#include <stdint.h>
#include <wchar.h>

#define FASTCALL __fastcall
#define ALIGN(n) __declspec(align(n))

#ifdef ssize_t
#undef ssize_t
#endif
#ifdef _WIN64
typedef __int64 ssize_t;
#else
typedef int size_t;
#endif

#ifdef float_t
#undef float_t
#endif
typedef float float_t;

#ifdef double_t
#undef double_t
#endif
typedef double double_t;
