#ifndef FEN_H
#define FEN_H 1

#include "position.h"

#include <sstream>
#include <string>

struct Fens
{
    static Position parse(const Fen& fen);
    static void serialize(const PositionBase& pos, std::ostream& ss);
    static Fen serialize(const PositionBase& pos);
};

#endif // !FEN_H
