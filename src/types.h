#ifndef TYPES_H
#define TYPES_H 1

// Just a "random stuff" header for now

#include <array>
#include <bit>
#include <cstdint>
#include <stdexcept>
#include <string>

#define DATAGEN 0
#if DATAGEN
constexpr bool do_datagen = true;
#else
constexpr bool do_datagen = false;
#endif

using Rank = uint8_t;
using File = uint8_t;
using Square = uint8_t;
constexpr Square SquareCount = 64;
template<class T>
using EachSquare = std::array<T, SquareCount>;
static constexpr Square no_square = 63;

static constexpr Square get_file(const Square square)
{
    return square % 8;
}

static constexpr Square get_rank(const Square square)
{
    return square / 8;
}

static constexpr Square get_square(const File file, const Rank rank)
{
    return rank * 8 + file;
}

static constexpr std::string to_square_str(const Square square)
{
    return std::string({ static_cast<char>('a' + get_file(square)), static_cast<char>('1' + get_rank(square)) });
}

static constexpr Square from_square_str(const std::string& square_str)
{
    return get_square(square_str[0] - 'a', square_str[1] - '1');
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
static constexpr Bitboard get_bitboard(const Square square)
{
    return 1ULL << square;
}

static constexpr Bitboard get_bitboard(const File file, const Rank rank)
{
    return get_bitboard(get_square(file, rank));
}

static constexpr Square lsb(const Bitboard bitboard)
{
    return static_cast<Square>(std::countr_zero(bitboard));
}

static Square pop_lsb(Bitboard& bitboard)
{
    const auto square = lsb(bitboard);
    bitboard &= bitboard - 1;
    return square;
}

static constexpr uint8_t pop_count(const Bitboard bitboard)
{
    return static_cast<uint8_t>(std::popcount(bitboard));
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
static const Fen initial_fen = "x5o/7/7/7/7/7/o5x x 0 1";

using MoveStr = std::string;

using Ply = int8_t;
constexpr Ply max_ply = 127;
template<class T>
using EachPly = std::array<T, max_ply>;

using Score = int16_t;
constexpr Score inf = 32000;
constexpr Score mate = 31000;

using MoveScore = int64_t;
using Time = int64_t;
using NodeCount = uint64_t;
using ZobristKey = uint64_t;
using ThreadCount = uint8_t;

#define EXCEPTIONS 1
static void do_throw(const std::string&& message)
{
#if EXCEPTIONS
    throw std::runtime_error(message);
#else
    cout << message << endl;
    exit(1);
#endif
}

#endif // !TYPES_H
