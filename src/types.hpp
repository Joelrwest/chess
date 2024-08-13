#pragma once

#include <array>
#include <concepts>
#include <cstdint>
#include <iostream>

static constexpr auto BOARD_WIDTH{8};
static constexpr auto BOARD_SQUARES{BOARD_WIDTH * BOARD_WIDTH};

template <typename T> using Lookup = std::array<T, BOARD_SQUARES>;
using BitBoard = std::uint64_t;
using Evaluation = std::int16_t;
using Magic = std::uint64_t;
using RankUnderlying = std::uint8_t;
using FileUnderlying = std::uint8_t;
using SquareUnderlying = std::uint8_t;
using DirectionUnderlying = std::int8_t;

enum Rank : RankUnderlying
{
    R1 = 0u,
    R2 = 1u,
    R3 = 2u,
    R4 = 3u,
    R5 = 4u,
    R6 = 5u,
    R7 = 6u,
    R8 = 7u,
};

enum File : FileUnderlying
{
    FA = 0u,
    FB = 1u,
    FC = 2u,
    FD = 3u,
    FE = 4u,
    FF = 5u,
    FG = 6u,
    FH = 7u,
};

// clang-format off
enum Square : SquareUnderlying
{
    A1 = 0u , B1 = 1u , C1 = 2u , D1 = 3u , E1 = 4u , F1 = 5u , G1 = 6u , H1 = 7u ,
    A2 = 8u , B2 = 9u , C2 = 10u, D2 = 11u, E2 = 12u, F2 = 13u, G2 = 14u, H2 = 15u,
    A3 = 16u, B3 = 17u, C3 = 18u, D3 = 19u, E3 = 20u, F3 = 21u, G3 = 22u, H3 = 23u,
    A4 = 24u, B4 = 25u, C4 = 26u, D4 = 27u, E4 = 28u, F4 = 29u, G4 = 30u, H4 = 31u,
    A5 = 32u, B5 = 33u, C5 = 34u, D5 = 35u, E5 = 36u, F5 = 37u, G5 = 38u, H5 = 39u,
    A6 = 40u, B6 = 41u, C6 = 42u, D6 = 43u, E6 = 44u, F6 = 45u, G6 = 46u, H6 = 47u,
    A7 = 48u, B7 = 49u, C7 = 50u, D7 = 51u, E7 = 52u, F7 = 53u, G7 = 54u, H7 = 55u,
    A8 = 56u, B8 = 57u, C8 = 58u, D8 = 59u, E8 = 60u, F8 = 61u, G8 = 62u, H8 = 63u,
};
// clang-format on

enum Direction : DirectionUnderlying
{
    N = BOARD_WIDTH,
    S = -N,
    E = 1,
    W = -E,

    NE = N + E,
    SE = S + E,
    SW = S + W,
    NW = N + W,

    /* For knight only */
    NNE = N + NE,
    ENE = E + NE,
    ESE = E + SE,
    SSE = S + SE,
    SSW = S + SW,
    WSW = W + SW,
    WNW = W + NW,
    NNW = N + NW,
};

enum Player : std::uint8_t
{
    White,
    Black,
};

enum Piece : std::uint8_t
{
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
};

inline constexpr BitBoard rank_to_bit_board(Rank rank)
{
    return BitBoard{0xFF} << (BOARD_WIDTH * rank);
}

inline constexpr BitBoard file_to_bit_board(File file)
{
    return BitBoard{0x0101010101010101} << file;
}

inline constexpr BitBoard square_to_bit_board(Square square)
{
    return BitBoard{1} << square;
}

template <Direction direction> inline constexpr BitBoard direction_shift(BitBoard bit_board)
{
    if constexpr (direction > 0)
    {
        return bit_board << direction;
    }
    else if constexpr (direction < 0)
    {
        return bit_board >> -direction;
    }

    throw std::logic_error{"No point in shifting by 0 bits"};
}

template <Direction direction, std::unsigned_integral T>
inline constexpr BitBoard direction_shift(BitBoard bit_board, T magnitude)
{
    if constexpr (direction > 0)
    {
        return bit_board << (direction * magnitude);
    }
    else if constexpr (direction < 0)
    {
        return bit_board >> -(direction * magnitude);
    }

    throw std::logic_error{"No point in shifting by 0 bits"};
}

template <std::integral T> inline constexpr BitBoard diagonal_shift(BitBoard diagonal, T shift)
{
    if (shift > 0)
    {
        diagonal = direction_shift<Direction::N>(diagonal, static_cast<std::uint8_t>(shift));
    }
    else if (shift < 0)
    {
        diagonal = direction_shift<Direction::S>(diagonal, static_cast<std::uint8_t>(-shift));
    }

    return diagonal;
}

inline constexpr Rank square_to_rank(Square square)
{
    return static_cast<Rank>(square / BOARD_WIDTH);
}

inline constexpr File square_to_file(Square square)
{
    return static_cast<File>(square % BOARD_WIDTH);
}

std::ostream &operator<<(std::ostream &os, Rank rank);
std::ostream &operator<<(std::ostream &os, File file);
std::ostream &operator<<(std::ostream &os, Square square);
std::ostream &operator<<(std::ostream &os, Direction direction);
std::ostream &operator<<(std::ostream &os, Player player);
