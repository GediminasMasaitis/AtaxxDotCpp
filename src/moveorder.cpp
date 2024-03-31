#include "moveorder.h"
#include "attacks.h"

#include <cmath>
#include <fstream>
#include <iostream>

#define DO_POLICY 0

#if DO_POLICY

#if _WIN32
#else
#define ENABLE_INCBIN 1
#endif

#if ENABLE_INCBIN
#include "external/incbin/incbin.h"
INCBIN(policy, "networks/policy.nnue-floats");
#endif

using namespace std;

struct PolicyIndexesClass
{
    std::array<std::array<int16_t, 64>, 64> values;

    constexpr PolicyIndexesClass()
    {
        values = {};
        for (Square from = 0; from < 64; from++)
        {
            for (Square to = 0; to < 64; to++)
            {
                values[from][to] = -1;
            }
        }

        uint16_t index = 0;
        for (Rank rank = 0; rank < 7; rank++)
        {
            for (File file = 0; file < 7; file++)
            {
                const auto sq = get_square(file, rank);
                values[no_square][sq] = index++;
            }
        }
        for (Rank rank = 0; rank < 7; rank++)
        {
            for (File file = 0; file < 7; file++)
            {
                const auto from = get_square(file, rank);
                auto bb = Attacks.far[from];
                while (bb)
                {
                    const auto to = lsb(bb);
                    bb &= bb - 1;
                    values[from][to] = index++;
                }
            }
        }
    }
};

static constexpr PolicyIndexesClass policy_indexes = PolicyIndexesClass();

using nnue_count_t = int32_t;
static constexpr nnue_count_t input_size = 98;
static constexpr nnue_count_t hidden_size = 128;
static constexpr nnue_count_t output_size = 610;
using nnue_param_t = int32_t;

using accumulator_t = std::array<int32_t, hidden_size>;

static std::array<std::array<nnue_param_t, hidden_size>, input_size> weights1;
static std::array<nnue_param_t, hidden_size> biases1;
static std::array<std::array<nnue_param_t, hidden_size>, output_size> weights2;
static std::array<nnue_param_t, output_size> biases2;

static nnue_param_t read(std::istream& stream)
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

void MoveOrder::init()
{
#if ENABLE_INCBIN
    auto file = stringstream(ios::in | ios::out | ios::binary);
    file.write(reinterpret_cast<const char*>(gpolicyData), gpolicySize);
    cout << "Using included policy" << endl;
#else
#ifdef _WIN32
    constexpr auto path = "C:/shared/ataxx/nets/policy.nnue-floats";
#else
    constexpr auto path = "/mnt/c/shared/ataxx/nets/policy.nnue-floats";
#endif
    cout << "Reading policy from " << path << endl;
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
            weights1[j][i] = read(file);
            ss << weights1[j][i] << " ";
        }
        ss << endl;
    }

    ss << "Biases 1: ";
    for (auto i = 0; i < hidden_size; i++)
    {
        biases1[i] = read(file);
        ss << biases1[i] << " ";
    }

    ss << "Weights 2:" << endl;
    for (auto i = 0; i < output_size; i++)
    {
        for (auto j = 0; j < hidden_size; j++)
        {
            weights2[i][j] = read(file);
            ss << weights2[i][j] << " ";
        }
        ss << endl;
    }

    ss << "Biases 2: ";
    for (auto i = 0; i < output_size; i++)
    {
        biases2[i] = read(file);
        ss << biases2[i] << " ";
    }

    const auto str = ss.str();
    //cout << str;
}

//static constexpr Square flip_square(const Square sq)
//{
//    return (63 - sq) - 9;
//}

static constexpr Square get_index(const File file, const Rank rank)
{
    return rank * 7 + file;
}

