#include "evaluation_nn.h"

#include "evaluation_nn_base.h"

#include <array>
#include <cmath>

using namespace std;

static constexpr Square get_index(const File file, const Rank rank)
{
    return rank * 7 + file;
}

Score EvaluationNnue::evaluate(const PositionBase& pos)
{
    const auto is_black = pos.Turn == Colors::Black;
    const auto us = is_black ? reverse_bits(pos.Bitboards[Colors::Black]) : pos.Bitboards[Colors::White];
    const auto them = is_black ? reverse_bits(pos.Bitboards[Colors::White]) : pos.Bitboards[Colors::Black];

    EvaluationNnueBase::hidden_layer_t hidden = EvaluationNnueBase::input_biases;
    for (Rank rank = 0; rank < 7; rank++)
    {
        for (File file = 0; file < 7; file++)
        {
            const auto sq = get_square(file, rank);
            const auto index = get_index(file, rank);
            
            if (us & (1ULL << sq))
            {
                EvaluationNnueBase::apply_piece_single<true>(hidden, index, false);
            }
            else if (them & (1ULL << sq))
            {
                EvaluationNnueBase::apply_piece_single<true>(hidden, index, true);
            }
        }
    }

    int32_t score = 0;
    for(auto i = 0; i < EvaluationNnueBase::hidden_size; i++)
    {
        const auto relu = std::max(hidden[i], static_cast<EvaluationNnueBase::nnue_param_t>(0));
        score += relu * EvaluationNnueBase::hidden_weights[i];
    }
    score += EvaluationNnueBase::hidden_bias * 512;
    score /= 512;

    const auto final_score = static_cast<Score>(score);
    return final_score;
}

Score EvaluationNnue::evaluate_from_pov(const PositionBase& pos, Color color)
{
    Score score = evaluate(pos);
    if (pos.Turn != color)
    {
        score = static_cast<Score>(-score);
    }
    return score;
}
