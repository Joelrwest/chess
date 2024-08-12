#pragma once

#include "types.hpp"

class Move
{
  public:
    Move(SquareUnderlying from, SquareUnderlying to);

    SquareUnderlying get_from() const;
    SquareUnderlying get_to() const;

  private:
    SquareUnderlying from;
    SquareUnderlying to;
};

std::ostream &operator<<(std::ostream &os, Move move);
