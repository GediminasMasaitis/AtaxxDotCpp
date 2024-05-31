#include "timer.h"
#include <chrono>

using namespace std;
using namespace chrono;

constexpr static size_t inf_time = numeric_limits<Time>::max();

void Timer::init(const bool infinite, const NodeCount nodes_min, const NodeCount nodes_max, const Time base, const Time increment)
{
    stopped = false;
    this->infinite = infinite;

    start_time = high_resolution_clock::now();
    allocated_time_min = base / 40;
    allocated_time_max = base / 5;

    this->nodes_min = nodes_min;
    this->nodes_max = nodes_max;
}

Time Timer::elapsed() const
{
    const auto now = high_resolution_clock::now();
    const auto result = duration_cast<milliseconds>(now - start_time).count();
    return result;
}

bool Timer::should_stop_min(const NodeCount nodes)
{
    if(stopped)
    {
        return true;
    }

    if (infinite)
    {
        return false;
    }

    if ((nodes_min > 0 && nodes > nodes_min) || (allocated_time_min > 0 && elapsed() >= allocated_time_min))
    {
        stopped = true;
        return true;
    }
    return false;
}

bool Timer::should_stop_max(const NodeCount nodes)
{
    if(infinite)
    {
        return false;
    }

    if(stopped || (nodes_max > 0 && nodes > nodes_max) || (allocated_time_max > 0 && elapsed() >= allocated_time_max))
    {
        stopped = true;
        return true;
    }
    return false;
}
