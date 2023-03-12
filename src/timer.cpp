#include "timer.h"
#include <chrono>

using namespace std;
using namespace chrono;

constexpr static size_t inf_time = numeric_limits<Time>::max();

void Timer::init(const bool infinite, const Time base, const Time increment)
{
    stopped = false;
    this->infinite = infinite;

    start_time = high_resolution_clock::now();
    allocated_time_min = base / 40;
    allocated_time_max = base / 5;
}

Time Timer::elapsed() const
{
    const auto now = high_resolution_clock::now();
    return duration_cast<milliseconds>(now - start_time).count();
}

bool Timer::should_stop_min()
{
    if (infinite)
    {
        return false;
    }

    const auto elapsed_time = elapsed();
    if (stopped || elapsed_time >= allocated_time_min)
    {
        stopped = true;
        return true;
    }
    return false;
}

bool Timer::should_stop_max()
{
    if(infinite)
    {
        return false;
    }

    const auto elapsed_time = elapsed();
    if(stopped || elapsed_time >= allocated_time_max)
    {
        stopped = true;
        return true;
    }
    return false;
}