void get_accumulator(const PositionBase& position, accumulator_t& accumulator)
{
    accumulator = biases1;

    const auto is_black = position.Turn == Colors::Black;
    const auto us_stm = is_black ? reverse_bits(position.Bitboards[Pieces::Black]) : position.Bitboards[Pieces::White];
    const auto them_stm = is_black ? reverse_bits(position.Bitboards[Pieces::White]) : position.Bitboards[Pieces::Black];

    for (Rank rank = 0; rank < 7; rank++)
    {
        for (File file = 0; file < 7; file++)
        {
            const auto square = get_square(file, rank);
            const auto sq_index = get_index(file, rank);

            if ((us_stm >> square) & 1)
            {
                auto& weightsArr = weights1[sq_index];
                for (auto i = 0; i < hidden_size; i++)
                {
                    accumulator[i] += weightsArr[i];
                }
            }
            if ((them_stm >> square) & 1)
            {
                auto& weightsArr = weights1[49 + sq_index];
                for (auto i = 0; i < hidden_size; i++)
                {
                    accumulator[i] += weightsArr[i];
                }
            }
        }
    }

    for (auto i = 0; i < hidden_size; i++)
    {
        accumulator[i] = std::max(static_cast<nnue_param_t>(0), accumulator[i]);
    }
}

MoveScore get_policy_score(const accumulator_t& accumulator, const int16_t output_index)
{
    int32_t score = biases2[output_index] * 128;
    auto& weights_arr = weights2[output_index];
    for(auto i = 0; i < hidden_size; i++)
    {
        score += accumulator[i] * weights_arr[i];
    }

    score /= 128;

    return score;
}

MoveScore get_policy_score(const accumulator_t& accumulator, const Move& move)
{
    const auto index = policy_indexes.values[move.From][move.To];
    const auto score = get_policy_score(accumulator, index);
    return score;
}

void MoveOrder::get_all_policy_scores(const PositionBase& position)
{
    accumulator_t accumulator;
    get_accumulator(position, accumulator);

    auto scores = std::array<MoveScore, 610>{};
    for(auto i = 0; i < scores.size(); i++)
    {
        scores[i] = get_policy_score(accumulator, i);
        cout << scores[i] << " ";
    }
    cout << endl;
    auto a = 123;
}
#else
void MoveOrder::init()
{
}
#endif

void MoveOrder::calculate_move_scores(const PositionBase& position, const SearchState& state, const Move& tt_move, const MoveArray& moves, MoveScoreArray& move_scores, const MoveCount move_count)
{
#if DO_POLICY
    accumulator_t accumulator;
    get_accumulator(position, accumulator);
#endif

    for(int i = 0; i < move_count; i++)
    {
        MoveScore move_score;
        const Move& move = moves[i];
        if(move == tt_move)
        {
            move_score = 1LL << 60;
        }
        else
        {
#if DO_POLICY
            move_score = get_policy_score(accumulator, move);
#else
            move_score = 0;

            const Bitboard captures = Attacks.near[move.To] & position.Bitboards[!position.Turn];
            const int32_t material_gain = pop_count(captures) + (move.From == no_square ? 1 : 0);
            //move_score += material_gain * 64;

            move_score = 1LL << (50 + material_gain);
            move_score += state.history[move.From][move.To];
#endif
        }
        move_scores[i] = move_score;
    }
}

void MoveOrder::order_next_move(MoveArray& moves, MoveScoreArray& move_scores, const MoveCount move_count, MoveCount current_index)
{
    MoveScore bestScore = std::numeric_limits<MoveScore>::min();
    MoveCount bestScoreIndex = 0;

    for (MoveCount i = current_index; i < move_count; i++)
    {
        const MoveScore staticScore = move_scores[i];
        const MoveScore score = staticScore;

        if (score > bestScore)
        {
            bestScore = score;
            bestScoreIndex = i;
        }
    }

    const Move tempMove = moves[current_index];
    moves[current_index] = moves[bestScoreIndex];
    moves[bestScoreIndex] = tempMove;

    const MoveScore tempScore = move_scores[current_index];
    move_scores[current_index] = move_scores[bestScoreIndex];
    move_scores[bestScoreIndex] = tempScore;
}
