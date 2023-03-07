#include "search.h"
#include "movegen.h"

#include <iostream>

using namespace std;

void Search::run(const Position& pos)
{
    MoveArray moves;
    MoveCount move_count = 0;
    MoveGenerator::generate(pos, moves, move_count);
    const auto& move = moves[0];
    const string move_str = move.to_move_str();
    
    cout << "bestmove " << move_str << endl;
}
