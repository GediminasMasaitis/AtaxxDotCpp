#include "fens.h"
#include "movegen.h"
#include "perft.h"

#include <iostream>

using namespace std;

int main()
{
    const Fen fen = "x5o/7/7/7/7/7/o5x x 0 1";
    const Position pos = Fens::parse(fen);
    cout << fen << endl;
    const Fen fen2 = Fens::serialize(pos);
    cout << fen2 << endl;
    
    MoveArray moves;
    MoveCount move_count = 0;
    MoveGenerator::generate(pos, moves, move_count);

    //Perft::perft_deepen(pos, 8);
    Perft::perft_suite();

    return 0;
}