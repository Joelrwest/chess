#pragma once

#include "move.hpp"
#include "types.hpp"

#include <bit>
#include <utility>
#include <vector>

#include <iostream>

template <Player player> struct BitBoardsConstants;

class Position;

template <> struct BitBoardsConstants<Player::White>
{
    static constexpr auto STARTING_PAWNS_BIT_BOARD{rank_to_bit_board(Rank::R2)};
    static constexpr auto STARTING_KNIGHTS_BIT_BOARD{square_to_bit_board(Square::B1) & square_to_bit_board(Square::G1)};
    static constexpr auto STARTING_BISHOPS_BIT_BOARD{square_to_bit_board(Square::C1) & square_to_bit_board(Square::C1)};
    static constexpr auto STARTING_ROOKS_BIT_BOARD{square_to_bit_board(Square::A1) & square_to_bit_board(Square::H1)};
    static constexpr auto STARTING_QUEENS_BIT_BOARD{square_to_bit_board(Square::D1)};
    static constexpr auto STARTING_KING_BIT_BOARD{square_to_bit_board(Square::E1)};

    static constexpr auto PAWN_PROMOTION_RANK_BIT_BOARD{rank_to_bit_board(Rank::R8)};
    static constexpr auto PAWN_PUSH_DIRECTION{Direction::N};
    static constexpr auto PAWN_LEFT_CAPTURE_DIRECTION{Direction::NW};
    static constexpr auto PAWN_RIGHT_CAPTURE_DIRECTION{Direction::NE};

    static constexpr auto LEFT_FILE_BIT_BOARD{file_to_bit_board(File::FA)};
    static constexpr auto RIGHT_FILE_BIT_BOARD{file_to_bit_board(File::FH)};
};

template <> struct BitBoardsConstants<Player::Black>
{
    static constexpr auto STARTING_PAWNS_BIT_BOARD{rank_to_bit_board(Rank::R7)};
    static constexpr auto STARTING_KNIGHTS_BIT_BOARD{square_to_bit_board(Square::B8) & square_to_bit_board(Square::G8)};
    static constexpr auto STARTING_BISHOPS_BIT_BOARD{square_to_bit_board(Square::C8) & square_to_bit_board(Square::C8)};
    static constexpr auto STARTING_ROOKS_BIT_BOARD{square_to_bit_board(Square::A8) & square_to_bit_board(Square::H8)};
    static constexpr auto STARTING_QUEENS_BIT_BOARD{square_to_bit_board(Square::D8)};
    static constexpr auto STARTING_KING_BIT_BOARD{square_to_bit_board(Square::E8)};

    static constexpr auto PAWN_PROMOTION_RANK_BIT_BOARD{rank_to_bit_board(Rank::R1)};
    static constexpr auto PAWN_PUSH_DIRECTION{Direction::S};
    static constexpr auto PAWN_LEFT_CAPTURE_DIRECTION{Direction::SE};
    static constexpr auto PAWN_RIGHT_CAPTURE_DIRECTION{Direction::SW};

    static constexpr auto LEFT_FILE_BIT_BOARD{file_to_bit_board(File::FH)};
    static constexpr auto RIGHT_FILE_BIT_BOARD{file_to_bit_board(File::FA)};
};

template <Player player> class BitBoards
{
  public:
    BitBoards();

    Evaluation get_total_piece_value() const;
    BitBoard get_occupied_bit_board() const;

    std::vector<Move> get_moves(BitBoard opponent_occupied_bit_board, BitBoard opponent_attacking_bit_board) const;

  private:
    using Constants = BitBoardsConstants<player>;

    static constexpr Evaluation PAWN_VALUE{1};
    static constexpr Evaluation KNIGHT_VALUE{3};
    static constexpr Evaluation BISHOP_VALUE{3};
    static constexpr Evaluation ROOK_VALUE{5};
    static constexpr Evaluation QUEEN_VALUE{9};

    void add_pawn_moves(std::vector<Move> &moves, BitBoard occupied_bit_board,
                        BitBoard opponent_occupied_bit_board) const;
    void add_knight_moves(std::vector<Move> &moves, BitBoard occupied_bit_board,
                          BitBoard opponent_occupied_bit_board) const;
    void add_bishop_moves(std::vector<Move> &moves, BitBoard occupied_bit_board,
                          BitBoard opponent_occupied_bit_board) const;
    void add_rook_moves(std::vector<Move> &moves, BitBoard occupied_bit_board,
                        BitBoard opponent_occupied_bit_board) const;
    void add_queen_moves(std::vector<Move> &moves, BitBoard occupied_bit_board,
                         BitBoard opponent_occupied_bit_board) const;
    void add_king_moves(std::vector<Move> &moves, BitBoard occupied_bit_board, BitBoard opponent_occupied_bit_board,
                        BitBoard opponent_attacking_bit_board) const;

    inline static std::uint8_t count_bits(BitBoard bit_board);
    inline static Square ls1b(BitBoard bit_board);

    BitBoard pawns;
    BitBoard knights;
    BitBoard bishops;
    BitBoard rooks;
    BitBoard queens;
    BitBoard king;

    friend std::ostream &operator<<(std::ostream &os, Position position);
};

template <Player player>
BitBoards<player>::BitBoards()
    : pawns{Constants::STARTING_PAWNS_BIT_BOARD}, knights{Constants::STARTING_KNIGHTS_BIT_BOARD},
      bishops{Constants::STARTING_BISHOPS_BIT_BOARD}, rooks{Constants::STARTING_ROOKS_BIT_BOARD},
      queens{Constants::STARTING_QUEENS_BIT_BOARD}, king{Constants::STARTING_KING_BIT_BOARD}
{
}

