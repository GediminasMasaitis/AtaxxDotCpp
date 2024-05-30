#pragma once

#include "types.h"

struct Options
{
    struct Defaults
    {
        static constexpr size_t hash = 32;
        static constexpr ThreadId threads = 1;

        Defaults() = delete;
    };

    class Min
    {
        public:
        static constexpr size_t hash = 1;
        static constexpr ThreadId threads = 1;

        Min() = delete;
    };

    class Max
    {
    public:
        static constexpr size_t hash = 64 * 1024;
        static constexpr ThreadId threads = 128;

        Max() = delete;
    };

    static inline size_t Hash = Defaults::hash;
    static inline ThreadId Threads = Defaults::threads;
    Options() = delete;
};