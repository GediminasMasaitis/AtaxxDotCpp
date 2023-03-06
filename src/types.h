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

using Fen = std::string;

#endif // !TYPES_H
