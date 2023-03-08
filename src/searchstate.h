#ifndef SEARCHSTATE_H
#define SEARCHSTATE_H 1

#include "types.h"
#include "timer.h"

struct SearchParameters
{
    bool infinite = false;
    Time white_time = 10000;
    Time white_increment = 100;
    Time black_time = 10000;
    Time black_increment = 100;
};

struct PrincipalVariationData
{
    Ply length;
    EachPly<Move> moves;
};

struct PlyState
{
    PrincipalVariationData principal_variation;
};

struct SearchState
{
    Timer timer;
    SearchParameters parameters;
    uint64_t nodes;

    EachPly<PlyState> plies;
};

#endif // !SEARCHSTATE_H
