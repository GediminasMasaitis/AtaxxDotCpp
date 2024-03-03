#ifndef EVALUATION_NN_BASE_H
#define EVALUATION_NN_BASE_H 1

#include "types.h"

struct EvaluationNnueBase
{
    using nnue_count_t = int32_t;

    static constexpr nnue_count_t input_size = 98;
    static constexpr nnue_count_t hidden_size = 64;

    using nnue_param_t = int16_t;
    using input_weights_t = std::array<std::array<nnue_param_t, hidden_size>, input_size>;
    using input_biases_t = std::array<nnue_param_t, hidden_size>;
    using hidden_weights_t = std::array<nnue_param_t, hidden_size>;
    using hidden_bias_t = nnue_param_t;

    using input_layer_t = std::array<nnue_param_t, input_size>;
    using hidden_layer_t = std::array<nnue_param_t, hidden_size>;
    using hidden_layers_t = EachColor<hidden_layer_t>;

    inline static input_weights_t input_weights;
    inline static input_biases_t input_biases;
    inline static hidden_weights_t hidden_weights;
    inline static hidden_bias_t hidden_bias;

    static constexpr Square get_index(const File file, const Rank rank)
    {
        return rank * 7 + file;
    }

    static constexpr Square square_to_index(const Square sq)
    {
        return get_index(get_file(sq), get_rank(sq));
    }

    static constexpr Square flip_square(const Square sq)
    {
        return (63 - sq) - 9;
    }

    template<bool TSet>
    static void apply_piece_single(hidden_layer_t& hidden_layer, const Square sq, Piece piece)
    {
        const auto index = square_to_index(sq);
        const auto input_index = piece * 49 + index;
        for (nnue_count_t hidden_index = 0; hidden_index < hidden_size; hidden_index++)
        {
            if constexpr (TSet)
            {
                hidden_layer[hidden_index] += input_weights[input_index][hidden_index];
            }
            else
            {
                hidden_layer[hidden_index] -= input_weights[input_index][hidden_index];
            }
        }
    }

    template<bool TSet>
    static void apply_piece(hidden_layers_t& hidden_layers, const Square sq, const Piece piece)
    {
        const Square flipped_sq = flip_square(sq);
        apply_piece_single<TSet>(hidden_layers[Colors::White], sq, piece);
        apply_piece_single<TSet>(hidden_layers[Colors::Black], flipped_sq, piece ^ 1);
    }

    static void init();
};

#endif // !EVALUATION_NN_BASE_H
