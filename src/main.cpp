#include "fens.h"

#include <iostream>

using namespace std;

int main()
{
    const Fen fen = "x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1";
    const Position pos = Fens::parse(fen);
    const Fen fen2 = Fens::serialize(pos);

    cout << fen << endl;
    cout << fen2 << endl;

    return 0;
}