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

    Score alpha_beta(PositionBase& pos, Ply depth, Ply ply, Score alpha, Score bet, bool is_pv);
    SearchResult iteratively_deepen(PositionBase& pos);
    SearchResult run(PositionBase& pos, const SearchParameters& parameters);
};

#endif // !SEARCH_H
