#include "evaluation_nn_base.h"

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

void EvaluationNnueBase::init()
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
            input_weights[j][i] = read<nnue_param_t>(file);
            ss << input_weights[j][i] << " ";
        }
        ss << endl;
    }

    ss << "Bias 1: ";
    for (auto i = 0; i < hidden_size; i++)
    {
        input_biases[i] = read<nnue_param_t>(file);
        ss << input_biases[i] << " ";
    }

    ss << endl << "Weights 2: ";
    for (auto i = 0; i < hidden_size; i++)
    {
        hidden_weights[i] = read<nnue_param_t>(file);
        ss << hidden_weights[i] << " ";
    }

    ss << endl << "Bias 2: ";
    hidden_bias = read<nnue_param_t>(file);
    ss << hidden_bias << endl;

    const auto str = ss.str();
    //cout << str;
}
