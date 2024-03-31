#include "evaluation_nn_base.h"

#include "attacks.h"

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

#if _WIN32
//#define ENABLE_INCBIN 1
#else
#define ENABLE_INCBIN 1
#endif

#if ENABLE_INCBIN
#include "external/incbin/incbin.h"
INCBIN(network, "networks/default.nnue-floats");
#endif

static EvaluationNnueBase::nnue_param_t read(std::istream& stream)
{
    constexpr size_t size = sizeof(float);
    char buffer[size];
    stream.read(buffer, size);
    const float* float_ptr = reinterpret_cast<float*>(buffer);
    const auto result_float = *float_ptr;
    const auto result_scaled = result_float * 128;
    const auto result_rounded = round(result_scaled);
    const auto result_quantized = static_cast<EvaluationNnueBase::nnue_param_t>(result_rounded);
    return result_quantized;
}

void EvaluationNnueBase::init()
{
#if ENABLE_INCBIN
    auto file = stringstream(ios::in | ios::out | ios::binary);
    file.write(reinterpret_cast<const char*>(gnetworkData), gnetworkSize);
    cout << "Using included NNUE" << endl;
#else
    #ifdef _WIN32
        constexpr auto path = "C:/shared/ataxx/nets/default.nnue-floats";
    #else
        constexpr auto path = "/mnt/c/shared/ataxx/nets/default.nnue-floats";
    #endif
    cout << "Reading NNUE from " << path << endl;
    auto file = ifstream(path, ios::binary);
    if (!file.good())
    {
        cout << "Failed to open " << path << endl;
        return;
    }
#endif

    auto ss = stringstream();

    ss << "Weights 1:" << endl;
    for (auto i = 0; i < hidden_size; i++)
    {
        for (auto j = 0; j < input_size; j++)
        {
            input_weights[j][i] = read(file);
            ss << input_weights[j][i] << " ";
        }
        ss << endl;
    }

    ss << "Bias 1: ";
    for (auto i = 0; i < hidden_size; i++)
    {
        input_biases[i] = read(file);
        ss << input_biases[i] << " ";
    }

    ss << endl << "Weights 2: ";
    //for (auto i = 0; i < hidden_size; i++)
    //{
    //    hidden_weights[i] = read(file);
    //    ss << hidden_weights[i] << " ";
    //}

    for(Color color = 0; color < Colors::Count; color++)
    {
        for (auto i = 0; i < hidden_size; i++)
        {
            hidden_weightses[color][i] = read(file);
            ss << hidden_weightses[color][i] << " ";
        }
    }

    ss << endl << "Bias 2: ";
    hidden_bias = read(file);
    ss << hidden_bias << endl;

    const auto str = ss.str();
    //cout << str;
}
