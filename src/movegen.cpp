#include "movegen.h"
#include "attacks.h"

#include <bit>

using namespace std;

void MoveGenerator::generate(const Position& pos, MoveArray& move_array, MoveCount& move_index)
{
    auto pieces = pos.Bitboards[pos.Turn];
    auto near = pieces | north(pieces) | south(pieces);
    near |= west(near) | east(near);
    near &= pos.Bitboards[Pieces::Empty];

    while(near)
    {
        const auto to = pop_lsb(near);
        move_array[move_index] = Move(pos.Turn, 64, to);
        move_index++;
    }

    Bitboard all_far = 0;
    while (pieces)
    {
        const auto from = pop_lsb(pieces);
        auto far = Attacks.far[from];
        far &= pos.Bitboards[Pieces::Empty];
        all_far |= far;
        while (far)
        {
            const auto to = pop_lsb(far);
            move_array[move_index] = Move(pos.Turn, from, to);
            move_index++;
        }
    }

    //if(move_index == 0)
    //{
    //    move_array[move_index] = no_move;
    //    move_index++;
    //}
}
