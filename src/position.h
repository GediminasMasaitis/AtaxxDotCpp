#ifndef POSITION_H
#define POSITION_H 1

#include "types.h"

struct Position
{
    Color Turn;
    EachSquare<Square> Squares;
    EachPiece<Bitboard> Bitboards;
};

#endif // !POSITION_H
