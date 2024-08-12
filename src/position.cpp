#include "position.hpp"

Position::Position() : white_bit_boards{}, black_bit_boards{}, current_player{Player::White}
{
}

Position::Position(const FenParser &fen_parser) : white_bit_boards{fen_parser}, black_bit_boards{fen_parser}, current_player{fen_parser.get_current_player()}
{
}

Evaluation Position::get_piece_difference() const
{
    return white_bit_boards.get_total_piece_value() - black_bit_boards.get_total_piece_value();
}

std::vector<Move> Position::get_moves() const
{
    if (current_player == Player::White)
    {
        BitBoard opponent_attacking_bit_board{0}; // TODO
        const auto opponent_occupied_bit_board{black_bit_boards.get_occupied_bit_board()};
        return white_bit_boards.get_moves(opponent_occupied_bit_board, opponent_attacking_bit_board);
    }
    else if (current_player == Player::Black)
    {
        BitBoard opponent_attacking_bit_board{0}; // TODO
        const auto opponent_occupied_bit_board{white_bit_boards.get_occupied_bit_board()};
        return black_bit_boards.get_moves(opponent_occupied_bit_board, opponent_attacking_bit_board);
    }

    throw std::logic_error{"It was neither black nor white's turn"};
}

std::ostream &operator<<(std::ostream &os, Position position)
{
    os << ' ';
    for (File file{0}; file < BOARD_WIDTH; ++file)
    {
        os << ' ' << file;
    }

    for (Rank rank{7}; rank < BOARD_WIDTH; --rank)
    {
        os << '\n' << rank << ' ';
        for (auto square{static_cast<Square>(rank * BOARD_WIDTH)}; square < (rank + 1) * BOARD_WIDTH; ++square)
        {
            /*
            Set background to appropriate colour for this square
            */
            os << ((square + rank) % 2 ? "\033[48;5;223m" : "\033[48;5;137m");

            /*
            Super repetitive but does the job
            */
            const auto mask{square_to_bit_board(square)};
            if (position.white_bit_boards.pawns & mask)
            {
                os << "♙";
            }
            else if (position.white_bit_boards.knights & mask)
            {
                os << "♘";
            }
            else if (position.white_bit_boards.bishops & mask)
            {
                os << "♗";
            }
            else if (position.white_bit_boards.rooks & mask)
            {
                os << "♖";
            }
            else if (position.white_bit_boards.queens & mask)
            {
                os << "♕";
            }
            else if (position.white_bit_boards.king & mask)
            {
                os << "♔";
            }
            else if (position.black_bit_boards.pawns & mask)
            {
                os << "♟︎";
            }
            else if (position.black_bit_boards.knights & mask)
            {
                os << "♞";
            }
            else if (position.black_bit_boards.bishops & mask)
            {
                os << "♝";
            }
            else if (position.black_bit_boards.rooks & mask)
            {
                os << "♜";
            }
            else if (position.black_bit_boards.queens & mask)
            {
                os << "♛";
            }
            else if (position.black_bit_boards.king & mask)
            {
                os << "♚";
            }
            else
            {
                os << ' ';
            }

            /*
            Turn background off
            */
            os << " \033[0m";
        }
    }

    return os;
}
