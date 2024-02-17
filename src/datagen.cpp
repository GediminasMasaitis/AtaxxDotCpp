#include "datagen.h"

#include "display.h"
#include "fens.h"
#include "movegen.h"
#include "search.h"

#include <iostream>
#include <fstream>
#include <random>
#include <vector>

using namespace std;

using Wdl = int8_t;

struct DatagenResult
{
    PositionBase position;
    Wdl wdl;
    Score score;
};

void write_result(ostream& stream, const DatagenResult& result)
{
    stream.write(reinterpret_cast<const char*>(&result.position.Bitboards[Pieces::White]), sizeof(Bitboard));
    stream.write(reinterpret_cast<const char*>(&result.position.Bitboards[Pieces::Black]), sizeof(Bitboard));
    stream.write(reinterpret_cast<const char*>(&result.position.Turn), sizeof(Color));
    stream.write(reinterpret_cast<const char*>(&result.wdl), sizeof(Wdl));
    stream.write(reinterpret_cast<const char*>(&result.score), sizeof(Score));
    //stream.write("\n\n", 2);
}

void write_results(const vector<DatagenResult>& results)
{
    ofstream file("C:/shared/ataxx/data/data.bin", std::ios::out | std::ios::binary);
    for(const DatagenResult& result : results)
    {
        write_result(file, result);
    }
    file.close();
}

static const Position initial_pos = Fens::parse(initial_fen);

void run_iteration(Search& search, mt19937& rng)
{
    Position pos;

    while(true)
    {
        bool generate_initial_position = true;
        const MoveCount random_move_count = rng() % 2 == 0 ? 8 : 9;
        while (generate_initial_position)
        {
            generate_initial_position = false;
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

                if(pos.Key == 4297044965681430157)
                {
                    auto a = 123;
                }

                if (move_count == 0)
                {
                    MoveGenerator::generate_all(pos, moves, move_count);
                }

                if (move_count == 0)
                {
                    Display::display_position(pos);
                    generate_initial_position = true;
                    break;
                }

                const auto move_index = rng() % move_count;
                const auto move = moves[move_index];
                pos.make_move_in_place(move);
            }
        }
        Display::display_position(pos);
    }


    SearchParameters parameters;
    parameters.nodes_min = 10000;
    parameters.nodes_max = 50000;

    std::vector<DatagenResult> results;
    while(pos.Bitboards[Pieces::Empty])
    {
        Display::display_position(pos);
        const SearchResult search_result = search.run(pos, parameters);
        const DatagenResult datagen_result = DatagenResult{ pos, 0, search_result.score };
        results.push_back(datagen_result);
        const Move best_move = search.state.saved_pv.moves[0];
        pos.make_move_in_place(best_move);
    }
    Display::display_position(pos);
    for(DatagenResult& result : results)
    {
        result.wdl = 1;
    }
    write_results(results);
}

static void run_thread()
{
    mt19937 rng(0);
    Search search;
    search.state.table.set_size(1024 * 1024 * 32);
    run_iteration(search, rng);
}

static void run_datagen()
{
    run_thread();
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
