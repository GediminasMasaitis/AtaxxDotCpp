#include "fens.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

static void check_file(const Fen& fen, const File file)
{
    if (file > 6)
    {
        cout << "Malformed FEN " << fen << endl;
        exit(-1);
    }
}

Position Fens::parse(const Fen& fen)
{
    Position pos;
    pos.Bitboards.fill(0);
    pos.Squares.fill(Pieces::Empty);

    File file = 0;
    Rank flipped_rank = 0;

    stringstream ss(fen);
    while (true)
    {
        const Rank rank = 6 - flipped_rank;
        char ch;
        ss.get(ch);
        const auto square = GetSquare(file, rank);
        if(ch == '/')
        {
            if(file != 7)
            {
                cout << "Malformed FEN " << fen << endl;
                exit(-1);
            }
            file = 0;
            flipped_rank++;
        }
        else if(ch == 'o')
        {
            check_file(fen, file);
            pos.Bitboards[Pieces::White] |= get_bitboard(file, rank);
            pos.Squares[square] = Pieces::White;
            file++;

        }
        else if (ch == 'x')
        {
            check_file(fen, file);
            pos.Bitboards[Pieces::Black] |= get_bitboard(file, rank);
            pos.Squares[square] = Pieces::Black;
            file++;
        }
        else if (ch == '-')
        {
            check_file(fen, file);
            pos.Bitboards[Pieces::Wall] |= get_bitboard(file, rank);
            pos.Squares[square] = Pieces::Wall;
            file++;
        }
        else if(ch >= '1' && ch <= '7')
        {
            check_file(fen, file);
            const File input_file = ch - 0x30;
            file += input_file;
        }
        else if(ch == ' ')
        {
            if(rank != 0 || file != 7)
            {
                cout << "Malformed FEN " << fen << endl;
                exit(-1);
            }
            break;
        }
    }

    char pos_ch;
    ss.get(pos_ch);
    if(pos_ch == 'o')
    {
        pos.Turn = Colors::White;
    }
    else if(pos_ch == 'x')
    {
        pos.Turn = Colors::Black;
    }
    else
    {
        cout << "Malformed FEN " << fen << endl;
        exit(-1);
    }

    pos.Bitboards[Pieces::Empty] = ~(pos.Bitboards[Pieces::White] | pos.Bitboards[Pieces::Black] | pos.Bitboards[Pieces::Wall]) & available_position;
    return pos;
}

Fen Fens::serialize(const Position& pos)
{
    stringstream ss;

    for(Rank flipped_rank = 0; flipped_rank < 7; flipped_rank++)
    {
        const Rank rank = 6 - flipped_rank;
        File file = 0;
        while(file < 7)
        {
            const auto square = GetSquare(file, rank);
            const auto piece = pos.Squares[square];
            if(piece == Pieces::Empty)
            {
                File empty_file = 0;
                while(file < 7 && pos.Squares[GetSquare(file, rank)] == Pieces::Empty)
                {
                    empty_file++;
                    file++;
                }
                ss << to_string(empty_file);
            }
            else if(piece == Pieces::White)
            {
                ss << string("o");
                file++;
            }
            else if(piece == Pieces::Black)
            {
                ss << string("x");
                file++;
            }
            else if(piece == Pieces::Wall)
            {
                ss << string("-");
                file++;
            }
        }
        if(rank != 0)
        {
            ss << string("/");
        }
    }

    ss << string(" ");
    ss << string(pos.Turn == Colors::White ? "o" : "x");

    const Fen fen = ss.str();
    return fen;
}
