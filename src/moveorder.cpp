#include "moveorder.h"
#include "attacks.h"

void MoveOrder::calculate_move_scores(const Position& position, const SearchState& state, const Move& tt_move, const MoveArray& moves, MoveScoreArray& move_scores, const MoveCount move_count)
{
    for(int i = 0; i < move_count; i++)
    {
        MoveScore move_score;
        const Move& move = moves[i];
        if(move == tt_move)
        {
            move_score = 1LL << 60;
        }
        else
        {
            const Bitboard captures = Attacks.near[move.To] & position.Bitboards[!position.Turn];
            move_score = 1LL << (50 + pop_count(captures));
            move_score += state.history[move.From][move.To];
        }
        move_scores[i] = move_score;
    }
}

void MoveOrder::order_next_move(MoveArray& moves, MoveScoreArray& move_scores, const MoveCount move_count, MoveCount current_index)
{
    MoveScore bestScore = std::numeric_limits<MoveScore>::min();
    MoveCount bestScoreIndex = 0;

    for (MoveCount i = current_index; i < move_count; i++)
    {
        const MoveScore staticScore = move_scores[i];
        const MoveScore score = staticScore;

        if (score > bestScore)
        {
            bestScore = score;
            bestScoreIndex = i;
        }
    }

    const Move tempMove = moves[current_index];
    moves[current_index] = moves[bestScoreIndex];
    moves[bestScoreIndex] = tempMove;

    const MoveScore tempScore = move_scores[current_index];
    move_scores[current_index] = move_scores[bestScoreIndex];
    move_scores[bestScoreIndex] = tempScore;
}
