#ifndef SEARCH_H
#define SEARCH_H 1

#include "types.h"
#include "position.h"
#include "searchstate.h"

struct SearchResult
{
    Move move = Move();
    Score score = 0;
};

class Search
{
public:
    SearchState state;

    Score alpha_beta(ThreadState& thread_state, Position& pos, Ply depth, Ply ply, Score alpha, Score bet, bool is_pv);
    SearchResult iteratively_deepen(ThreadState& thread_state, Position& pos);
    SearchResult run(Position& pos, const SearchParameters& parameters);
};

#endif // !SEARCH_H
