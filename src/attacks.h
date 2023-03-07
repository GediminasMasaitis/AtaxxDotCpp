#ifndef ATTACKS_H
#define ATTACKS_H 1

#include "types.h"

static constexpr Bitboard west(const Bitboard bb)
{
    return (bb >> 1) & 0x003F3F3F3F3F3F3FULL;
}

static constexpr Bitboard east(const Bitboard bb)
{
    return (bb << 1) & 0x007E7E7E7E7E7E7EULL;
}

static constexpr Bitboard north(const Bitboard bb)
{
    return (bb << 8) & 0x007F7F7F7F7F7F7FULL;
}

static constexpr Bitboard south(const Bitboard bb)
{
    return bb >> 8;
}

static constexpr Bitboard nw(const Bitboard bb)
{
    return north(west(bb));
}

static constexpr Bitboard ne(const Bitboard bb)
{
    return north(east(bb));
}

static constexpr Bitboard sw(const Bitboard bb)
{
    return south(west(bb));
}

static constexpr Bitboard se(const Bitboard bb)
{
    return south(east(bb));
}

class AttacksClass
{
    static constexpr Bitboard get_masked_attack(const Square square, const Bitboard jumpMask, const Square jump_mask_center)
    {
        Bitboard jumps;
        if (square > jump_mask_center)
        {
            jumps = jumpMask << (square - jump_mask_center);
        }
        else
        {
            jumps = jumpMask >> (jump_mask_center - square);
        }

        jumps &= ~(square % 8 < 4 ? Files[6] | Files[7] : Files[0] | Files[1]);
        jumps &= ~((square >> 3) < 4 ? Ranks[6] | Ranks[7] : Ranks[0] | Ranks[1]);
        return jumps;
    }

public:
    using JumpArray = EachSquare<Bitboard>;
    JumpArray near;
    JumpArray far;

    constexpr AttacksClass()
    {
        for(Square square = 0; square < SquareCount; square++)
        {
            constexpr Bitboard near_span = 0x0000000000070507ULL;
            constexpr Square near_span_center = 9ULL;
            near[square] = get_masked_attack(square, near_span, near_span_center);

            constexpr Bitboard far_span = 0x0000001F1111111F;
            constexpr Square far_span_center = 18ULL;
            far[square] = get_masked_attack(square, far_span, far_span_center);
        }
    }
};

static constexpr AttacksClass Attacks = AttacksClass();

#endif // !ATTACKS_H
