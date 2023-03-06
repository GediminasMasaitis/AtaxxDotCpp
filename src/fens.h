#ifndef FEN_H
#define FEN_H 1

#include "position.h"

struct Fens
{
    static Position parse(const Fen& fen);
    static Fen serialize(const Position& pos);
};

#endif // !FEN_H
