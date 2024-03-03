#include "evaluation_nn.h"

#include "evaluation_nn_base.h"

#include <array>
#include <cassert>
#include <cmath>

using namespace std;

Score EvaluationNnue::evaluate(const PositionNnue& pos)
{
    int32_t score = EvaluationNnueBase::hidden_bias * 512;
    const auto& hidden = pos.accumulators[pos.Turn];
    for(auto i = 0; i < EvaluationNnueBase::hidden_size; i++)
    {
        const auto relu = std::max(hidden[i], static_cast<EvaluationNnueBase::nnue_param_t>(0));
        score += relu * EvaluationNnueBase::hidden_weights[i];
    }
    score /= 512;

    const auto final_score = static_cast<Score>(score);
    return final_score;
}

Score EvaluationNnue::evaluate_from_pov(const PositionNnue& pos, Color color)
{
    Score score = evaluate(pos);
    if (pos.Turn != color)
    {
        score = static_cast<Score>(-score);
    }
    return score;
}
