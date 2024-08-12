#pragma once

#include "types.hpp"

class Move
{
  public:
    Move(Square from, Square to);

    Square get_from() const;
    Square get_to() const;

  private:
    Square from;
    Square to;
};

std::ostream &operator<<(std::ostream &os, Move move);
