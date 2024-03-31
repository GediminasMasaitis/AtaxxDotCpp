#include "datagen.h"

#include "display.h"
#include "evaluation.h"
#include "fens.h"
#include "movegen.h"
#include "moveorder.h"
#include "search.h"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <mutex>
#include <optional>
#include <random>
#include <thread>
#include <vector>

using namespace std;
using namespace chrono;

using Wdl = int8_t;

struct DatagenResult
{
    PositionBase position;
    Wdl wdl;
    Score score;
};

struct DatagenStatsEntry
{
    uint64_t nodes = 0;
    uint64_t positions = 0;
    uint64_t games = 0;

    DatagenStatsEntry& operator+=(const DatagenStatsEntry& other) {
        nodes += other.nodes;
        positions += other.positions;
        games += other.games;
        return *this;
    }
};

static DatagenStatsEntry operator+(DatagenStatsEntry lhs, const DatagenStatsEntry& rhs) {
    lhs += rhs;
    return lhs;
}

static std::ostream& operator<<(std::ostream& os, const DatagenStatsEntry& entry) {
    os << "Nodes: " << entry.nodes << ", Positions: " << entry.positions << ", Games: " << entry.games;
    return os;
}

struct DatagenStats
{
    DatagenStatsEntry total{};
    DatagenStatsEntry win_by_elimination{};
    DatagenStatsEntry discarded_repetition{};

    DatagenStats& operator+=(const DatagenStats& other)
    {
        total += other.total;
        win_by_elimination += other.win_by_elimination;
        discarded_repetition += other.discarded_repetition;
        return *this;
    }
};

static constexpr uint64_t seed_base = 0;
static constexpr ThreadCount thread_count = 12;
static const Position initial_pos = Fens::parse(initial_fen);

void write_result_bin(ostream& stream, const DatagenResult& result)
{
    stream.write(reinterpret_cast<const char*>(&result.position.Bitboards[Pieces::White]), sizeof(Bitboard));
    stream.write(reinterpret_cast<const char*>(&result.position.Bitboards[Pieces::Black]), sizeof(Bitboard));
    stream.write(reinterpret_cast<const char*>(&result.position.Turn), sizeof(Color));
    stream.write(reinterpret_cast<const char*>(&result.wdl), sizeof(Wdl));
    stream.write(reinterpret_cast<const char*>(&result.score), sizeof(Score));
    //stream.write("\n\n", 2);
}

void write_results_bin(ostream& stream, const vector<DatagenResult>& results)
{
    for(const DatagenResult& result : results)
    {
        write_result_bin(stream, result);
    }
    stream.flush();
}

void write_result_epd(ostream& stream, const DatagenResult& result)
{
    Fens::serialize(result.position, stream);
    const auto wdl_float = static_cast<float>(result.wdl) / 2;
    stream << "; " << wdl_float << endl;
}

void write_results_epd(ostream& stream, const vector<DatagenResult>& results)
{
    for (const DatagenResult& result : results)
    {
        write_result_epd(stream, result);
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
            return 2;
        }
        if (own_count < opp_count)
        {
            return 0;
        }

        return 1;
    }

    return nullopt;
}

void do_random_moves(Position& pos, mt19937_64& rng)
{
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
                if (is_terminal)
                {
                    break;
                }

                MoveGenerator::generate_all(pos, moves, move_count);
            }

            //for (MoveCount j = 0; j < move_count; j++)
            //{
            //    const auto move = moves[j];
            //    cout << move.to_move_str() << " ";
            //}

            const auto move_index = rng() % move_count;
            const auto move = moves[move_index];
            pos.make_move_in_place(move);
        }
        generate_initial_position = pos.is_terminal();
    }
}

void place_random_pieces(Position& pos, mt19937_64& rng)
{
    pos = initial_pos;
    for(Color color = Colors::White; color < Colors::Count; color++)
    {
        while(pop_count(pos.Bitboards[color]) < 5)
        {
            const auto file = rng() % 7;
            const auto rank = rng() % 7;
            const auto sq = get_square(file, rank);
            const auto bb = get_bitboard(sq);
            if(!(pos.Bitboards[Pieces::Empty] & bb))
            {
                continue;
            }

            pos.Squares[sq] = color;
            pos.Bitboards[color] |= bb;
            pos.Bitboards[Pieces::Empty] &= ~bb;
        }
    }
}

