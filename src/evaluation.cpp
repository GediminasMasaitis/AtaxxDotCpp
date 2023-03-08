#include "evaluation.h"

Score Evaluation::evaluate(const Position& pos)
{
    Score score = 0;
    score += pop_count(pos.Bitboards[pos.Turn]);
    score -= pop_count(pos.Bitboards[!pos.Turn]);
    return score;
}
