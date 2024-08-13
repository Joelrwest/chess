#pragma once

#include "types.hpp"

template <Player player> struct BitBoardsConstants;

template <> struct BitBoardsConstants<Player::White>
{
    static constexpr auto STARTING_PAWNS_BIT_BOARD{rank_to_bit_board(Rank::R2)};
    static constexpr auto STARTING_KNIGHTS_BIT_BOARD{square_to_bit_board(Square::B1) | square_to_bit_board(Square::G1)};
    static constexpr auto STARTING_BISHOPS_BIT_BOARD{square_to_bit_board(Square::C1) | square_to_bit_board(Square::F1)};
    static constexpr auto STARTING_ROOKS_BIT_BOARD{square_to_bit_board(Square::A1) | square_to_bit_board(Square::H1)};
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
    static constexpr auto STARTING_KNIGHTS_BIT_BOARD{square_to_bit_board(Square::B8) | square_to_bit_board(Square::G8)};
    static constexpr auto STARTING_BISHOPS_BIT_BOARD{square_to_bit_board(Square::C8) | square_to_bit_board(Square::F8)};
    static constexpr auto STARTING_ROOKS_BIT_BOARD{square_to_bit_board(Square::A8) | square_to_bit_board(Square::H8)};
    static constexpr auto STARTING_QUEENS_BIT_BOARD{square_to_bit_board(Square::D8)};
    static constexpr auto STARTING_KING_BIT_BOARD{square_to_bit_board(Square::E8)};

    static constexpr auto PAWN_PROMOTION_RANK_BIT_BOARD{rank_to_bit_board(Rank::R1)};
    static constexpr auto PAWN_PUSH_DIRECTION{Direction::S};
    static constexpr auto PAWN_LEFT_CAPTURE_DIRECTION{Direction::SE};
    static constexpr auto PAWN_RIGHT_CAPTURE_DIRECTION{Direction::SW};

    static constexpr auto LEFT_FILE_BIT_BOARD{file_to_bit_board(File::FH)};
    static constexpr auto RIGHT_FILE_BIT_BOARD{file_to_bit_board(File::FA)};
};
