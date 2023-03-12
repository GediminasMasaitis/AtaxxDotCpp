#ifndef TIMER_H
#define TIMER_H 1

#include "types.h"
#include <chrono>

class Timer
{
    bool infinite;
    std::chrono::high_resolution_clock::time_point start_time;
    Time allocated_time_min;
    Time allocated_time_max;

public:
    bool stopped;

    void init(const bool infinite, const Time base, const Time increment);
    Time elapsed() const;
    bool should_stop_min();
    bool should_stop_max();
};

#endif // !TIMER_H
