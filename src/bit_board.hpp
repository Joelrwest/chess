#pragma once

#include "fen_parser.hpp"
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

template <Player player> class BitBoards
{
  public:
    BitBoards();
    BitBoards(const FenParser &fen_parser);

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

    template <typename F>
    static inline void serialise_bit_board(std::vector<Move> &moves, BitBoard bit_board, F from_function);
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

template <Player player>
BitBoards<player>::BitBoards(const FenParser &fen_parser)
    : pawns{0}, knights{0}, bishops{0}, rooks{0}, queens{0}, king{0}
{

    const auto &board_array{fen_parser.get_board_array()};
    for (Square square{0}; square < BOARD_SQUARES; ++square)
    {
        const auto &entry{board_array.at(square)};
        if (entry.has_value() && entry->first == player)
        {
            const auto mask{square_to_bit_board(square)};
            switch (entry->second)
            {
            case Piece::Pawn:
                pawns |= mask;
                break;
            case Piece::Knight:
                knights |= mask;
                break;
            case Piece::Bishop:
                bishops |= mask;
                break;
            case Piece::Rook:
                rooks |= mask;
                break;
            case Piece::Queen:
                queens |= mask;
                break;
            case Piece::King:
                king |= mask;
                break;
            }
        }
    }
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
    return pawns | knights | bishops | rooks | queens | king;
}

template <Player player>
std::vector<Move> BitBoards<player>::get_moves(BitBoard opponent_occupied_bit_board,
                                               BitBoard opponent_attacking_bit_board) const
{
    std::vector<Move> moves{};
    const auto occupied_bit_board{get_occupied_bit_board() | opponent_occupied_bit_board};

    add_pawn_moves(moves, occupied_bit_board, opponent_occupied_bit_board);
    add_knight_moves(moves, occupied_bit_board, opponent_occupied_bit_board);
    add_bishop_moves(moves, occupied_bit_board, opponent_occupied_bit_board);
    add_rook_moves(moves, occupied_bit_board, opponent_occupied_bit_board);
    add_queen_moves(moves, occupied_bit_board, opponent_occupied_bit_board);
    add_king_moves(moves, occupied_bit_board, opponent_occupied_bit_board, opponent_attacking_bit_board);

    return moves;
}

template <Player player>
void BitBoards<player>::add_pawn_moves(std::vector<Move> &moves, BitBoard occupied_bit_board,
                                       BitBoard opponent_occupied_bit_board) const
{
    /*
    Push moves
    */
    auto single_push_bit_board{direction_shift<Constants::PAWN_PUSH_DIRECTION>(pawns)};
    single_push_bit_board &= ~occupied_bit_board;
    serialise_bit_board(moves, single_push_bit_board, [](auto to) { return to - Constants::PAWN_PUSH_DIRECTION; });

    static constexpr auto SINGLE_PUSH_RANK{
        direction_shift<Constants::PAWN_PUSH_DIRECTION>(Constants::STARTING_PAWNS_BIT_BOARD)};
    auto double_push_bit_board{
        direction_shift<Constants::PAWN_PUSH_DIRECTION>(single_push_bit_board & SINGLE_PUSH_RANK)};
    double_push_bit_board &= ~occupied_bit_board;
    serialise_bit_board(moves, double_push_bit_board, [](auto to) { return to - 2 * Constants::PAWN_PUSH_DIRECTION; });

    /*
    Capture left
    */
    auto left_capture_bit_board{
        direction_shift<Constants::PAWN_LEFT_CAPTURE_DIRECTION>(pawns & ~Constants::LEFT_FILE_BIT_BOARD)};
    left_capture_bit_board &= opponent_occupied_bit_board;
    serialise_bit_board(moves, left_capture_bit_board,
                        [](auto to) { return to - Constants::PAWN_LEFT_CAPTURE_DIRECTION; });

    /*
    Capture right
    */
    auto right_capture_bit_board{
        direction_shift<Constants::PAWN_RIGHT_CAPTURE_DIRECTION>(pawns & ~Constants::RIGHT_FILE_BIT_BOARD)};
    right_capture_bit_board &= opponent_occupied_bit_board;
    serialise_bit_board(moves, right_capture_bit_board,
                        [](auto to) { return to - Constants::PAWN_RIGHT_CAPTURE_DIRECTION; });
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

template <Player player>
template <typename F>
inline void BitBoards<player>::serialise_bit_board(std::vector<Move> &moves, BitBoard bit_board, F from_function)
{
    for (Square to{0}; bit_board; ++to)
    {
        const auto lsb{ls1b(bit_board)};
        to += lsb;
        const auto from{static_cast<Square>(from_function(to))};
        moves.emplace_back(from, to);
        bit_board >>= (lsb + 1);
    }
}

template <Player player> inline std::uint8_t BitBoards<player>::count_bits(BitBoard bit_board)
{
    return std::popcount(bit_board);
}

template <Player player> inline Square BitBoards<player>::ls1b(BitBoard bit_board)
{
    return static_cast<Square>(std::countr_zero(bit_board));
}
