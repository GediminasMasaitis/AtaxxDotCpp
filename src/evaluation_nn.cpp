#include "evaluation.h"
#include "attacks.h"

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

#define ENABLE_INCBIN 1

#if ENABLE_INCBIN
#include "external/incbin/incbin.h"
INCBIN(network, "networks/default.nnue");
#endif

constexpr size_t input_size = 98;

using nnue_param_t = float;
static std::array<nnue_param_t, input_size> weights;
static nnue_param_t bias;

static constexpr Square get_index(const File file, const Rank rank)
{
    return rank * 7 + file;
}

void EvaluationNnue::init()
{
#if ENABLE_INCBIN
    auto file = stringstream(ios::in | ios::out | ios::binary);
    file.write(reinterpret_cast<const char*>(gnetworkData), gnetworkSize);
    cout << "Using included NNUE" << endl;
#else
    #ifdef _WIN32
        constexpr auto path = "C:/shared/ataxx/nets/default.nnue";
    #else
        constexpr auto path = "/mnt/c/shared/ataxx/nets/default.nnue";
    #endif
    //constexpr auto path = "./networks/default.nnue";
    cout << "Reading NNUE from " << path << endl;
    auto file = ifstream(path, ios::binary);
    if (!file.good())
    {
        cout << "Failed to open " << path << endl;
        return;
    }
#endif

    auto ss = stringstream();
    ss << "Weights: ";
    for (auto i = 0; i < weights.size(); i++)
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
    const auto is_black = pos.Turn == Colors::Black;
    const auto us = is_black ? reverse_bits(pos.Bitboards[Colors::Black]) : pos.Bitboards[Colors::White];
    const auto them = is_black ? reverse_bits(pos.Bitboards[Colors::White]) : pos.Bitboards[Colors::Black];

    auto inputs = std::array<float_t, input_size>{0};
    for (Rank rank = 0; rank < 7; rank++)
    {
        for (File file = 0; file < 7; file++)
        {
            const auto sq = get_square(file, rank);
            const auto index = get_index(file, rank);
            if (us & (1ULL << sq))
            {
                inputs[index] = 1;
            }
            else if (them & (1ULL << sq))
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

    return score;
}

Score EvaluationNnue::evaluate(const PositionBase& pos)
{
    auto score = evaluate_inner(pos);
    score *= 512;
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
