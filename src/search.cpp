#include "search.h"
#include "evaluation.h"
#include "movegen.h"
#include "moveorder.h"

#include <iostream>

using namespace std;


Score Search::alpha_beta(const Position& pos, const Ply depth, const Ply ply, Score alpha, const Score beta, const bool is_pv)
{
    auto& ply_state = state.plies[ply];

    // WIN / LOSS / DRAW DETECTION
    if(pos.Bitboards[Pieces::Empty] == 0)
    {
        const auto own_count = pop_count(pos.Bitboards[pos.Turn]);
        const auto opp_count = pop_count(pos.Bitboards[!pos.Turn]);
        if(own_count > opp_count)
        {
            return mate - ply;
        }
        if(own_count < opp_count)
        {
            return ply - mate;
        }

        return 0;
    }

    // EARLY EXITS
    const Score static_eval = Evaluation::evaluate(pos);
    if(depth == 0 || ply == max_ply - 1 || depth > 2 && state.timer.should_stop())
    {
        return static_eval;
    }

    // TRANSPOSITION TABLE PROBING
    TranspositionTableEntry tt_entry;
    const bool tt_entry_exists = state.table.get(pos.Key, tt_entry);
    if (tt_entry_exists) {
        if (ply > 0 && tt_entry.depth >= depth) {
            if (tt_entry.flag == Upper && tt_entry.score <= alpha) {
                return tt_entry.score;
            }
            if (tt_entry.flag == Lower && tt_entry.score >= beta) {
                return tt_entry.score;
            }
            if (tt_entry.flag == Exact) {
                return tt_entry.score;
            }
        }
    }

    // REVERSE FUTILITY PRUNING
    if(!is_pv && depth < 7 && static_eval - 150 * depth > beta)
    {
        return beta;
    }

    MoveArray moves;
    MoveCount move_count = 0;
    MoveGenerator::generate(pos, moves, move_count);
    MoveScoreArray move_scores;
    MoveOrder::calculate_move_scores(pos, state, tt_entry.move, moves, move_scores, move_count);
    
    Score best_score = -inf;
    Move best_move = no_move;
    TranspositionTableFlag flag = Upper;
    int moves_evaluated = 0;
    for(MoveCount move_index = 0; move_index < move_count; ++move_index)
    {
        MoveOrder::order_next_move(moves, move_scores, move_count, move_index);

        const auto& move = moves[move_index];
        Position new_pos = pos.make_move(move);
        state.nodes++;

        // PRINCIPAL VARIATION SEARCH
        Score score;
        if(moves_evaluated > 0)
        {
            auto reduction = moves_evaluated > 8 && depth > 4 ? 2 : 0;
            score = -alpha_beta(new_pos, depth - 1 - reduction, ply + 1, -alpha - 1, -alpha, false);
        }
        if(moves_evaluated == 0 || (score > alpha && score < beta))
        {
            score = -alpha_beta(new_pos, depth - 1, ply + 1, -beta, -alpha, true);
        }

        moves_evaluated++;

        if(score > best_score)
        {
            best_score = score;
            best_move = move;

            if(score > alpha)
            {
                alpha = score;

                if(score >= beta)
                {
                    flag = Lower;
                    break;
                }

                flag = Exact;

                // SAVE PRINCIPAL VARIATION
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

    // STORE TRANSPOSITION TABLE
    if(!state.timer.stopped)
    {
        state.table.set(pos.Key, flag, best_score, depth, best_move);
    }

    return best_score;
}

void Search::iteratively_deepen(const Position& pos)
{
    Move saved_move = no_move;
    for(int depth = 1; depth <= max_ply; ++depth)
    {
        Score score = alpha_beta(pos, depth, 0, -inf, inf, true);
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