void run_iteration(const ThreadCount thread_id, const uint64_t iteration, Search& search, mutex& mut, DatagenStats& thread_stats, vector<DatagenResult>& thread_results, vector<DatagenResult>& iteration_results)
{
    const uint64_t seed = seed_base * 1'000'000'000'000ULL + static_cast<uint64_t>(thread_id) * 1'000'000'000ULL + iteration;
    auto rng = mt19937_64(seed);
    //cout << iteration << endl;

    auto iteration_stats_entry = DatagenStatsEntry{};
    iteration_stats_entry.games = 1;

    Position pos;

    do_random_moves(pos, rng);
    //place_random_pieces(pos, rng);

    assert(!pos.is_terminal());

    SearchParameters parameters;
    parameters.nodes_min = 10000;
    parameters.nodes_max = 50000;
    parameters.print_info = false;
    parameters.print_bestmove = false;

    while(true)
    {
        //Display::display_position(pos);
        const SearchResult search_result = search.run(pos, parameters);
        const DatagenResult datagen_result = DatagenResult{ pos, 0, search_result.score };
        iteration_results.push_back(datagen_result);
        const Move best_move = search.state.saved_pv.moves[0];
        pos.make_move_in_place(best_move);
        iteration_stats_entry.positions++;
        iteration_stats_entry.nodes += search.state.nodes;

        // On repetition, discard
        for(int i = 0; i < pos.HistoryCount; i++)
        {
            if(pos.History[i].key == pos.Key)
            {
                lock_guard lock(mut);
                thread_stats.discarded_repetition += iteration_stats_entry;
                thread_stats.total += iteration_stats_entry;
                return;
            }
        }

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
        wdl = 2 - wdl;
    }

    const bool is_elimination = pos.Bitboards[Pieces::Empty] != 0;

    for (DatagenResult& result : iteration_results)
    {
        result.wdl = wdl;
    }

    {
        lock_guard lock(mut);
        thread_stats.total += iteration_stats_entry;
        if(is_elimination)
        {
            thread_stats.win_by_elimination += iteration_stats_entry;
        }
        thread_results.insert(thread_results.end(), iteration_results.begin(), iteration_results.end());
    }
    iteration_results.clear();
}

static void run_thread(ThreadCount thread_id, mutex& mut, DatagenStats& thread_stats, vector<DatagenResult>& thread_results)
{
    Search search;
    search.state.table.set_size(1024 * 1024 * 32);
    vector<DatagenResult> iteration_results;
    for(uint64_t iteration = 0; iteration < 1'000'000'000'000; iteration++)
    {
        try
        {
            run_iteration(thread_id, iteration, search, mut, thread_stats, thread_results, iteration_results);
        }
        catch(...)
        {
            cout << "ERROR" << thread_id << " " << iteration << endl;
        }
    }
}

static void print_stats(const DatagenStats& stats, time_point<high_resolution_clock> train_start)
{
    const auto now = high_resolution_clock::now();
    const auto elapsed = now - train_start;
    const auto elapsed_seconds = std::chrono::duration_cast<seconds>(elapsed);
    auto elapsedSecondCount = static_cast<int32_t>(elapsed_seconds.count());
    if (elapsedSecondCount == 0)
    {
        elapsedSecondCount = 1;
    }

    const auto pps = static_cast<double>(stats.total.positions) / elapsedSecondCount;
    const auto pps_int = static_cast<size_t>(pps);

    const auto discarded_percent = (static_cast<double>(stats.discarded_repetition.positions) / static_cast<double>(stats.total.positions)) * 100;
    const auto eliminations_percent = (static_cast<double>(stats.win_by_elimination.games) / static_cast<double>(stats.total.games)) * 100;

    auto ss = stringstream();
    ss << " Games: " << std::right << std::setw(4) << stats.total.games;
    ss << " Positions: " << std::right << std::setw(9) << stats.total.positions;
    ss << " (" << std::right << std::setw(3) << pps_int << " PPS)";
    ss << " Discarded: " << std::fixed << std::setprecision(3) << discarded_percent << "%";
    ss << " Eliminations: " << std::fixed << std::setprecision(3) << eliminations_percent << "%";
    ss << std::endl;

    const auto log = ss.str();
    std::cout << log;
}

