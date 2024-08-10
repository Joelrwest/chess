#pragma once

#include "types.hpp"

template <Player player> struct PlayerBitBoards
{
  public:
    PlayerBitBoards();

    std::int16_t total_piece_value();
    BitBoard occupied_bit_board();

    BitBoard pawns;
    BitBoard knights;
    BitBoard bishops;
    BitBoard rooks;
    BitBoard queens;
    BitBoard king;

  private:
    static consteval BitBoard get_starting_pawns();
    static consteval BitBoard get_starting_knights();
    static consteval BitBoard get_starting_bishops();
    static consteval BitBoard get_starting_rooks();
    static consteval BitBoard get_starting_queens();
    static consteval BitBoard get_starting_king();

    static constexpr auto PAWN_VALUE{1};
    static constexpr auto KNIGHT_VALUE{3};
    static constexpr auto BISHOP_VALUE{3};
    static constexpr auto ROOK_VALUE{5};
    static constexpr auto QUEEN_VALUE{9};

    inline static std::int16_t count_bits(BitBoard bit_board);
};

template <Player player>
PlayerBitBoards<player>::PlayerBitBoards()
    : pawns{get_starting_pawns()}, knights{get_starting_knights()}, bishops{get_starting_bishops()},
      rooks{get_starting_rooks()}, queens{get_starting_queens()}, king{get_starting_king()}
{
}

template <Player player> std::int16_t PlayerBitBoards<player>::total_piece_value()
{
    const auto pawns_value{PAWN_VALUE * count_bits(pawns)};
    const auto knights_value{KNIGHT_VALUE * count_bits(knights)};
    const auto bishops_value{BISHOP_VALUE * count_bits(bishops)};
    const auto rooks_value{ROOK_VALUE * count_bits(rooks)};
    const auto queens_value{QUEEN_VALUE * count_bits(queens)};

    return pawns_value + knights_value + bishops_value + rooks_value + queens_value;
}

template <Player player> BitBoard PlayerBitBoards<player>::occupied_bit_board()
{
    return pawns & knights & bishops & rooks & queens & king;
}

template <Player player> consteval BitBoard PlayerBitBoards<player>::get_starting_pawns()
{
    if (player == Player::White)
    {
        return rank_to_bit_board(Rank::R2);
    }
    else
    {
        return rank_to_bit_board(Rank::R7);
    }
}

template <Player player> consteval BitBoard PlayerBitBoards<player>::get_starting_knights()
{
    if (player == Player::White)
    {
        return square_to_bit_board(Square::B1) & square_to_bit_board(Square::G1);
    }
    else
    {
        return square_to_bit_board(Square::B8) & square_to_bit_board(Square::G8);
    }
}

template <Player player> consteval BitBoard PlayerBitBoards<player>::get_starting_bishops()
{
    if (player == Player::White)
    {
        return square_to_bit_board(Square::C1) & square_to_bit_board(Square::C1);
    }
    else
    {
        return square_to_bit_board(Square::C8) & square_to_bit_board(Square::C8);
    }
}

template <Player player> consteval BitBoard PlayerBitBoards<player>::get_starting_rooks()
{
    if (player == Player::White)
    {
        return square_to_bit_board(Square::A1) & square_to_bit_board(Square::H1);
    }
    else
    {
        return square_to_bit_board(Square::A8) & square_to_bit_board(Square::H8);
    }
}

template <Player player> consteval BitBoard PlayerBitBoards<player>::get_starting_queens()
{
    if (player == Player::White)
    {
        return square_to_bit_board(Square::D1);
    }
    else
    {
        return square_to_bit_board(Square::D8);
    }
}

template <Player player> consteval BitBoard PlayerBitBoards<player>::get_starting_king()
{
    if (player == Player::White)
    {
        return square_to_bit_board(Square::E1);
    }
    else
    {
        return square_to_bit_board(Square::E8);
    }
}

template <Player player> inline std::int16_t PlayerBitBoards<player>::count_bits(BitBoard bit_board)
{
    std::uint8_t count{0};
    while (bit_board > 0)
    {
        ++count;
        bit_board &= bit_board - 1;
    }

    return count;
}
