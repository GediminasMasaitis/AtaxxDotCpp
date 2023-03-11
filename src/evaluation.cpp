#include "evaluation.h"
#include "attacks.h"

Score evaluate_color(const Position& pos, Color color)
{
    Score score = 0;
    auto pieces = pos.Bitboards[color];

    while (pieces)
    {
        const auto sq = pop_lsb(pieces);

        // MATERIAL
        score += 100;

        // UNATTACKABLE
        const Bitboard empty_near_sq = Attacks.near[sq] & pos.Bitboards[Pieces::Empty];
        if(!empty_near_sq)
        {
            score += 50;
        }
    }

    return score;
}

Score Evaluation::evaluate(const Position& pos)
{
    Score score = 0;
    score += evaluate_color(pos, pos.Turn);
    score -= evaluate_color(pos, !pos.Turn);
    return score;
}
