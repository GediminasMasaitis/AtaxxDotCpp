#ifndef MOVEGEN_H
#define MOVEGEN_H 1

#include "types.h"
#include "position.h"

struct MoveGenerator
{
    static void generate(const PositionBase& pos, MoveArray& move_array, MoveCount& move_index);
};

#endif // !MOVEGEN_H
