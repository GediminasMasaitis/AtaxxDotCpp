#ifndef SEARCH_H
#define SEARCH_H 1

#include "types.h"
#include "position.h"
#include "searchstate.h"

class Search
{
public:
    SearchState state;

    Score alpha_beta(const Position& pos, Ply depth, Ply ply, Score alpha, Score bet);
    void iteratively_deepen(const Position& pos);
    void run(const Position& pos, const SearchParameters& parameters);
};

#endif // !SEARCH_H
