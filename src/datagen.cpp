#include "datagen.h"

#include "display.h"
#include "fens.h"
#include "search.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

using Wdl = int8_t;

struct DatagenResult
{
    Position position;
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

void run_iteration(Search& search)
{
    Position pos = Fens::parse(initial_fen);
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
    Search search;
    search.state.table.set_size(1024 * 1024 * 32);
    run_iteration(search);
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
