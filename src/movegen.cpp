#include "movegen.h"
#include "attacks.h"

#include <cassert>

using namespace std;

void MoveGenerator::generate_near(const PositionBase& pos, MoveArray& move_array, MoveCount& move_index)
{
    auto pieces = pos.Bitboards[pos.Turn];
    auto near = pieces | north(pieces) | south(pieces);
    near |= west(near) | east(near);
    near &= pos.Bitboards[Pieces::Empty];

    while(near)
    {
        const auto to = pop_lsb(near);
        move_array[move_index] = Move(pos.Turn, no_square, to);
        move_index++;
    }
}

void MoveGenerator::generate_far(const PositionBase& pos, MoveArray& move_array, MoveCount& move_index)
{
    auto pieces = pos.Bitboards[pos.Turn];
    while(pieces)
    {
        const auto from = pop_lsb(pieces);
        auto far = Attacks.far[from];
        far &= pos.Bitboards[Pieces::Empty];
        while(far)
        {
            const auto to = pop_lsb(far);
            move_array[move_index] = Move(pos.Turn, from, to);
            move_index++;
        }
    }
}

void MoveGenerator::generate_all(const PositionBase& pos, MoveArray& move_array, MoveCount& move_index)
{
    generate_near(pos, move_array, move_index);
    generate_far(pos, move_array, move_index);

    if(move_index == 0 && pos.Bitboards[pos.Turn])
    {
        move_array[move_index] = no_move;
        move_index++;
    }

    assert(move_index <= max_move_count);
}
