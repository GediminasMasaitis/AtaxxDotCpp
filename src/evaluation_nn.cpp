#include "evaluation_nn.h"

#include "evaluation_nn_base.h"

#include <array>
#include <cassert>
#include <cmath>

using namespace std;

Score EvaluationNnue::evaluate(const PositionNnue& pos)
{
    int32_t score = EvaluationNnueBase::hidden_bias * 128;

    //const auto& accumulator = pos.accumulators[pos.Turn];
    //for (auto i = 0; i < EvaluationNnueBase::hidden_size; i++)
    //{
    //    const auto relu = std::max(accumulator[i], static_cast<EvaluationNnueBase::nnue_param_t>(0));
    //    score += relu * EvaluationNnueBase::hidden_weights[i];
    //}

    for (auto c = 0; c < 2; c++)
    {
        const auto& accumulator = pos.accumulators[pos.Turn ^ c];
        for (auto i = 0; i < EvaluationNnueBase::hidden_size; i++)
        {
            const auto relu = std::max(accumulator[i], static_cast<EvaluationNnueBase::nnue_param_t>(0));
            score += relu * EvaluationNnueBase::hidden_weightses[c][i];
        }
    }
    
    score /= 32;

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
