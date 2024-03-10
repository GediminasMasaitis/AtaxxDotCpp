#ifndef POSITION_H
#define POSITION_H 1

#include "types.h"
#include "move.h"

#include "evaluation_nn_base.h"

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
    bool verify_bitboards() const;
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
    bool enable_accumulator_stack = false;
    EachPly<EvaluationNnueBase::hidden_layers_t> accumulators_stack;
    Ply accumulator_index = 0;

    void accumulators_push();
    void accumulators_pop();
    void accumulators_set(const Square sq, const Piece piece);
    void accumulators_unset(const Square sq, const Piece piece);
    void reset_accumulators();

    static constexpr int32_t max_history_count = 1024;
    std::array<UndoData, max_history_count> History;
    int32_t HistoryCount = 0;

    void make_move_in_place(const Move& move);
    void make_move_in_place(const MoveStr& move_str);
    void unmake_move();
};

#endif // !POSITION_H
