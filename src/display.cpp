#include "display.h"

#include "fens.h"
#include "evaluation.h"

#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

void Display::display_position(const Position& pos)
{
    const auto eval = Evaluation::evaluate_from_pov(pos);

    stringstream ss;
    const std::string border = "+-------+-------+-------+-------+-------+-------+-------+\n";
    const std::string blank =  "|       |       |       |       |       |       |       |\n";
    for (auto rank = 6; rank >= 0; rank--)
    {
        ss << border;
        ss << blank;
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
                piece_char = '#';
                break;
            case Pieces::Empty:
                piece_char = ' ';
                break;
            }

            ss << "|   " << piece_char << "   ";
        }

        const string rank_str = to_string(rank + 1);
        ss << "| " << rank_str << endl;

        for (auto file = 0; file < 7; file++)
        {
            const Square sq = get_square(file, rank);
            const Piece piece = pos.Squares[sq];
            if(piece != Pieces::Empty)
            {
                Position npos = pos;
                npos.Squares[sq] = Pieces::Empty;
                npos.Bitboards[Colors::White] &= ~get_bitboard(sq);
                npos.Bitboards[Colors::Black] &= ~get_bitboard(sq);
                npos.accumulators_unset(sq, piece);
                const auto eval_without_piece = Evaluation::evaluate_from_pov(npos);
                const auto diff = eval - eval_without_piece;

                const std::string diffStr = std::to_string(diff);
                const auto padLeft = static_cast<int32_t>((7 - diffStr.size()) / 2);
                const auto padRight = static_cast<int32_t>(7 - diffStr.size() - padLeft);
                ss << "|";
                for (int32_t i = 0; i < padLeft; i++)
                {
                    ss << " ";
                }
                ss << diffStr;
                for (int32_t i = 0; i < padRight; i++)
                {
                    ss << " ";
                }
            }
            else
            {
                ss << "|       ";
            }
        }
        ss << "| " << endl;
    }
    ss << border << "";
    for (auto file = 0; file < 7; file++)
    {
        ss << "    " << static_cast<char>('A' + file) << "   ";
    }
    ss << endl << endl;

    const auto turn_ch = pos.Turn == Pieces::White ? 'o' : 'x';
    const auto fen = Fens::serialize(pos);
    ss << "Turn: " << turn_ch << endl;
    ss << "FEN: " << fen << endl;
    ss << endl;
    ss << "White: 0x" << hex << setfill('0') << setw(16) << pos.Bitboards[Pieces::White] << endl;
    ss << "Black: 0x" << hex << setfill('0') << setw(16) << pos.Bitboards[Pieces::Black] << endl;
    ss << "Wall:  0x" << hex << setfill('0') << setw(16) << pos.Bitboards[Pieces::Wall] << endl;
    ss << "Empty: 0x" << hex << setfill('0') << setw(16) << pos.Bitboards[Pieces::Empty] << endl;
    ss << "Play:  0x" << hex << setfill('0') << setw(16) << pos.playable << endl;
    ss << endl;
    ss << "Eval: " << dec << eval << endl;

    ss << endl;
    const auto str = ss.str();
    cout << str;
}
