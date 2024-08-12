#include "move.hpp"

Move::Move(Square from, Square to) : from{from}, to{to}
{
}

Square Move::get_from() const
{
    return from;
}

Square Move::get_to() const
{
    return to;
}

std::ostream &operator<<(std::ostream &os, Move move)
{
    os << move.get_from() << move.get_to();
    return os;
}
