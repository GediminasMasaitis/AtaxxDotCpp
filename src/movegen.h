#ifndef MOVEGEN_H
#define MOVEGEN_H 1

#include "types.h"
#include "position.h"

struct MoveGenerator
{
    static void generate_near(const PositionBase& pos, MoveArray& move_array, MoveCount& move_index);
    static void generate_far(const PositionBase& pos, MoveArray& move_array, MoveCount& move_index);
    static void generate_all(const PositionBase& pos, MoveArray& move_array, MoveCount& move_index);
};

#endif // !MOVEGEN_H
