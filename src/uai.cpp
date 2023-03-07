#include "uai.h"
#include "fens.h"
#include "position.h"
#include "perft.h"

#include <iostream>
#include <sstream>

using namespace std;

static const std::string initial_fen = "x5o/7/7/7/7/7/o5x x 0 1";

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
            int depth = 5;
            if(!reader.eof())
            {
                reader >> depth;
            }
            Perft::perft_deepen(current_pos, depth);
        }
        else if (token == "quit" || token == "exit")
        {
            exit(0);
        }
    }
}

void Uai::run()
{
    current_pos = Fens::parse(initial_fen);
    while (true)
    {
        string command = get_uci_line();
        handle_input(command);
    }
}
