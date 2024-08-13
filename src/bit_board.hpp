#pragma once

#include "bit_board_constants.hpp"
#include "fen_parser.hpp"
#include "move.hpp"
#include "types.hpp"

#include <bit>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

class Position;

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

    static constexpr auto NOT_A_FILE_BIT_BOARD{~file_to_bit_board(File::FA)};
    static constexpr auto NOT_AB_FILE_BIT_BOARD{NOT_A_FILE_BIT_BOARD & ~file_to_bit_board(File::FB)};
    static constexpr auto NOT_H_FILE_BIT_BOARD{~file_to_bit_board(File::FH)};
    static constexpr auto NOT_GH_FILE_BIT_BOARD{NOT_H_FILE_BIT_BOARD & ~file_to_bit_board(File::FG)};
    static constexpr auto NOT_RANK_END_BIT_BOARD{NOT_A_FILE_BIT_BOARD & NOT_H_FILE_BIT_BOARD};
    static constexpr auto NOT_FILE_END_BIT_BOARD{~rank_to_bit_board(Rank::R1) & ~rank_to_bit_board(Rank::R8)};
    static constexpr auto NOT_EDGE_BIT_BOARD{NOT_RANK_END_BIT_BOARD & NOT_FILE_END_BIT_BOARD};

    template <Direction direction> static consteval BitBoard create_diagonal_bit_board(Square starting_square);
    static constexpr auto MAIN_DIAGONAL_BIT_BOARD{create_diagonal_bit_board<Direction::NE>(Square::A1)};
    static constexpr auto MAIN_ANTIDIAGONAL_BIT_BOARD{create_diagonal_bit_board<Direction::NW>(Square::H1)};

    static consteval Lookup<BitBoard> create_knight_attacks_bit_board_lookup();
    static consteval Lookup<BitBoard> create_king_attacks_bit_board_lookup();
    static constexpr Lookup<BitBoard> KNIGHT_ATTACKS_BIT_BOARD_LOOKUP{create_knight_attacks_bit_board_lookup()};
    static constexpr Lookup<BitBoard> KING_ATTACKS_BIT_BOARD_LOOKUP{create_king_attacks_bit_board_lookup()};

    static consteval Lookup<BitBoard> create_bishop_relevant_occupancies_bit_board_lookup();
    static consteval Lookup<BitBoard> create_rook_relevant_occupancies_bit_board_lookup();
    static constexpr Lookup<BitBoard> BISHOP_RELEVANT_OCCUPANCIES_BIT_BOARD_LOOKUP{
        create_bishop_relevant_occupancies_bit_board_lookup()};
    static constexpr Lookup<BitBoard> ROOK_RELEVANT_OCCUPANCIES_BIT_BOARD_LOOKUP{
        create_rook_relevant_occupancies_bit_board_lookup()};

    static consteval Lookup<Magic> create_bishop_magics_lookup();
    static consteval Lookup<Magic> create_rook_magics_lookup();
    static constexpr Lookup<Magic> BISHOP_MAGICS_LOOKUP{create_bishop_magics_lookup()};
    static constexpr Lookup<Magic> ROOK_MAGICS_LOOKUP{create_rook_magics_lookup()};

    using BishopAttackLookup = Lookup<BitBoard, 512>;
    using RookAttackLookup = Lookup<BitBoard, 4096>;
    static consteval BishopAttackLookup create_bishop_attacks_bit_board_lookup();
    static consteval RookAttackLookup create_rook_attacks_bit_board_lookup();
    static constexpr BishopAttackLookup BISHOP_ATTACKS_BIT_BOARD_LOOKUP{create_bishop_attacks_bit_board_lookup()};
    static constexpr RookAttackLookup ROOK_ATTACKS_BIT_BOARD_LOOKUP{create_rook_attacks_bit_board_lookup()};

    void add_pawn_moves(std::vector<Move> &moves, BitBoard self_occupied_bit_board,
                        BitBoard opponent_occupied_bit_board) const;
    void add_knight_moves(std::vector<Move> &moves, BitBoard self_occupied_bit_board) const;
    void add_bishop_moves(std::vector<Move> &moves, BitBoard self_occupied_bit_board,
                          BitBoard opponent_occupied_bit_board) const;
    void add_rook_moves(std::vector<Move> &moves, BitBoard self_occupied_bit_board,
                        BitBoard opponent_occupied_bit_board) const;
    void add_queen_moves(std::vector<Move> &moves, BitBoard self_occupied_bit_board,
                         BitBoard opponent_occupied_bit_board) const;
    void add_king_moves(std::vector<Move> &moves, BitBoard self_occupied_bit_board,
                        BitBoard opponent_attacking_bit_board) const;

    template <typename F>
    static inline void serialise_bit_board(std::vector<Move> &moves, BitBoard bit_board, F from_function);
    inline static std::uint8_t count_bits(BitBoard bit_board);
    inline static std::uint8_t ls1b(BitBoard bit_board);

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
    for (SquareUnderlying square{0}; square < BOARD_SQUARES; ++square)
    {
        const auto &entry{board_array.at(square)};
        if (entry.has_value() && entry->first == player)
        {
            const auto mask{square_to_bit_board(Square{square})};
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
    const auto self_occupied_bit_board{get_occupied_bit_board()};

    add_pawn_moves(moves, self_occupied_bit_board, opponent_occupied_bit_board);
    add_knight_moves(moves, self_occupied_bit_board);
    add_bishop_moves(moves, self_occupied_bit_board, opponent_occupied_bit_board);
    add_rook_moves(moves, self_occupied_bit_board, opponent_occupied_bit_board);
    add_queen_moves(moves, self_occupied_bit_board, opponent_occupied_bit_board);
    add_king_moves(moves, self_occupied_bit_board, opponent_attacking_bit_board);

    return moves;
}

template <Player player>
void BitBoards<player>::add_pawn_moves(std::vector<Move> &moves, BitBoard self_occupied_bit_board,
                                       BitBoard opponent_occupied_bit_board) const
{
    /*
    Push moves
    */
    const auto occupied_bit_board{self_occupied_bit_board | opponent_occupied_bit_board};
    auto single_push_bit_board{direction_shift<Constants::PAWN_PUSH_DIRECTION>(pawns)};
    single_push_bit_board &= ~occupied_bit_board;
    serialise_bit_board(moves, single_push_bit_board, [](auto to) { return to - Constants::PAWN_PUSH_DIRECTION; });

    static constexpr auto SINGLE_PUSH_RANK{
        direction_shift<Constants::PAWN_PUSH_DIRECTION>(Constants::STARTING_PAWNS_BIT_BOARD)};
    auto double_push_bit_board{
        direction_shift<Constants::PAWN_PUSH_DIRECTION>(single_push_bit_board & SINGLE_PUSH_RANK)};
    double_push_bit_board &= ~occupied_bit_board;
    serialise_bit_board(moves, double_push_bit_board,
                        [](auto to) { return to - static_cast<std::uint8_t>(2 * Constants::PAWN_PUSH_DIRECTION); });

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
void BitBoards<player>::add_knight_moves(std::vector<Move> &moves, BitBoard self_occupied_bit_board) const
{
    auto bit_board{knights};
    for (SquareUnderlying from{0}; bit_board; ++from)
    {
        const auto lsb{ls1b(bit_board)};
        from += lsb;

        const auto attacks_bit_board{KNIGHT_ATTACKS_BIT_BOARD_LOOKUP.at(from) & ~self_occupied_bit_board};
        serialise_bit_board(moves, attacks_bit_board, [from](auto) { return from; });

        bit_board >>= (lsb + 1);
    }
}

template <Player player>
void BitBoards<player>::add_bishop_moves(std::vector<Move> &moves, BitBoard self_occupied_bit_board,
                                         BitBoard opponent_occupied_bit_board) const
{
    auto bit_board{rooks};
    for (SquareUnderlying from{0}; bit_board; ++from)
    {
        const auto lsb{ls1b(bit_board)};
        from += lsb;

        auto idx{self_occupied_bit_board | opponent_occupied_bit_board};
        idx &= BISHOP_RELEVANT_OCCUPANCIES_BIT_BOARD_LOOKUP.at(from);
        idx *= BISHOP_MAGICS_LOOKUP.at(from);

        static constexpr auto SHIFT_AMOUNT{BOARD_SQUARES -
                                           static_cast<int>(std::log2(BISHOP_ATTACKS_BIT_BOARD_LOOKUP.size()))};
        idx >>= SHIFT_AMOUNT;

        const auto attacks_bit_board{BISHOP_ATTACKS_BIT_BOARD_LOOKUP.at(idx)};
        serialise_bit_board(moves, attacks_bit_board, [from](auto) { return from; });

        bit_board >>= (lsb + 1);
    }
}

template <Player player>
void BitBoards<player>::add_rook_moves(std::vector<Move> &moves, BitBoard self_occupied_bit_board,
                                       BitBoard opponent_occupied_bit_board) const
{
    auto bit_board{rooks};
    for (SquareUnderlying from{0}; bit_board; ++from)
    {
        const auto lsb{ls1b(bit_board)};
        from += lsb;

        auto idx{self_occupied_bit_board | opponent_occupied_bit_board};
        idx &= ROOK_RELEVANT_OCCUPANCIES_BIT_BOARD_LOOKUP.at(from);
        idx *= ROOK_MAGICS_LOOKUP.at(from);

        static constexpr auto SHIFT_AMOUNT{BOARD_SQUARES -
                                           static_cast<int>(std::log2(ROOK_ATTACKS_BIT_BOARD_LOOKUP.size()))};
        idx >>= SHIFT_AMOUNT;

        const auto attacks_bit_board{ROOK_ATTACKS_BIT_BOARD_LOOKUP.at(idx)};
        serialise_bit_board(moves, attacks_bit_board, [from](auto) { return from; });

        bit_board >>= (lsb + 1);
    }
}

template <Player player>
void BitBoards<player>::add_queen_moves(std::vector<Move> &moves, BitBoard self_occupied_bit_board,
                                        BitBoard opponent_occupied_bit_board) const
{
    (void)moves;
    (void)self_occupied_bit_board;
    (void)opponent_occupied_bit_board;
}

template <Player player>
void BitBoards<player>::add_king_moves(std::vector<Move> &moves, BitBoard self_occupied_bit_board,
                                       BitBoard opponent_attacking_bit_board) const
{
    /* Reasonably assumes for the sake of speed there's exactly 1 king */
    const SquareUnderlying from{ls1b(king)};
    const auto attacks_no_check_bit_board{KING_ATTACKS_BIT_BOARD_LOOKUP.at(from) & ~self_occupied_bit_board &
                                          ~opponent_attacking_bit_board};

    serialise_bit_board(moves, attacks_no_check_bit_board, [from](auto) { return from; });
}

template <Player player>
template <typename F>
inline void BitBoards<player>::serialise_bit_board(std::vector<Move> &moves, BitBoard bit_board, F from_function)
{
    for (SquareUnderlying to{0}; bit_board; ++to)
    {
        const auto lsb{ls1b(bit_board)};
        to += lsb;
        const auto from{static_cast<SquareUnderlying>(from_function(to))};
        moves.emplace_back(from, to);
        bit_board >>= (lsb + 1);
    }
}

template <Player player> inline std::uint8_t BitBoards<player>::count_bits(BitBoard bit_board)
{
    return std::popcount(bit_board);
}

template <Player player> inline std::uint8_t BitBoards<player>::ls1b(BitBoard bit_board)
{
    return static_cast<std::uint8_t>(std::countr_zero(bit_board));
}

template <Player player>
template <Direction direction>
consteval BitBoard BitBoards<player>::create_diagonal_bit_board(Square starting_square)
{
    BitBoard diagonal{square_to_bit_board(starting_square)};
    diagonal |= direction_shift<direction>(diagonal);
    diagonal |= direction_shift<Direction{2 * direction}>(diagonal);
    diagonal |= direction_shift<Direction{4 * direction}>(diagonal);

    return diagonal;
}

template <Player player> consteval Lookup<BitBoard> BitBoards<player>::create_knight_attacks_bit_board_lookup()
{
    Lookup<BitBoard> knight_attacks_bit_board_lookup{};
    for (SquareUnderlying from{0}; from < BOARD_SQUARES; ++from)
    {
        const auto from_bit_board{square_to_bit_board(Square{from})};

        BitBoard attacks_bit_board{0};
        attacks_bit_board |= direction_shift<Direction::NNE>(from_bit_board & NOT_H_FILE_BIT_BOARD);
        attacks_bit_board |= direction_shift<Direction::ENE>(from_bit_board & NOT_GH_FILE_BIT_BOARD);
        attacks_bit_board |= direction_shift<Direction::ESE>(from_bit_board & NOT_GH_FILE_BIT_BOARD);
        attacks_bit_board |= direction_shift<Direction::SSE>(from_bit_board & NOT_H_FILE_BIT_BOARD);
        attacks_bit_board |= direction_shift<Direction::SSW>(from_bit_board & NOT_A_FILE_BIT_BOARD);
        attacks_bit_board |= direction_shift<Direction::WSW>(from_bit_board & NOT_AB_FILE_BIT_BOARD);
        attacks_bit_board |= direction_shift<Direction::WNW>(from_bit_board & NOT_AB_FILE_BIT_BOARD);
        attacks_bit_board |= direction_shift<Direction::NNW>(from_bit_board & NOT_A_FILE_BIT_BOARD);

        knight_attacks_bit_board_lookup.at(from) = attacks_bit_board;
    }

    return knight_attacks_bit_board_lookup;
}

template <Player player>
consteval Lookup<BitBoard> BitBoards<player>::create_bishop_relevant_occupancies_bit_board_lookup()
{
    Lookup<BitBoard> bishop_relevant_occupancies_bit_board_lookup{};
    for (SquareUnderlying from{0}; from < BOARD_SQUARES; ++from)
    {
        const auto diagonal_shift_amount{(from >> 3) - (from & 0x7)}; /* Cursed */
        const auto antidiagonal_shift_amount{(from & 0x7) + (from >> 3) - 7};

        BitBoard relevant_occupancy_bit_board{0};

        relevant_occupancy_bit_board |= diagonal_shift(MAIN_DIAGONAL_BIT_BOARD, diagonal_shift_amount);
        relevant_occupancy_bit_board |= diagonal_shift(MAIN_ANTIDIAGONAL_BIT_BOARD, antidiagonal_shift_amount);
        relevant_occupancy_bit_board &= NOT_EDGE_BIT_BOARD;
        relevant_occupancy_bit_board &= ~square_to_bit_board(Square{from});

        bishop_relevant_occupancies_bit_board_lookup.at(from) = relevant_occupancy_bit_board;
    }

    return bishop_relevant_occupancies_bit_board_lookup;
}

template <Player player>
consteval Lookup<BitBoard> BitBoards<player>::create_rook_relevant_occupancies_bit_board_lookup()
{
    Lookup<BitBoard> rook_relevant_occupancies_bit_board_lookup{};
    for (SquareUnderlying from{0}; from < BOARD_SQUARES; ++from)
    {
        const Square from_square{from};
        const auto rank{square_to_rank(from_square)};
        const auto file{square_to_file(from_square)};

        BitBoard relevant_occupancy_bit_board{0};
        relevant_occupancy_bit_board |= rank_to_bit_board(rank) & NOT_RANK_END_BIT_BOARD;
        relevant_occupancy_bit_board |= file_to_bit_board(file) & NOT_FILE_END_BIT_BOARD;
        relevant_occupancy_bit_board &= ~square_to_bit_board(from_square);

        rook_relevant_occupancies_bit_board_lookup.at(from) = relevant_occupancy_bit_board;
    }

    return rook_relevant_occupancies_bit_board_lookup;
}

template <Player player> consteval Lookup<BitBoard> BitBoards<player>::create_king_attacks_bit_board_lookup()
{
    Lookup<BitBoard> king_attacks_bit_board_lookup{};
    for (SquareUnderlying from{0}; from < BOARD_SQUARES; ++from)
    {
        const auto from_bit_board{square_to_bit_board(Square{from})};

        BitBoard attacks_bit_board{0};
        attacks_bit_board |= direction_shift<Direction::N>(from_bit_board);
        attacks_bit_board |= direction_shift<Direction::NE>(from_bit_board & NOT_H_FILE_BIT_BOARD);
        attacks_bit_board |= direction_shift<Direction::E>(from_bit_board & NOT_H_FILE_BIT_BOARD);
        attacks_bit_board |= direction_shift<Direction::SE>(from_bit_board & NOT_H_FILE_BIT_BOARD);
        attacks_bit_board |= direction_shift<Direction::S>(from_bit_board);
        attacks_bit_board |= direction_shift<Direction::SW>(from_bit_board & NOT_A_FILE_BIT_BOARD);
        attacks_bit_board |= direction_shift<Direction::W>(from_bit_board & NOT_A_FILE_BIT_BOARD);
        attacks_bit_board |= direction_shift<Direction::NW>(from_bit_board & NOT_A_FILE_BIT_BOARD);

        king_attacks_bit_board_lookup.at(from) = attacks_bit_board;
    }

    return king_attacks_bit_board_lookup;
}

template <Player player> consteval Lookup<Magic> BitBoards<player>::create_bishop_magics_lookup()
{
    Lookup<Magic> bishop_magics_lookup{};

    return bishop_magics_lookup;
}

template <Player player> consteval Lookup<Magic> BitBoards<player>::create_rook_magics_lookup()
{
    Lookup<Magic> rook_magics_lookup{};

    return rook_magics_lookup;
}

template <Player player>
consteval BitBoards<player>::BishopAttackLookup BitBoards<player>::create_bishop_attacks_bit_board_lookup()
{
    BishopAttackLookup bishop_attacks_bit_board_lookup{};

    return bishop_attacks_bit_board_lookup;
}

template <Player player>
consteval BitBoards<player>::RookAttackLookup BitBoards<player>::create_rook_attacks_bit_board_lookup()
{
    RookAttackLookup rook_attacks_bit_board_lookup{};

    return rook_attacks_bit_board_lookup;
}
