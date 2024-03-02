#include "fens.h"

#include "attacks.h"
#include "zobrist.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

static void check_file(const Fen& fen, const File file)
{
    if (file > 6)
    {
        do_throw("Malformed FEN " + fen);
    }
}

static constexpr Bitboard get_playable_zone(const PositionBase& pos)
{
    Bitboard playable_zone = pos.Bitboards[Pieces::White] | pos.Bitboards[Pieces::Black];
    while(true)
    {
        const Bitboard old_playable_zone = playable_zone;
        for(auto i = 0; i < 2; i++)
        {
            playable_zone |= north(playable_zone) | south(playable_zone);
            playable_zone |= west(playable_zone) | east(playable_zone);
        }
        playable_zone &= ~pos.Bitboards[Pieces::Wall];
        if(playable_zone == old_playable_zone)
        {
            break;
        }
    }
    return playable_zone;
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
        const auto square = get_square(file, rank);
        if(ch == '/')
        {
            if(file != 7)
            {
                do_throw("Malformed FEN " + fen);
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
                do_throw("Malformed FEN " + fen);
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
        do_throw("Malformed FEN " + fen);
    }

    pos.Bitboards[Pieces::Empty] = ~(pos.Bitboards[Pieces::White] | pos.Bitboards[Pieces::Black] | pos.Bitboards[Pieces::Wall]) & available_position;
    pos.Key = Zobrist.get_key(pos.Turn, pos.Squares);
    pos.playable = get_playable_zone(pos);
    return pos;
}

void Fens::serialize(const PositionBase& pos, ostream& ss)
{
    for (Rank flipped_rank = 0; flipped_rank < 7; flipped_rank++)
    {
        const Rank rank = 6 - flipped_rank;
        File file = 0;
        while (file < 7)
        {
            const auto square = get_square(file, rank);
            const auto piece = pos.Squares[square];
            if (piece == Pieces::Empty)
            {
                File empty_file = 0;
                while (file < 7 && pos.Squares[get_square(file, rank)] == Pieces::Empty)
                {
                    empty_file++;
                    file++;
                }
                ss << to_string(empty_file);
            }
            else if (piece == Pieces::White)
            {
                ss << string("o");
                file++;
            }
            else if (piece == Pieces::Black)
            {
                ss << string("x");
                file++;
            }
            else if (piece == Pieces::Wall)
            {
                ss << string("-");
                file++;
            }
        }
        if (rank != 0)
        {
            ss << string("/");
        }
    }

    ss << string(" ");
    ss << string(pos.Turn == Colors::White ? "o" : "x");
}

Fen Fens::serialize(const PositionBase& pos)
{
    stringstream ss;
    serialize(pos, ss);
    const Fen fen = ss.str();
    return fen;
}