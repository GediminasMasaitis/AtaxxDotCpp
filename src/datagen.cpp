#include "datagen.h"

#include <cassert>

#include "display.h"
#include "fens.h"
#include "movegen.h"
#include "search.h"

#include <iostream>
#include <fstream>
#include <mutex>
#include <optional>
#include <random>
#include <thread>
#include <vector>

using namespace std;

using Wdl = int8_t;

struct DatagenResult
{
    PositionBase position;
    Wdl wdl;
    Score score;
};

struct DatagenStats
{
    uint64_t nodes = 0;
    uint64_t positions = 0;
    uint64_t games = 0;
};

static constexpr ThreadCount thread_count = 8;
static const Position initial_pos = Fens::parse(initial_fen);

void write_result(ostream& stream, const DatagenResult& result)
{
    stream.write(reinterpret_cast<const char*>(&result.position.Bitboards[Pieces::White]), sizeof(Bitboard));
    stream.write(reinterpret_cast<const char*>(&result.position.Bitboards[Pieces::Black]), sizeof(Bitboard));
    stream.write(reinterpret_cast<const char*>(&result.position.Turn), sizeof(Color));
    stream.write(reinterpret_cast<const char*>(&result.wdl), sizeof(Wdl));
    stream.write(reinterpret_cast<const char*>(&result.score), sizeof(Score));
    //stream.write("\n\n", 2);
}

void write_results(ostream& stream, const vector<DatagenResult>& results)
{
    for(const DatagenResult& result : results)
    {
        write_result(stream, result);
    }
    stream.flush();
}

optional<Wdl> adjudicate(const Position& pos)
{
    if(pos.Bitboards[pos.Turn] == 0)
    {
        return 0;
    }

    if ((pos.Bitboards[Pieces::White] | pos.Bitboards[Pieces::Black]) == pos.playable)
    {
        const auto own_count = pop_count(pos.Bitboards[pos.Turn]);
        const auto opp_count = pop_count(pos.Bitboards[!pos.Turn]);
        if (own_count > opp_count)
        {
            return 1;
        }
        if (own_count < opp_count)
        {
            return 0;
        }

        return 0.5;
    }

    return nullopt;
}

void run_iteration(Search& search, mt19937& rng, mutex& mut, DatagenStats& thread_stats, vector<DatagenResult>& thread_results, vector<DatagenResult>& iteration_results)
{
    DatagenStats iteration_stats = DatagenStats{};
    Position pos;
    bool generate_initial_position = true;
    const MoveCount random_move_count = rng() % 2 == 0 ? 8 : 9;
    while (generate_initial_position)
    {
        pos = initial_pos;
        for (MoveCount i = 0; i < random_move_count; i++)
        {
            MoveArray moves;
            MoveCount move_count = 0;

            const auto do_far = (rng() % 3) == 0;
            if (do_far)
            {
                MoveGenerator::generate_far(pos, moves, move_count);
            }
            else
            {
                MoveGenerator::generate_near(pos, moves, move_count);
            }

            if (move_count == 0)
            {
                const auto is_terminal = pos.is_terminal();
                if(is_terminal)
                {
                    break;
                }

                MoveGenerator::generate_all(pos, moves, move_count);
            }

            const auto move_index = rng() % move_count;
            const auto move = moves[move_index];
            pos.make_move_in_place(move);
        }

        generate_initial_position = pos.is_terminal();
    }
    assert(!pos.is_terminal());

    SearchParameters parameters;
    parameters.nodes_min = 10000;
    parameters.nodes_max = 50000;

    while(true)
    {
        //Display::display_position(pos);
        const SearchResult search_result = search.run(pos, parameters);
        const DatagenResult datagen_result = DatagenResult{ pos, 0, search_result.score };
        iteration_results.push_back(datagen_result);
        const Move best_move = search.state.saved_pv.moves[0];
        pos.make_move_in_place(best_move);
        if(pos.is_terminal())
        {
            break;
        }
    }

    //Display::display_position(pos);
    const auto adj_result = adjudicate(pos);

    assert(adj_result.has_value());
    Wdl wdl = adj_result.value();
    if (pos.Turn == Colors::Black)
    {
        wdl = 1 - wdl;
    }

    for (DatagenResult& result : iteration_results)
    {
        result.wdl = wdl;
    }

    {
        lock_guard lock(mut);
        thread_stats.nodes += iteration_stats.nodes;
        thread_stats.positions += iteration_stats.positions;
        thread_stats.games += iteration_stats.games;
        thread_results.insert(thread_results.end(), iteration_results.begin(), iteration_results.end());
        iteration_results.clear();
    }
}

static void run_thread(ThreadCount thread_id, mutex& mut, DatagenStats& thread_stats, vector<DatagenResult>& thread_results)
{
    mt19937 rng(thread_id);
    Search search;
    search.state.table.set_size(1024 * 1024 * 32);
    vector<DatagenResult> iteration_results;
    while (true)
    {
        run_iteration(search, rng, mut, thread_stats, thread_results, iteration_results);
    }
}

static void run_datagen()
{
    vector<thread> threads;
    array<mutex, thread_count> mutexes;
    array<DatagenStats, thread_count> thread_stats;
    array<vector<DatagenResult>, thread_count> thread_results;
    for(ThreadCount thread_id = 0; thread_id < thread_count; thread_id++)
    {
        threads.emplace_back([&, thread_id]()
        {
            run_thread(thread_id, mutexes[thread_id], thread_stats[thread_id], thread_results[thread_id]);
        });
    }

    ofstream file("C:/shared/ataxx/data/data.bin", std::ios::out | std::ios::binary);
    constexpr chrono::milliseconds delay = chrono::milliseconds(1000);
    auto total_results = vector<DatagenResult>();
    while (true)
    {
        this_thread::sleep_for(delay);
        auto total_stats = DatagenStats();
        
        for(ThreadCount thread_id = 0; thread_id < thread_count; thread_id++)
        {
            lock_guard lock(mutexes[thread_id]);

            const auto& stats = thread_stats[thread_id];
            total_stats.nodes += stats.nodes;
            total_stats.positions += stats.positions;
            total_stats.games += stats.games;

            auto& this_thread_results = thread_results[thread_id];
            total_results.insert(total_results.end(), this_thread_results.begin(), this_thread_results.end());
            this_thread_results.clear();
        }

        cout << total_results.size() << endl;
        write_results(file, total_results);
        total_results.clear();
    }
}

void Datagen::run()
{
    if constexpr (do_datagen)
    {
        run_datagen();
    }
    else
    {
        cout << "Datagen is not enabled";
    }
}
