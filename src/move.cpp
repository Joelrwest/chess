#include "move.hpp"

Move::Move(SquareUnderlying from, SquareUnderlying to) : from{from}, to{to}
{
}

SquareUnderlying Move::get_from() const
{
    return from;
}

SquareUnderlying Move::get_to() const
{
    return to;
}

std::ostream &operator<<(std::ostream &os, Move move)
{
    os << Square{move.get_from()} << Square{move.get_to()};
    return os;
}
