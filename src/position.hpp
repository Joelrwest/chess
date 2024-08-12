#pragma once

#include "bit_board.hpp"
#include "move.hpp"
#include "fen_parser.hpp"

class Position
{
  public:
    Position();
    Position(const FenParser &fen_parser);

    /*
    Positive for white, negative for black
    */
    Evaluation get_piece_difference() const;
    std::vector<Move> get_moves() const;

  private:
    BitBoards<Player::White> white_bit_boards;
    BitBoards<Player::Black> black_bit_boards;
    Player current_player;

    friend std::ostream &operator<<(std::ostream &os, Position position);
};
