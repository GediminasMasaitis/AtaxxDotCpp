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

    NodeCount nodes_min;
    NodeCount nodes_max;

public:
    bool stopped;

    void init(bool infinite, NodeCount nodes_min, NodeCount nodes_max, Time base, Time increment);
    Time elapsed() const;
    bool should_stop_min(NodeCount nodes);
    bool should_stop_max(NodeCount nodes);
};

#endif // !TIMER_H
