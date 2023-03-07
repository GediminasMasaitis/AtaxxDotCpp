#ifndef SEARCH_H
#define SEARCH_H 1

#include "types.h"
#include "position.h"

class SearchState
{
public:
    
};

class Search
{
public:
    SearchState state;
    void run(const Position& pos);
};

#endif // !SEARCH_H
