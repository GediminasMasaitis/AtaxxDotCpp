#include "uai.h"

#include "datagen.h"
#include "fens.h"
#include "position.h"
#include "perft.h"

#include <iostream>
#include <sstream>

using namespace std;

static string get_uci_line()
{
    string command;
    getline(cin, command);
    return command;
}

void Uai::handle_uai()
{
    cout << "id name Ataxx.cpp" << endl;
    cout << "id author Gediminas Masaitis" << endl;
    cout << "uaiok" << endl;
}

void Uai::handle_uainewgame()
{
    current_pos = Fens::parse(initial_fen);
    search.state.table.clear();
}

void Uai::handle_startpos(std::stringstream& reader)
{
    current_pos = Fens::parse(initial_fen);
    if (!reader.eof())
    {
        std::string type;
        reader >> type;

        if (type == "moves")
        {
            handle_moves(reader);
        }
    }
}

void Uai::handle_moves(std::stringstream& reader)
{
    while (!reader.eof())
    {
        MoveStr moveStr;
        reader >> moveStr;

        current_pos = current_pos.make_move(moveStr);
    }
}

void Uai::handle_fen(std::stringstream& reader)
{
    std::stringstream fen_builder;
    bool hasMoves = false;
    while (!reader.eof())
    {
        std::string fenPart;
        reader >> fenPart;
        if (fenPart == "moves")
        {
            hasMoves = true;
            break;
        }
        fen_builder << fenPart << " ";
    }

    string fen = fen_builder.str();
    current_pos = Fens::parse(fen);

    if (hasMoves)
    {
        handle_moves(reader);
    }
}

void Uai::handle_position(stringstream& reader)
{
    std::string type;
    reader >> type;

    if (type == "startpos")
    {
        handle_startpos(reader);
    }
    else if (type == "moves")
    {
        current_pos = Fens::parse(initial_fen);
        handle_moves(reader);
    }
    else if (type == "fen")
    {
        handle_fen(reader);
    }
}

void Uai::handle_perft(stringstream& reader)
{
    int depth = 5;
    if (!reader.eof())
    {
        reader >> depth;
    }
    Perft::perft_deepen(current_pos, depth);
}

static void read_search_parameters(std::stringstream& reader, SearchParameters& parameters)
{
    while (!reader.eof())
    {
        std::string word;
        reader >> word;

        if (word == "wtime")
        {
            reader >> parameters.white_time;
        }
        else if (word == "btime")
        {
            reader >> parameters.black_time;
        }
        else if (word == "winc")
        {
            reader >> parameters.white_increment;
        }
        else if (word == "binc")
        {
            reader >> parameters.black_increment;
        }
        else if (word == "infinite")
        {
            parameters.infinite = true;
        }
    }
}


void Uai::handle_go(std::stringstream& reader)
{
    SearchParameters parameters;
    read_search_parameters(reader, parameters);
    search.run(current_pos, parameters);
}

void Uai::handle_input(const std::string& command)
{
    stringstream reader(command);
    while (!reader.eof())
    {
        string token;
        reader >> token;

        if(token == "uai")
        {
            handle_uai();
        }
        else if(token == "isready")
        {
            cout << "readyok" << endl;
        }
        else if(token == "uainewgame")
        {
            handle_uainewgame();
        }
        else if (token == "position")
        {
            handle_position(reader);
        }
        else if(token == "perft")
        {
            handle_perft(reader);
        }
        else if(token == "go")
        {
            handle_go(reader);
        }
        else if (token == "quit" || token == "exit")
        {
            exit(0);
        }

        // Extensions
        else if(token == "gi")
        {
            handle_input("go infinite");
        }
        else if(token == "datagen")
        {
            Datagen::run();
        }
    }
}

void Uai::run()
{
    current_pos = Fens::parse(initial_fen);
    search = Search();
    search.state.table.set_size(1024 * 1024 * 32);

    while (true)
    {
        string command = get_uci_line();
        handle_input(command);
    }
}
