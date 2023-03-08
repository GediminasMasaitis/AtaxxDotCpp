#include "search.h"
#include "evaluation.h"
#include "movegen.h"

#include <iostream>

using namespace std;


Score Search::alpha_beta(const Position& pos, const Ply depth, const Ply ply, Score alpha, const Score beta)
{
    auto& ply_state = state.plies[ply];

    if(depth == 0 || ply == max_ply - 1)
    {
        return Evaluation::evaluate(pos);
    }

    if(depth > 2 && state.timer.should_stop())
    {
        return 0;
    }

    MoveArray moves;
    MoveCount move_count = 0;
    MoveGenerator::generate(pos, moves, move_count);
    Score best_score = -inf;
    Move best_move = no_move;
    for(MoveCount i = 0; i < move_count; ++i)
    {
        const auto& move = moves[i];
        Position new_pos = pos.make_move(move);
        state.nodes++;
        const Score score = -alpha_beta(new_pos, static_cast<Ply>(depth - 1), static_cast<Ply>(ply + 1), -beta, -alpha);
        if(score > best_score)
        {
            best_score = score;
            best_move = move;

            if(score > alpha)
            {
                alpha = score;

                if(score > beta)
                {
                    break;
                }

                PrincipalVariationData& this_ply_pv = ply_state.principal_variation;
                this_ply_pv.moves[0] = best_move;
                if (ply < max_ply - 1)
                {
                    PlyState& next_ply_state = state.plies[ply + 1];
                    PrincipalVariationData& next_ply_pv = next_ply_state.principal_variation;
                    this_ply_pv.length = static_cast<Ply>(1 + next_ply_pv.length);
                    for (Ply next_ply_index = 0; next_ply_index < next_ply_pv.length; next_ply_index++)
                    {
                        this_ply_pv.moves[next_ply_index + 1] = next_ply_pv.moves[next_ply_index];
                    }
                }
                else
                {
                    this_ply_pv.length = 1;
                }
            }
        }
    }

    return best_score;
}

void Search::iteratively_deepen(const Position& pos)
{
    Move saved_move = no_move;
    for(int depth = 1; depth <= max_ply; ++depth)
    {
        Score score = alpha_beta(pos, depth, 0, -inf, inf);
        if(state.timer.should_stop())
        {
            break;
        }

        saved_move = state.plies[0].principal_variation.moves[0];

        Time elapsed = state.timer.elapsed();
        if(elapsed == 0)
        {
            elapsed = 1;
        }

        cout << "info";
        cout << " depth " << depth;
        cout << " time " << elapsed;
        cout << " score " << score;
        cout << " nodes " << state.nodes;
        cout << " nps " << state.nodes * 1000 / elapsed;
        cout << " pv ";
        for(int i = 0; i < state.plies[0].principal_variation.length; ++i)
        {
            const Move& pv_move = state.plies[0].principal_variation.moves[i];
            cout << pv_move.to_move_str() << " ";
        }
        cout << endl;
    }

    const auto move_str = saved_move.to_move_str();
    cout << "bestmove " << move_str << endl;
}

void Search::run(const Position& pos, const SearchParameters& parameters)
{
    state.parameters = parameters;
    state.plies = {};
    state.nodes = 0;
    const Time time = pos.Turn == Colors::White ? state.parameters.white_time : state.parameters.black_time;
    const Time increment = pos.Turn == Colors::White ? state.parameters.white_increment : state.parameters.black_increment;
    state.timer.init(parameters.infinite, time, increment);
    iteratively_deepen(pos);
}
