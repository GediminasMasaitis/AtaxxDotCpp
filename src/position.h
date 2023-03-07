#ifndef POSITION_H
#define POSITION_H 1

#include "types.h"

struct Position
{
    Color Turn;
    EachSquare<Square> Squares;
    EachPiece<Bitboard> Bitboards;

    Position make_move(const Move& move) const;
};

#endif // !POSITION_H
