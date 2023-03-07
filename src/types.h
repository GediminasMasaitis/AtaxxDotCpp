#ifndef TYPES_H
#define TYPES_H 1

// Just a "random stuff" header for now

#include <array>
#include <cstdint>
#include <string>

using Rank = uint8_t;
using File = uint8_t;
using Square = uint8_t;
constexpr Square SquareCount = 64;
template<class T>
using EachSquare = std::array<T, SquareCount>;
static constexpr Square no_square = 64;

static constexpr Square GetFile(const Square square)
{
    return square % 8;
}

static constexpr Square GetRank(const Square square)
{
    return square / 8;
}

static constexpr Square GetSquare(const File file, const Rank rank)
{
    return rank * 8 + file;
}

using Color = uint8_t;
struct Colors
{
    static constexpr Color White = 0;
    static constexpr Color Black = 1;
    static constexpr Color Count = 2;
};
template<class T>
using EachColor = std::array<T, Colors::Count>;

using Piece = uint8_t;
struct Pieces
{
    static constexpr Piece White = 0;
    static constexpr Piece Black = 1;
    static constexpr Piece Wall = 2;
    static constexpr Piece Empty = 3;
    static constexpr Piece Count = 4;
};
template<class T>
using EachPiece = std::array<T, Pieces::Count>;

using Bitboard = uint64_t;
static constexpr Bitboard GetBitboard(const Square square)
{
    return 1ULL << square;
}

static constexpr Bitboard GetBitboard(const File file, const Rank rank)
{
    return GetBitboard(GetSquare(file, rank));
}

static constexpr Bitboard available_position = 0x007F7F7F7F7F7F7FULL;

static constexpr std::array<Bitboard, 8> Files =
{
    0x101010101010101ULL,
    0x202020202020202ULL,
    0x404040404040404ULL,
    0x808080808080808ULL,
    0x1010101010101010ULL,
    0x2020202020202020ULL,
    0x4040404040404040ULL,
    0x8080808080808080ULL
};

static constexpr std::array<Bitboard, 8> Ranks =
{
    0xFFULL,
    0xFF00ULL,
    0xFF0000ULL,
    0xFF000000ULL,
    0xFF00000000ULL,
    0xFF0000000000ULL,
    0xFF000000000000ULL,
    0xFF00000000000000ULL
};

using Fen = std::string;

struct Move
{
    Color Turn;
    Square From;
    Square To;

    constexpr Move(const Color turn, const Square from, const Square to)
        : Turn(turn), From(from), To(to)
    {
    }

    constexpr Move() : Move(Colors::White, 0, 0)
    {
        
    }
};
static constexpr Move no_move = Move(Colors::White, 0, 0);

using MoveCount = uint16_t;
constexpr MoveCount MaxMoveCount = 1024;
template<class T>
using EachMove = std::array<T, MaxMoveCount>;
using MoveArray = EachMove<Move>;

#endif // !TYPES_H
