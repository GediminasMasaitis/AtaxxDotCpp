#include "position.h"

#include "attacks.h"
#include "zobrist.h"

#include <cassert>

Position Position::make_move(const Move& move) const
{
    Position new_pos = *this;
    new_pos.Turn = !new_pos.Turn;
    new_pos.Key ^= Zobrist.turn;

    if(move == no_move)
    {
        return new_pos;
    }

    // TO
    assert(move.Turn == this->Turn);
    assert(move.To != no_square);
    assert(new_pos.Squares[move.To] == Pieces::Empty);
    new_pos.Squares[move.To] = move.Turn;
    new_pos.Bitboards[move.Turn] |= get_bitboard(move.To);
    new_pos.Bitboards[Pieces::Empty] &= ~get_bitboard(move.To);
    new_pos.Key ^= Zobrist.squares[move.Turn][move.To];

    // FROM
    if(move.From != no_square)
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

Position Position::make_move(const MoveStr& move_str) const
{
    const Move move = Move::from_move_str(Turn, move_str);
    return make_move(move);
}
