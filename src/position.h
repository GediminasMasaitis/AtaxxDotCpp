#ifndef POSITION_H
#define POSITION_H 1

#include "types.h"
#include "move.h"

#include <array>
#include <vector>

struct PositionBase
{
    ZobristKey Key;
    Color Turn;
    EachSquare<Square> Squares;
    EachPiece<Bitboard> Bitboards;
    Bitboard playable;

    bool is_terminal() const;
    PositionBase make_move_copy(const Move& move) const;
    PositionBase make_move_copy(const MoveStr& move_str) const;

    bool operator ==(const PositionBase& position_base) const = default;
};

struct UndoData
{
    ZobristKey key;
    Move move;
    Bitboard captured;
};

struct Position : PositionBase
{
    static constexpr int32_t max_history_count = 1024;
    std::array<UndoData, max_history_count> History;
    int32_t HistoryCount = 0;

    void make_move_in_place(const Move& move);
    void make_move_in_place(const MoveStr& move_str);
    void unmake_move();
};

#endif // !POSITION_H
