#include "display.h"

#include "fens.h"
#include "evaluation.h"
#include "search.h"

#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>

using namespace std;

Score run_search(Position& pos, Square removed_sq, const SearchParameters parameters)
{
    if(removed_sq == no_square)
    {
        cout << "Initial search... ";
    }
    else
    {
        cout << "Searching without " << to_square_str(removed_sq) << "... ";
    }
    cout.flush();
    Search search;
    search.state.table.set_size_from_options();

    const auto result = search.run(pos, parameters);
    const auto pov_score = pos.Turn == Colors::White ? result.score : -result.score;
    cout << "score cp " << pov_score << " pv ";
    for (int i = 0; i < search.state.threads[0].saved_pv.length; ++i)
    {
        const Move& pv_move = search.state.threads[0].saved_pv.moves[i];
        cout << pv_move.to_move_str() << " ";
    }
    cout << endl;
    return result.score;
}

void Display::display_position(Position& pos, optional<SearchParameters> search_parameters)
{
    const bool do_search = search_parameters.has_value();
    Score score;

    if(do_search)
    {
        score = run_search(pos, no_square, search_parameters.value());
    }
    else
    {
        score = Evaluation::evaluate(pos);
    }

    if(pos.Turn == Colors::Black)
    {
        score = -score;
    }

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
                npos.Bitboards[Pieces::White] &= ~get_bitboard(sq);
                npos.Bitboards[Pieces::Black] &= ~get_bitboard(sq);
                npos.Bitboards[Pieces::Empty] |= get_bitboard(sq);
                npos.accumulators_unset(sq, piece);
                Score score_without_piece;
                if (do_search)
                {
                    score_without_piece = run_search(npos, sq, search_parameters.value());
                }
                else
                {
                    score_without_piece = Evaluation::evaluate(npos);
                }
                if(pos.Turn == Colors::Black)
                {
                    score_without_piece = -score_without_piece;
                }
                const auto diff = score - score_without_piece;

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
    if(do_search)
    {
        ss << "Search: " << dec << score << endl;
    }
    else
    {
        ss << "Eval: " << dec << score << endl;
    }

    ss << endl;
    const auto str = ss.str();
    cout << str;
}
