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
    allocated_time = base / 30;
}

Time Timer::elapsed() const
{
    const auto now = high_resolution_clock::now();
    return duration_cast<milliseconds>(now - start_time).count();
}

bool Timer::should_stop()
{
    if(infinite)
    {
        return false;
    }

    const auto elapsed_time = elapsed();
    if(stopped || elapsed_time >= allocated_time)
    {
        stopped = true;
        return true;
    }
    return false;
}