template <Player player> std::int16_t BitBoards<player>::get_total_piece_value() const
{
    const auto pawns_value{PAWN_VALUE * count_bits(pawns)};
    const auto knights_value{KNIGHT_VALUE * count_bits(knights)};
    const auto bishops_value{BISHOP_VALUE * count_bits(bishops)};
    const auto rooks_value{ROOK_VALUE * count_bits(rooks)};
    const auto queens_value{QUEEN_VALUE * count_bits(queens)};

    return pawns_value + knights_value + bishops_value + rooks_value + queens_value;
}

template <Player player> BitBoard BitBoards<player>::get_occupied_bit_board() const
{
    return pawns & knights & bishops & rooks & queens & king;
}

template <Player player>
std::vector<Move> BitBoards<player>::get_moves(BitBoard opponent_occupied_bit_board,
                                               BitBoard opponent_attacking_bit_board) const
{
    std::vector<Move> moves{};
    const auto occupied{get_occupied_bit_board() & opponent_occupied_bit_board};

    add_pawn_moves(moves, occupied, opponent_occupied_bit_board);
    add_knight_moves(moves, occupied, opponent_occupied_bit_board);
    add_bishop_moves(moves, occupied, opponent_occupied_bit_board);
    add_rook_moves(moves, occupied, opponent_occupied_bit_board);
    add_queen_moves(moves, occupied, opponent_occupied_bit_board);
    add_king_moves(moves, occupied, opponent_occupied_bit_board, opponent_attacking_bit_board);

    return moves;
}

template <Player player>
void BitBoards<player>::add_pawn_moves(std::vector<Move> &moves, BitBoard occupied_bit_board,
                                       BitBoard opponent_occupied_bit_board) const
{
    /*
    Push moves
    */
    auto single_push_bit_board{direction_shift<Constants::PAWN_PUSH_DIRECTION>(pawns) & ~occupied_bit_board};
    auto double_push_bit_board{direction_shift<Constants::PAWN_PUSH_DIRECTION>(single_push_bit_board) &
                               ~occupied_bit_board};

    while (single_push_bit_board)
    {
        const auto to{ls1b(single_push_bit_board)};
        const auto from{static_cast<Square>(to - Constants::PAWN_PUSH_DIRECTION)};
        moves.emplace_back(from, to);
        single_push_bit_board <<= (to + 1);
    }

    while (double_push_bit_board)
    {
        const auto to{ls1b(double_push_bit_board)};
        const auto from{static_cast<Square>(to - 2 * Constants::PAWN_PUSH_DIRECTION)};
        moves.emplace_back(from, to);
        double_push_bit_board <<= (to + 1);
    }

    /*
    Capture left
    */
    auto left_capture_bit_board{
        direction_shift<Constants::PAWN_LEFT_CAPTURE_DIRECTION>(pawns & ~Constants::LEFT_FILE_BIT_BOARD) &
        opponent_occupied_bit_board};
    while (left_capture_bit_board)
    {
        const auto to{ls1b(left_capture_bit_board)};
        const auto from{static_cast<Square>(to - Constants::PAWN_LEFT_CAPTURE_DIRECTION)};
        moves.emplace_back(from, to);
        left_capture_bit_board <<= (to + 1);
    }

    /*
    Capture right
    */
    auto right_capture_bit_board{
        direction_shift<Constants::PAWN_RIGHT_CAPTURE_DIRECTION>(pawns & ~Constants::RIGHT_FILE_BIT_BOARD) &
        opponent_occupied_bit_board};
    while (right_capture_bit_board)
    {
        const auto to{ls1b(right_capture_bit_board)};
        const auto from{static_cast<Square>(to - Constants::PAWN_RIGHT_CAPTURE_DIRECTION)};
        moves.emplace_back(from, to);
        right_capture_bit_board <<= (to + 1);
    }
}

template <Player player>
void BitBoards<player>::add_knight_moves(std::vector<Move> &moves, BitBoard occupied_bit_board,
                                         BitBoard opponent_occupied_bit_board) const
{
    (void)moves;
    (void)occupied_bit_board;
    (void)opponent_occupied_bit_board;
}

template <Player player>
void BitBoards<player>::add_bishop_moves(std::vector<Move> &moves, BitBoard occupied_bit_board,
                                         BitBoard opponent_occupied_bit_board) const
{
    (void)moves;
    (void)occupied_bit_board;
    (void)opponent_occupied_bit_board;
}

template <Player player>
void BitBoards<player>::add_rook_moves(std::vector<Move> &moves, BitBoard occupied_bit_board,
                                       BitBoard opponent_occupied_bit_board) const
{
    (void)moves;
    (void)occupied_bit_board;
    (void)opponent_occupied_bit_board;
}

template <Player player>
void BitBoards<player>::add_queen_moves(std::vector<Move> &moves, BitBoard occupied_bit_board,
                                        BitBoard opponent_occupied_bit_board) const
{
    (void)moves;
    (void)occupied_bit_board;
    (void)opponent_occupied_bit_board;
}

template <Player player>
void BitBoards<player>::add_king_moves(std::vector<Move> &moves, BitBoard occupied_bit_board,
                                       BitBoard opponent_occupied_bit_board,
                                       BitBoard opponent_attacking_bit_board) const
{
    (void)moves;
    (void)occupied_bit_board;
    (void)opponent_occupied_bit_board;
    (void)opponent_attacking_bit_board;
}

template <Player player> inline std::uint8_t BitBoards<player>::count_bits(BitBoard bit_board)
{
    return std::popcount(bit_board);
}

template <Player player> inline Square BitBoards<player>::ls1b(BitBoard bit_board)
{
    return static_cast<Square>(std::countr_zero(bit_board));
}
