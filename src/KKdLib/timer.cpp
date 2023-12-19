/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "timer.hpp"

timer::timer(double_t freq) : history(), curr_time(), prev_time(), inv_freq() {
    for (double_t& i : history)
        i = freq;
    history_counter = 0;
    this->freq = freq;
    this->freq_hist = freq;

    LARGE_INTEGER frequency;
    if (QueryPerformanceFrequency(&frequency))
        inv_freq = 1000.0 / (double_t)frequency.LowPart;
    else
        inv_freq = 0.0;

    if (!QueryPerformanceCounter(&curr_time))
        curr_time.QuadPart = 0;
    if (!QueryPerformanceCounter(&prev_time))
        prev_time.QuadPart = 0;
}

timer::~timer() {

}

void timer::start_of_cycle() {
    LARGE_INTEGER timestamp;
    if (!QueryPerformanceCounter(&timestamp))
        timestamp.QuadPart = 0;

    double_t time = (timestamp.QuadPart - curr_time.QuadPart) * inv_freq;

    if (!QueryPerformanceCounter(&curr_time))
        curr_time.QuadPart = 0;

    history[history_counter] = time;

    double_t freq = 0.0;
    for (uint8_t i = 0; i < history_counter; i++)
        freq += history[i];
    freq += time;
    for (uint8_t i = history_counter + 1; i < HISTORY_COUNT; i++)
        freq += history[i];

    history_counter++;
    if (history_counter >= HISTORY_COUNT)
        history_counter = 0;

    std::unique_lock<std::mutex> u_lock(freq_mtx);
    freq_hist = 1000.0 / (freq / HISTORY_COUNT);
}

void timer::end_of_cycle() {
    LONGLONG& curr_timestamp = curr_time.QuadPart;
    LONGLONG& prev_timestamp = prev_time.QuadPart;

    double_t msec = 1000.0 / get_freq() - (curr_timestamp - prev_timestamp) * inv_freq;
    if (floor(msec) > 0)
        wait_timer.sleep_float(msec);

    if (msec < -get_freq() / 4.0)
        prev_timestamp = curr_timestamp - (LONGLONG)(get_freq() / 4.0 / inv_freq);
    else
        prev_timestamp = curr_timestamp + (LONGLONG)(msec / inv_freq);
}

double_t timer::get_freq() {
    std::unique_lock<std::mutex> u_lock(freq_mtx);
    return freq;
}

void timer::set_freq(double_t freq) {
    std::unique_lock<std::mutex> u_lock(freq_mtx);
    this->freq = freq;
}

double_t timer::get_freq_hist() {
    std::unique_lock<std::mutex> u_lock(freq_hist_mtx);
    return freq_hist;
}

double_t timer::get_freq_ratio() {
    double_t freq_ratio = 0.0;
    {
        std::unique_lock<std::mutex> u_lock(freq_mtx);
        freq_ratio = freq;
    }

    {
        std::unique_lock<std::mutex> u_lock(freq_hist_mtx);
        freq_ratio /= freq_hist;
    }
    return freq_ratio;
}

void timer::reset() {
    if (!QueryPerformanceCounter(&curr_time))
        curr_time.QuadPart = 0;
    if (!QueryPerformanceCounter(&prev_time))
        prev_time.QuadPart = 0;
}

void timer::sleep(double_t msec) {
    wait_timer.sleep_float(msec);
}
