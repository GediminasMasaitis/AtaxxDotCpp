#include "position.h"

#include "attacks.h"
#include "zobrist.h"

#include <cassert>

PositionBase PositionBase::make_move_copy(const Move& move) const
{
    assert(move != no_move);
    assert(move.Turn == Turn);

    PositionBase new_pos = *this;
    new_pos.Turn = !new_pos.Turn;
    new_pos.Key ^= Zobrist.turn;

    if (move == passes[move.Turn])
    {
        return new_pos;
    }

    // TO
    assert(move.To != no_square);
    assert(new_pos.Squares[move.To] == Pieces::Empty);
    new_pos.Squares[move.To] = move.Turn;
    new_pos.Bitboards[move.Turn] |= get_bitboard(move.To);
    new_pos.Bitboards[Pieces::Empty] &= ~get_bitboard(move.To);
    new_pos.Key ^= Zobrist.squares[move.Turn][move.To];

    // FROM
    if (move.From != no_square)
    {
        assert(move.From != move.To);
        assert(new_pos.Squares[move.From] == move.Turn);
        new_pos.Squares[move.From] = Pieces::Empty;
        new_pos.Bitboards[move.Turn] &= ~get_bitboard(move.From);
        new_pos.Bitboards[Pieces::Empty] |= get_bitboard(move.From);
        new_pos.Key ^= Zobrist.squares[move.Turn][move.From];
    }

    // CAPTURE
    auto attacked = Attacks.near[move.To] & new_pos.Bitboards[!move.Turn];
    new_pos.Bitboards[move.Turn] |= attacked;
    new_pos.Bitboards[!move.Turn] &= ~attacked;
    while (attacked)
    {
        const auto attacked_square = pop_lsb(attacked);
        new_pos.Squares[attacked_square] = move.Turn;
        new_pos.Key ^= Zobrist.squares[move.Turn][attacked_square];
        new_pos.Key ^= Zobrist.squares[!move.Turn][attacked_square];
    }

    return new_pos;
}

PositionBase PositionBase::make_move_copy(const MoveStr& move_str) const
{
    const Move move = Move::from_move_str(Turn, move_str);
    return make_move_copy(move);
}

void Position::make_move_in_place(const Move& move)
{
    assert(move != no_move);
    assert(move.Turn == Turn);

    auto attacked = Attacks.near[move.To] & Bitboards[!move.Turn];
    History[HistoryCount] = { Key, move, attacked };
    HistoryCount++;

    Turn = !Turn;
    Key ^= Zobrist.turn;

    if(move == passes[move.Turn])
    {
        return;
    }

    // TO
    assert(move.To != no_square);
    assert(Squares[move.To] == Pieces::Empty);
    Squares[move.To] = move.Turn;
    Bitboards[move.Turn] |= get_bitboard(move.To);
    Bitboards[Pieces::Empty] &= ~get_bitboard(move.To);
    Key ^= Zobrist.squares[move.Turn][move.To];

    // FROM
    if(move.From != no_square)
    {
        assert(move.From != move.To);
        assert(Squares[move.From] == move.Turn);
        Squares[move.From] = Pieces::Empty;
        Bitboards[move.Turn] &= ~get_bitboard(move.From);
        Bitboards[Pieces::Empty] |= get_bitboard(move.From);
        Key ^= Zobrist.squares[move.Turn][move.From];
    }

    // CAPTURE
    Bitboards[move.Turn] |= attacked;
    Bitboards[!move.Turn] &= ~attacked;
    while (attacked)
    {
        const auto attacked_square = pop_lsb(attacked);
        Squares[attacked_square] = move.Turn;
        Key ^= Zobrist.squares[move.Turn][attacked_square];
        Key ^= Zobrist.squares[!move.Turn][attacked_square];
    }
}

void Position::make_move_in_place(const MoveStr& move_str)
{
    const Move move = Move::from_move_str(Turn, move_str);
    make_move_in_place(move);
}

void Position::unmake_move()
{
    HistoryCount--;
    UndoData& undo_data = History[HistoryCount];

    const Move& move = undo_data.move;
    assert(move != no_move);

    Key = undo_data.key;
    Turn = !Turn;
    assert(move.Turn == Turn);
    if(move == passes[move.Turn])
    {
        return;
    }

    // TO
    assert(move.To != no_square);
    assert(Squares[move.To] == move.Turn);
    Squares[move.To] = Pieces::Empty;
    Bitboards[move.Turn] &= ~get_bitboard(move.To);
    Bitboards[Pieces::Empty] |= get_bitboard(move.To);

    // FROM
    if(undo_data.move.From != no_square)
    {
        assert(move.From != move.To);
        assert(Squares[move.From] == Pieces::Empty);
        Squares[move.From] = Turn;
        Bitboards[Turn] |= get_bitboard(move.From);
        Bitboards[Pieces::Empty] &= ~get_bitboard(move.From);
    }

    // CAPTURE
    Bitboards[move.Turn] &= ~undo_data.captured;
    Bitboards[!move.Turn] |= undo_data.captured;
    while (undo_data.captured)
    {
        const auto attacked_square = pop_lsb(undo_data.captured);
        Squares[attacked_square] = !move.Turn;
    }
}
