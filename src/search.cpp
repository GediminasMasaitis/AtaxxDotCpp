#include "search.h"

#include "display.h"
#include "evaluation.h"
#include "movegen.h"
#include "moveorder.h"

#include <iostream>
#include <thread>


using namespace std;

Score Search::alpha_beta(ThreadState& thread_state, Position& pos, Ply depth, const Ply ply, Score alpha, const Score beta, const bool is_pv)
{
    auto& ply_state = thread_state.plies[ply];

    // LOSS BY NO PIECES
    if(pos.Bitboards[pos.Turn] == 0)
    {
        return static_cast<Score>(ply - mate);
    }

    // WIN / LOSS / DRAW BY MAJORITY
    if((pos.Bitboards[Pieces::White] | pos.Bitboards[Pieces::Black]) == pos.playable)
    {
        const auto own_count = pop_count(pos.Bitboards[pos.Turn]);
        const auto opp_count = pop_count(pos.Bitboards[!pos.Turn]);
        if(own_count > opp_count)
        {
            return static_cast<Score>(mate - ply);
        }
        if(own_count < opp_count)
        {
            return static_cast<Score>(ply - mate);
        }

        return 0;
    }

    // WIN BY JAILING
    if(ply > 0 && pos.History[pos.HistoryCount - 1].move == passes[!pos.Turn])
    {
        const auto own_count = pop_count(pos.Bitboards[pos.Turn]);
        const auto opp_count = pop_count(pos.Bitboards[!pos.Turn]);
        const auto empty_count = pop_count(pos.Bitboards[Pieces::Empty]);
        if(own_count + empty_count > opp_count)
        {
            return static_cast<Score>(mate - ply - empty_count);
        }
    }

    // EARLY EXITS
    const Score static_eval = Evaluation::evaluate(pos);
    if(depth == 0 || ply == max_ply - 1 || depth > 2 && thread_state.timer.should_stop_max(thread_state.nodes))
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
    // INTERNAL ITERATIVE REDUCTION
    else if(depth > 3)
    {
        depth -= 2;
    }

    // REVERSE FUTILITY PRUNING
    if(!is_pv && depth < 8 && static_eval - 100 * depth > beta)
    {
        return beta;
    }

    MoveArray moves;
    MoveCount move_count = 0;
    MoveGenerator::generate_all(pos, moves, move_count);
    MoveScoreArray move_scores;
    MoveOrder::calculate_move_scores(pos, thread_state, tt_entry.move, moves, move_scores, move_count);
    
    Score best_score = -inf;
    Move best_move = no_move;
    TranspositionTableFlag flag = Upper;
    for(MoveCount move_index = 0; move_index < move_count; ++move_index)
    {
        MoveOrder::order_next_move(moves, move_scores, move_count, move_index);

        const auto& move = moves[move_index];
        pos.make_move_in_place(move);
        thread_state.nodes++;

        // PRINCIPAL VARIATION SEARCH
        Score score;
        if (move_index > 0)
        {
            // LATE MOVE REDUCTION
            auto reduction = 0;
            if(depth > 3 && move_index > 1)
            {
                reduction = 1 + !is_pv + move_index / 16;
            }

            score = -alpha_beta(thread_state, pos, depth - 1 - reduction, ply + 1, -alpha - 1, -alpha, false);
            if (reduction > 0 && score > alpha)
            {
                score = -alpha_beta(thread_state, pos, depth - 1, ply + 1, -alpha - 1, -alpha, false);
            }
            if (is_pv && score > alpha && score < beta)
            {
                score = -alpha_beta(thread_state, pos, depth - 1, ply + 1, -beta, -alpha, is_pv);
            }
        }
        else
        {
            score = -alpha_beta(thread_state, pos, depth - 1, ply + 1, -beta, -alpha, is_pv);
        }
        pos.unmake_move();

        if(score > best_score)
        {
            best_score = score;
            best_move = move;

            if(score > alpha)
            {
                alpha = score;

                if(score >= beta)
                {
                    thread_state.history[move.From][move.To] += depth * depth;
                    for(MoveCount i = 0; i < move_index; i++)
                    {
                        thread_state.history[moves[i].From][moves[i].To] -= depth * depth;
                    }
                    flag = Lower;
                    break;
                }

                flag = Exact;

                // SAVE PRINCIPAL VARIATION
                PrincipalVariationData& this_ply_pv = ply_state.principal_variation;
                this_ply_pv.moves[0] = best_move;
                if (ply < max_ply - 1)
                {
                    PlyState& next_ply_state = thread_state.plies[ply + 1];
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

        // LATE MOVE PRUNING
        if(move_index > 3 + 4 * depth * depth)
        {
            break;
        }
    }

    // STORE TRANSPOSITION TABLE
    if(!thread_state.timer.stopped)
    {
        state.table.set(pos.Key, flag, best_score, depth, best_move);
    }

    return best_score;
}

SearchResult Search::iteratively_deepen(ThreadState& thread_state, Position& pos)
{
    Score score = 0;
    for(int depth = 1; depth <= max_ply; ++depth)
    {
        score = alpha_beta(thread_state, pos, depth, 0, -inf, inf, true);
        if(thread_state.timer.stopped)
        {
            break;
        }

        thread_state.saved_pv = thread_state.plies[0].principal_variation;

        Time elapsed = thread_state.timer.elapsed();
        if(elapsed == 0)
        {
            elapsed = 1;
        }

        if (thread_state.parameters.print_info)
        {
            cout << "info";
            cout << " depth " << depth;
            cout << " time " << elapsed;
            cout << " score " << score;
            cout << " nodes " << thread_state.nodes;
            cout << " nps " << thread_state.nodes * 1000 / elapsed;
            cout << " pv ";
            for (int i = 0; i < thread_state.plies[0].principal_variation.length; ++i)
            {
                const Move& pv_move = thread_state.plies[0].principal_variation.moves[i];
                cout << pv_move.to_move_str() << " ";
            }
            cout << endl;
        }

        if (thread_state.timer.should_stop_min(thread_state.nodes))
        {
            break;
        }
    }

    if(thread_state.parameters.print_bestmove)
    {
        const auto move_str = thread_state.saved_pv.moves[0].to_move_str();
        cout << "bestmove " << move_str << endl;
    }

    const SearchResult result = SearchResult(thread_state.saved_pv.moves[0], score);
    return result;
}

SearchResult Search::lazy_smp(Position& pos)
{
    vector<thread> threads;

    for (ThreadId helper_id = 1; helper_id < Options::Threads; helper_id++)
    {
        state.threads[helper_id] = state.threads[0];
        state.threads[helper_id].parameters.infinite = true;
        state.threads[helper_id].parameters.print_info = false;
        state.threads[helper_id].parameters.print_bestmove = false;
        threads.emplace_back([this, helper_id, pos]()
        {
            auto pos_clone = pos;
            auto& thread_state = state.threads[helper_id];
            iteratively_deepen(thread_state, pos_clone);
        });
    }
    
    const auto result = iteratively_deepen(state.threads[0], pos);

    for (ThreadId helperId = 1; helperId < Options::Threads; helperId++)
    {
        state.threads[helperId].timer.stopped = true;
    }

    for (ThreadId helperId = 1; helperId < Options::Threads; helperId++)
    {
        threads[helperId - 1].join();
    }

    return result;
}

SearchResult Search::run(Position& pos, const SearchParameters& parameters)
{
    state.new_search(pos, parameters);
    const auto original_enable_accumulator_stack = pos.enable_accumulator_stack;
    pos.enable_accumulator_stack = true;
    const SearchResult result = lazy_smp(pos);
    pos.enable_accumulator_stack = original_enable_accumulator_stack;
    return result;
}
