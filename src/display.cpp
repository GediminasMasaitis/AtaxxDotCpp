#include "display.h"

#include "fens.h"
#include "evaluation.h"

#include <iostream>
#include <sstream>

using namespace std;

void Display::display_position(const Position& pos)
{
    stringstream ss;

    const std::string border = "+---+---+---+---+---+---+---+\n";
    for (auto rank = 6; rank >= 0; rank--)
    {
        ss << border;
        for (auto file = 0; file < 7; file++)
        {
            const Square sq = get_square(file, rank);
            const Piece piece = pos.Squares[sq];
            char piece_char;
            switch (piece)
            {
            case Pieces::White:
                piece_char = 'o';
                break;
            case Pieces::Black:
                piece_char = 'x';
                break;
            case Pieces::Wall:
                piece_char = '=';
                break;
            case Pieces::Empty:
                piece_char = ' ';
                break;
            }
            ss << "| " << piece_char << " ";
        }
        const string rank_str = to_string(rank + 1);
        ss << "| " << rank_str << endl;
    }
    ss << border << "";
    for (auto file = 0; file < 7; file++)
    {
        ss << "  " << static_cast<char>('A' + file) << " ";
    }
    ss << endl << endl;

    const auto turn_ch = pos.Turn == Pieces::White ? 'o' : 'x';
    ss << "Turn: " << turn_ch << endl;

    const auto fen = Fens::serialize(pos);
    ss << "FEN: " << fen << endl;

    const auto eval = Evaluation::evaluate(pos);
    ss << "Eval: " << eval << endl;

    ss << endl;
    const auto str = ss.str();
    cout << str;
}