static void run_datagen()
{
    const auto train_start = high_resolution_clock::now();

    vector<thread> threads;
    array<mutex, thread_count> mutexes{};
    array<DatagenStats, thread_count> thread_stats{};
    array<vector<DatagenResult>, thread_count> thread_results{};
    for(ThreadCount thread_id = 0; thread_id < thread_count; thread_id++)
    {
        threads.emplace_back([&, thread_id]()
        {
            run_thread(thread_id, mutexes[thread_id], thread_stats[thread_id], thread_results[thread_id]);
        });
    }

#ifdef _WIN32
    constexpr auto out_bin_path = "C:/shared/ataxx/data/data2.bin";
    constexpr auto out_epd_path = "C:/shared/ataxx/data/data2.epd";
#else
    constexpr auto out_bin_path = "/mnt/c/shared/ataxx/data/data2.bin";
    constexpr auto out_epd_path = "/mnt/c/shared/ataxx/data/data2.epd";
#endif
    cout << "Writing binary results to " << out_bin_path << endl;
    ofstream bin_file(out_bin_path, std::ios::out | std::ios::binary);

    cout << "Writing epd results to " << out_epd_path << endl;
    ofstream epd_file(out_epd_path, std::ios::out);

    constexpr auto delay = milliseconds(1000);
    auto total_results = vector<DatagenResult>();
    while (true)
    {
        this_thread::sleep_for(delay);
        auto total_stats = DatagenStats();
        
        for(ThreadCount thread_id = 0; thread_id < thread_count; thread_id++)
        {
            lock_guard lock(mutexes[thread_id]);

            const auto& stats = thread_stats[thread_id];
            total_stats += stats;

            auto& this_thread_results = thread_results[thread_id];
            total_results.insert(total_results.end(), this_thread_results.begin(), this_thread_results.end());
            this_thread_results.clear();
        }

        print_stats(total_stats, train_start);
        write_results_bin(bin_file, total_results);
        //write_results_epd(epd_file, total_results);
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

struct DatagenResultExt
{
    Bitboard white;
    Bitboard black;
    Color turn;
    Wdl wdl;
    Score score;
    Square from;
    Square to;
};

DatagenResultExt get(ifstream& file)
{
    DatagenResultExt entry;
    file.read(reinterpret_cast<char*>(&entry.white), sizeof(entry.white));
    file.read(reinterpret_cast<char*>(&entry.black), sizeof(entry.black));
    file.read(reinterpret_cast<char*>(&entry.turn), sizeof(entry.turn));
    file.read(reinterpret_cast<char*>(&entry.wdl), sizeof(entry.wdl));
    file.read(reinterpret_cast<char*>(&entry.score), sizeof(entry.score));

    return entry;
}

void write_ext(ofstream& file, const DatagenResultExt& entry)
{
    file.write(reinterpret_cast<const char*>(&entry.white), sizeof(entry.white));
    file.write(reinterpret_cast<const char*>(&entry.black), sizeof(entry.black));
    file.write(reinterpret_cast<const char*>(&entry.turn), sizeof(entry.turn));
    file.write(reinterpret_cast<const char*>(&entry.wdl), sizeof(entry.wdl));
    file.write(reinterpret_cast<const char*>(&entry.score), sizeof(entry.score));
    file.write(reinterpret_cast<const char*>(&entry.from), sizeof(entry.from));
    file.write(reinterpret_cast<const char*>(&entry.to), sizeof(entry.to));
}

void set_pos(PositionBase& pos, const DatagenResultExt& entry)
{
    pos.Bitboards[Pieces::White] = entry.white;
    pos.Bitboards[Pieces::Black] = entry.black;
    pos.Bitboards[Pieces::Wall] = 0ULL;
    pos.Bitboards[Pieces::Empty] = ~(pos.Bitboards[Pieces::White] | pos.Bitboards[Pieces::Black]) & available_position;
    for (Rank rank = 0; rank < 7; rank++)
    {
        for (File file = 0; file < 7; file++)
        {
            const auto square = get_square(rank, file);
            auto bb = get_bitboard(square);
            if (pos.Bitboards[Pieces::Empty] & bb)
            {
                pos.Squares[square] = Pieces::Empty;
            }
            else if (pos.Bitboards[Pieces::White] & bb)
            {
                pos.Squares[square] = Pieces::White;
            }
            else if (pos.Bitboards[Pieces::Black] & bb)
            {
                pos.Squares[square] = Pieces::Black;
            }
        }
    }
    pos.Turn = entry.turn;
}

void Datagen::read()
{
#if DO_POLICY
    const auto path = "C:/shared/ataxx/data/data3M-policy.bin";
    auto file = ifstream(path);

    auto entry = get(file);
    Position pos;
    set_pos(pos, entry);

    Display::display_position(pos, nullopt);
    MoveOrder::get_all_policy_scores(pos);
#endif
}

std::vector<std::string> split_by_delimiter(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;

    // Loop until we have parsed the whole string
    while ((end = str.find(delimiter, start)) != std::string::npos) {
        // Extract substring from start to end
        tokens.push_back(str.substr(start, end - start));
        // Move the start position past this delimiter
        start = end + delimiter.length();
    }
    // Add the last token
    tokens.push_back(str.substr(start));

    return tokens;
}

void Datagen::convert2()
{
    const auto path = "C:/shared/ataxx/data/Zataxx-67M.txt";
    const auto out_path = "C:/shared/ataxx/data/Zataxx-67M-policy.bin";
    auto file = ifstream(path);
    auto out_file = ofstream(out_path, ios::binary);

    string line;
    auto index = 0;
    while(getline(file, line))
    {
        if(line.empty())
        {
            break;
        }

        auto tokens = split_by_delimiter(line, " | ");

        auto pos = Fens::parse(tokens[0]);
        //Display::display_position(pos, nullopt);
        //cout << "WDL: " << line[line.length() - 1] << endl;
        //cout << "NNUE: " << EvaluationNnue::evaluate(pos) << endl;

        DatagenResultExt entry;
        entry.white = pos.Bitboards[Pieces::White];
        entry.black = pos.Bitboards[Pieces::Black];
        entry.turn = pos.Turn;
        auto move_str = tokens[1];
        if(move_str == "0000")
        {
            continue;
        }
        auto move = Move::from_move_str(pos.Turn, move_str);
        entry.from = move.From;
        entry.to = move.To;
        entry.score = stoi(tokens[2]);
        auto wdl = stof(tokens[3]);
        auto wdl2 = static_cast<int8_t>(2 - (wdl * 2));
        entry.wdl = wdl2;

        write_ext(out_file, entry);

        constexpr size_t print_every = 1000000;
        if (index % print_every == print_every - 1)
        {
            cout << "Read " << index + 1 << " entries" << endl;
        }
        index++;
    }
    out_file.flush();
}

void Datagen::convert()
{
    constexpr static size_t file_entry_size = sizeof(Bitboard) + sizeof(Bitboard) + sizeof(Color) + sizeof(Wdl) + sizeof(Score);
    const auto path = "C:/shared/ataxx/data/data100M.bin";
    const auto out_path = "C:/shared/ataxx/data/data100M-policy.bin";
    const auto fs_path = filesystem::path(path);
    if (!filesystem::exists(fs_path))
    {
        cerr << path << " doesn't exist";
        throw exception();
    }

    const auto file_size = filesystem::file_size(fs_path);
    auto entry_count = file_size / file_entry_size;
    //entry_count = 100000;

    auto file = ifstream(path, ios::binary);
    if (!file)
    {
        cerr << "Failed to open file: " << path << endl;
        throw exception();
    }

    auto out_file = ofstream(out_path, ios::binary);

    constexpr size_t print_every = 1000000;
    vector<DatagenResultExt> entries;
    entries.reserve(entry_count);

    for(auto index = 0; index < entry_count; index++)
    {
        auto entry = get(file);
        entries.push_back(entry);
        if (index % print_every == print_every - 1 || index == entry_count - 1)
        {
            cout << "Read " << index + 1 << " entries" << endl;
        }
    }

    cout << "Read " << entries.size() << " entries" << endl;

    for (size_t index = 0; index < entry_count; index++)
    {
        //cout << index << endl;
        auto& entry = entries[index];
        PositionBase pos;
        set_pos(pos, entry);

        //Display::display_position(pos, nullopt);

        auto& next_entry = entries[index + 1];
        PositionBase next_pos;
        set_pos(next_pos, next_entry);

        MoveArray moves;
        MoveCount move_count = 0;
        MoveGenerator::generate_all(pos, moves, move_count);

        Move best_move;
        bool best_found = false;
        if(move_count == 1)
        {
            best_move = moves[0];
            best_found = true;
            //cout << "A";
        }
        else
        {
            //cout << "B";
            for (MoveCount move_index = 0; move_index < move_count; move_index++)
            {
                const auto move = moves[move_index];
                auto npos = pos.make_move_copy(move);
                if
                (
                    npos.Bitboards[Pieces::White] == next_pos.Bitboards[Pieces::White]
                    && npos.Bitboards[Pieces::Black] == next_pos.Bitboards[Pieces::Black]
                    && npos.Turn == next_pos.Turn
                )
                {
                    best_move = move;
                    if (best_found)
                    {
                        cout << "Multiple best moves found" << endl;
                        break;
                    }
                    best_found = true;
                }
            }
        }

        if(best_found && best_move != passes[Colors::White] && best_move != passes[Colors::White] && best_move.To != no_square)
        {
            entry.from = best_move.From;
            entry.to = best_move.To;
            auto a = 123;
            write_ext(out_file, entry);
        }

        //Display::display_position(pos, nullopt);
        //cout << "Best move: " << best_move.to_move_str() << endl;

        if (index == entry_count - 2)
        {
            break;
        }
    }

    out_file.flush();
    cout << "Done" << endl;
    
}
