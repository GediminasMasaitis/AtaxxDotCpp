#include "evaluation.h"
#include "attacks.h"

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void EvaluationHce::init()
{
    // Nothing to do
}

Score evaluate_color(const PositionBase& pos, Color color)
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

Score EvaluationHce::evaluate(const PositionBase& pos)
{
    Score score = 0;

    // TEMPO
    score += 400;
    
    score += evaluate_color(pos, pos.Turn);
    score -= evaluate_color(pos, !pos.Turn);
    return score;
}

Score EvaluationHce::evaluate_from_pov(const PositionBase& pos, Color color)
{
    Score score = evaluate(pos);
    if(pos.Turn != color)
    {
        score = static_cast<Score>(-score);
    }
    return score;
}

using nnue_param_t = float;
static std::array<nnue_param_t, 98> weights;
static nnue_param_t bias;

static constexpr Square get_index(const File file, const Rank rank)
{
    return rank * 7 + file;
}

void EvaluationNnue::init()
{
#ifdef _WIN32
    constexpr auto path = "C:/shared/ataxx/nets/current.bin";
#else
    constexpr auto path = "/mnt/c/shared/ataxx/nets/current.bin";
#endif
    auto file = ifstream(path, ios::binary);
    if(!file.good())
    {
        cout << "Failed to open " << path << endl;
        return;
    }

    auto ss = stringstream();
    ss << "Reading NNUE from " << path << endl;
    ss << "Weights: ";
    for(auto i = 0; i < weights.size(); i++)
    {
        file.read(reinterpret_cast<char*>(&weights[i]), sizeof(nnue_param_t));
        ss << weights[i] << " ";
    }
    ss << endl;
    file.read(reinterpret_cast<char*>(&bias), sizeof(nnue_param_t));
    ss << "Bias: " << bias << endl;
    const auto str = ss.str();
    //cout << str;
}

static float sigmoid(const float val)
{
    float ex = exp(-val);
    const auto res = 1 / (1 + ex);
    return res;
}

float EvaluationNnue::evaluate_sigmoid(const PositionBase& pos)
{
    auto score = evaluate_inner(pos);
    score = sigmoid(score);
    return score;
}

float EvaluationNnue::evaluate_inner(const PositionBase& pos)
{
    auto inputs = std::array<float_t, 98>{0};
    for(Rank rank = 0; rank < 7; rank++)
    {
        for(File file = 0; file < 7; file++)
        {
            const auto sq = get_square(file, rank);
            const auto index = get_index(file, rank);
            if (pos.Bitboards[Pieces::White] & (1ULL << sq))
            {
                inputs[index] = 1;
            }
            else if (pos.Bitboards[Pieces::Black] & (1ULL << sq))
            {
                inputs[49 + index] = 1;
            }
        }
    }

    double score = 0;
    for (auto i = 0; i < weights.size(); i++)
    {
        score += inputs[i] * weights[i];
    }
    score += bias;

    if (pos.Turn == Colors::Black)
    {
        score = -score;
    }

    return score;
}

Score EvaluationNnue::evaluate(const PositionBase& pos)
{
    auto score = evaluate_inner(pos);
    score *= 1024;
    const auto final_score = static_cast<Score>(round(score));
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
