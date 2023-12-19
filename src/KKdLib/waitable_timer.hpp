/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "default.hpp"

struct waitable_timer {
    HANDLE handle;

    inline waitable_timer() {
        handle = CreateWaitableTimerW(0, 0, 0);
    }

    inline ~waitable_timer() {
        if (handle) {
            CloseHandle(handle);
            handle = 0;
        }
    }

    inline void sleep(int64_t msec) {
        if (msec <= 0.0)
            return;

        if (handle) {
            LARGE_INTEGER t;
            t.QuadPart = (LONGLONG)(msec * -10000);
            SetWaitableTimer(handle, &t, 0, 0, 0, 0);
            WaitForSingleObject(handle, INFINITE);
        }
        else {
            DWORD msec_dw = (DWORD)msec;
            if (msec_dw)
                Sleep(msec_dw);
        }
    }

    inline void sleep_float(double_t msec) {
        if (msec <= 0.0)
            return;

        if (handle) {
            LARGE_INTEGER t;
            t.QuadPart = (LONGLONG)round(msec * -10000.0);
            SetWaitableTimer(handle, &t, 0, 0, 0, 0);
            WaitForSingleObject(handle, INFINITE);
        }
        else {
            DWORD msec_dw = (DWORD)round(msec);
            if (msec_dw)
                Sleep(msec_dw);
        }
    }
};
