#ifndef POSITION_H
#define POSITION_H 1

#include "types.h"
#include "move.h"

struct Position
{
    ZobristKey Key;
    Color Turn;
    EachSquare<Square> Squares;
    EachPiece<Bitboard> Bitboards;

    Position make_move(const Move& move) const;
    Position make_move(const MoveStr& move_str) const;
};

#endif // !POSITION_H
