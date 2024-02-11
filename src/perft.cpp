#include "perft.h"

#include "fens.h"
#include "movegen.h"

#include <cassert>
#include <chrono>
#include <iostream>

using namespace std;

uint64_t Perft::perft(PositionBase& pos, const Ply depth)
{
    if (depth == 0)
    {
        return 1;
    }

    MoveArray moves;
    MoveCount move_count = 0;
    MoveGenerator::generate_all(pos, moves, move_count);

    uint64_t nodes = 0;
    for (MoveCount i = 0; i < move_count; ++i)
    {
        const Move move = moves[i];
        //pos.make_move_in_place(move);
        const auto npos = pos.make_move_copy(move);
        nodes += perft(pos, depth - 1);
        //pos.unmake_move();
    }

    return nodes;
}

void Perft::perft_deepen(PositionBase& pos, const Ply depth, vector<uint64_t>* verification)
{
    for (Ply i = 1; i <= depth; ++i)
    {
        auto start = chrono::high_resolution_clock::now();
        const uint64_t nodes = perft(pos, i);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        if (duration == 0)
        {
            duration = 1;
        }
        auto nps = nodes * 1000 / duration;
        cout << "time " << duration << " depth " << to_string(i) << " nodes " << nodes << " nps " << nps;
        if(verification != nullptr)
        {
            if(verification->size() > i)
            {
                const auto iteration_verification = (*verification)[i];
                if(iteration_verification == nodes)
                {
                    cout << " verify OK";
                }
                else
                {
                    cout << " verify FAIL " << iteration_verification;
                }
            }
        }
        cout << endl;
    }
}

void Perft::perft_deepen(const Fen& fen, const Ply depth, vector<uint64_t>* verification)
{
    Position pos = Fens::parse(fen);
    perft_deepen(pos, depth, verification);
}

void Perft::perft_suite()
{
    // Positions from https://github.com/kz04px/libataxx/blob/master/tests/perft.cpp
    vector<pair<string, vector<uint64_t>>> positions = {
        {"7/7/7/7/7/7/7 x 0 1", {1, 0, 0, 0, 0}},
        {"7/7/7/7/7/7/7 o 0 1", {1, 0, 0, 0, 0}},
        {"x5o/7/7/7/7/7/o5x x 0 1", {1, 16, 256, 6460, 155888, 4752668}},
        {"x5o/7/7/7/7/7/o5x o 0 1", {1, 16, 256, 6460, 155888, 4752668}},
        {"x5o/7/2-1-2/7/2-1-2/7/o5x x 0 1", {1, 14, 196, 4184, 86528, 2266352}},
        {"x5o/7/2-1-2/7/2-1-2/7/o5x o 0 1", {1, 14, 196, 4184, 86528, 2266352}},
        {"x5o/7/2-1-2/3-3/2-1-2/7/o5x x 0 1", {1, 14, 196, 4100, 83104, 2114588}},
        {"x5o/7/2-1-2/3-3/2-1-2/7/o5x o 0 1", {1, 14, 196, 4100, 83104, 2114588}},
        {"x5o/7/3-3/2-1-2/3-3/7/o5x x 0 1", {1, 16, 256, 5948, 133264, 3639856}},
        {"x5o/7/3-3/2-1-2/3-3/7/o5x o 0 1", {1, 16, 256, 5948, 133264, 3639856}},
        {"7/7/7/7/ooooooo/ooooooo/xxxxxxx x 0 1", {1, 1, 75, 249, 14270, 452980}},
        {"7/7/7/7/ooooooo/ooooooo/xxxxxxx o 0 1", {1, 75, 249, 14270, 452980}},
        {"7/7/7/7/xxxxxxx/xxxxxxx/ooooooo x 0 1", {1, 75, 249, 14270, 452980}},
        {"7/7/7/7/xxxxxxx/xxxxxxx/ooooooo o 0 1", {1, 1, 75, 249, 14270, 452980}},
        {"7/7/7/2x1o2/7/7/7 x 0 1", {1, 23, 419, 7887, 168317, 4266992}},
        {"7/7/7/2x1o2/7/7/7 o 0 1", {1, 23, 419, 7887, 168317, 4266992}},
        {"x5o/7/7/7/7/7/o5x x 100 1", {1, 0, 0, 0, 0}},
        {"x5o/7/7/7/7/7/o5x o 100 1", {1, 0, 0, 0, 0}},
        {"7/7/7/7/-------/-------/x5o x 0 1", {1, 2, 4, 13, 30, 73, 174}},
        {"7/7/7/7/-------/-------/x5o o 0 1", {1, 2, 4, 13, 30, 73, 174}},
    };

    for(auto& position : positions)
    {
        cout << position.first << endl;
        vector<uint64_t>& verification = position.second;
        perft_deepen(position.first, verification.size() - 1, &verification);
        cout << endl;
    }
}
