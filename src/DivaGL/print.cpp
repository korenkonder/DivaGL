/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "print.hpp"

void printf_divagl(const char* fmt, ...) {
    printf("[DivaGL] ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}

void printf_proxy(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
