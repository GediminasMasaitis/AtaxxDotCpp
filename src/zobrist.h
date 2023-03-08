#ifndef ZOBRIST_H
#define ZOBRIST_H 1

#include "types.h"

class Rng
{
    uint64_t s;
public:
    constexpr Rng() : s(1070372)
    {
    }

    constexpr uint64_t rand64()
    {
        s ^= s >> 12, s ^= s << 25, s ^= s >> 27;
        return s * 2685821657736338717LL;
    }
};

struct ZobristClass
{
    ZobristKey turn;
    EachColor<EachSquare<ZobristKey>> squares;

    constexpr ZobristClass()
    {
        auto rng = Rng();
        turn = rng.rand64();
        for(int color = 0; color < Colors::Count; color++)
        {
            for(int square = 0; square < SquareCount; square++)
            {
                squares[color][square] = rng.rand64();
            }
        }
    }

    constexpr ZobristKey get_key(Color pos_turn, const EachSquare<Piece>& pieces) const
    {
        ZobristKey key = 0;

        if(pos_turn == Colors::Black)
        {
            key ^= turn;
        }

        for(int square = 0; square < SquareCount; square++)
        {
            auto piece = pieces[square];
            if(piece == Pieces::White || piece == Pieces::Black)
            {
                key ^= squares[piece][square];
            }
        }

        return key;
    }
};

static constexpr ZobristClass Zobrist;

#endif // !ZOBRIST_H
