#ifndef MOVE_H
#define MOVE_H 1

#include "types.h"

struct Move
{
    Color Turn;
    Square From;
    Square To;

    constexpr Move(const Color turn, const Square from, const Square to)
        : Turn(turn), From(from), To(to)
    {
    }

    constexpr Move() : Move(Colors::White, 0, 0)
    {
    }

    MoveStr to_move_str() const;
    static Move from_move_str(const Color color, const MoveStr& move_str);

    bool operator==(const Move& move) const = default;
};

static constexpr Move no_move = Move(Colors::White, 0, 0);

using MoveCount = uint16_t;
constexpr MoveCount max_move_count = 1024;
template<class T>
using EachMove = std::array<T, max_move_count>;
using MoveArray = EachMove<Move>;
using MoveScoreArray = EachMove<MoveScore>;

#endif // !MOVE_H
