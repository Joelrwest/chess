#include "position.hpp"

Position::Position() : white_bit_boards{}, black_bit_boards{}, current_player{Player::White}
{
}

std::int16_t Position::piece_difference()
{
    return white_bit_boards.total_piece_value() - black_bit_boards.total_piece_value();
}

inline BitBoard Position::occupied_bit_board()
{
    return white_bit_boards.occupied_bit_board() & black_bit_boards.occupied_bit_board();
}
