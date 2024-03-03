#include "evaluation.h"
#include "attacks.h"

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

#if _WIN32
#else
#define ENABLE_INCBIN 1
#endif

#if ENABLE_INCBIN
#include "external/incbin/incbin.h"
INCBIN(network, "networks/default.nnue");
#endif

constexpr int32_t input_size = 98;
constexpr int32_t hidden_size = 16;

using nnue_param_t = int16_t;
//using nnue_param_t = float;

//static array<nnue_param_t, input_size> weights_single;
//static nnue_param_t bias_single;

static array<array<nnue_param_t, hidden_size>, input_size> weights1;
static array<nnue_param_t, hidden_size> bias1;
static array<nnue_param_t, hidden_size> weights2;
static nnue_param_t bias2;

static constexpr Square get_index(const File file, const Rank rank)
{
    return rank * 7 + file;
}

template<class T>
static T read(std::istream& stream)
{
    constexpr size_t size = sizeof(T);
    char buffer[size];
    stream.read(buffer, size);
    const T* resultPtr = reinterpret_cast<T*>(buffer);
    const auto result = *resultPtr;
    return result;
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

    //ss << "Weights single: ";
    //for(auto i = 0; i < input_size; i++)
    //{
    //    weights_single[i] = read<nnue_param_t>(file);
    //    ss << weights_single[i] << " ";
    //}
    //ss << endl << "Bias single: ";
    //bias_single = read<nnue_param_t>(file);
    //ss << bias_single << endl;
    

    ss << "Weights 1:" << endl;
    for (auto i = 0; i < hidden_size; i++)
    {
        for (auto j = 0; j < input_size; j++)
        {
            weights1[j][i] = read<nnue_param_t>(file);
            ss << weights1[j][i] << " ";
        }
        ss << endl;
    }

    ss << "Bias 1: ";
    for (auto i = 0; i < hidden_size; i++)
    {
        bias1[i] = read<nnue_param_t>(file);
        ss << bias1[i] << " ";
    }

    ss << endl << "Weights 2: ";
    for (auto i = 0; i < hidden_size; i++)
    {
        weights2[i] = read<nnue_param_t>(file);
        ss << weights2[i] << " ";
    }

    ss << endl << "Bias 2: ";
    bias2 = read<nnue_param_t>(file);
    ss << bias2 << endl;

    const auto str = ss.str();
    //cout << str;
}

Score EvaluationNnue::evaluate(const PositionBase& pos)
{
    const auto is_black = pos.Turn == Colors::Black;
    const auto us = is_black ? reverse_bits(pos.Bitboards[Colors::Black]) : pos.Bitboards[Colors::White];
    const auto them = is_black ? reverse_bits(pos.Bitboards[Colors::White]) : pos.Bitboards[Colors::Black];

    //Score score = bias_single;
    std::array<nnue_param_t, hidden_size> hidden = bias1;
    for (Rank rank = 0; rank < 7; rank++)
    {
        for (File file = 0; file < 7; file++)
        {
            const auto sq = get_square(file, rank);
            const auto index = get_index(file, rank);
            
            if (us & (1ULL << sq))
            {
                for (int i = 0; i < hidden_size; i++)
                {
                    hidden[i] += weights1[index][i];
                }
                //score += weights_single[index];
            }
            else if (them & (1ULL << sq))
            {
                for (int i = 0; i < hidden_size; i++)
                {
                    hidden[i] += weights1[49 + index][i];
                }
                //score += weights_single[49 + index];
            }
        }
    }

    int32_t score = 0;
    for(auto i = 0; i < hidden_size; i++)
    {
        const auto relu = std::max(hidden[i], static_cast<nnue_param_t>(0));
        //const auto relu = hidden[i];
        score += relu * weights2[i];
    }
    score += bias2 * 512;
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
