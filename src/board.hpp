#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace board
{
class Board
{
  public:
    Board();

  private:
    using BitMap = std::uint64_t;

    static constexpr BitMap STARTING_WHITE_PAWNS{0x000000000000FF00};
    static constexpr BitMap STARTING_WHITE_KNIGHTS{0x0000000000000042};
    static constexpr BitMap STARTING_WHITE_BISHOPS{0x0000000000000024};
    static constexpr BitMap STARTING_WHITE_ROOKS{0x0000000000000081};
    static constexpr BitMap STARTING_WHITE_QUEENS{0x0000000000000008};
    static constexpr BitMap STARTING_WHITE_KINGS{0x0000000000000010};

    static constexpr BitMap STARTING_BLACK_PAWNS{0x00FF000000000000};
    static constexpr BitMap STARTING_BLACK_KNIGHTS{0x4200000000000000};
    static constexpr BitMap STARTING_BLACK_BISHOPS{0x2400000000000000};
    static constexpr BitMap STARTING_BLACK_ROOKS{0x8100000000000000};
    static constexpr BitMap STARTING_BLACK_QUEENS{0x0800000000000000};
    static constexpr BitMap STARTING_BLACK_KINGS{0x1000000000000000};

    BitMap white_pawns;
    BitMap white_knights;
    BitMap white_bishops;
    BitMap white_rooks;
    BitMap white_queens;
    BitMap white_kings;

    BitMap black_pawns;
    BitMap black_knights;
    BitMap black_bishops;
    BitMap black_rooks;
    BitMap black_queens;
    BitMap black_kings;
};
} // namespace board
