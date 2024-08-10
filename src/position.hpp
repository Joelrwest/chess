#pragma once

#include "bit_board.hpp"

class Position
{
  public:
    Position();

    /*
    Positive for white, negative for black
    */
    std::int16_t piece_difference();

  private:
    inline BitBoard occupied_bit_board();

    PlayerBitBoards<Player::White> white_bit_boards;
    PlayerBitBoards<Player::Black> black_bit_boards;
    Player current_player;
};
