#include "types.hpp"

std::ostream &operator<<(std::ostream &os, Rank rank)
{
    os << rank + 1;
    return os;
}

std::ostream &operator<<(std::ostream &os, File file)
{
    os << static_cast<char>('A' + file);
    return os;
}

std::ostream &operator<<(std::ostream &os, Square square)
{
    os << square_to_file(square) << square_to_rank(square);
    return os;
}

std::ostream &operator<<(std::ostream &os, Direction direction)
{
    switch (direction)
    {
    case Direction::N:
        os << 'N';
        break;
    case Direction::S:
        os << 'S';
        break;
    case Direction::E:
        os << 'E';
        break;
    case Direction::W:
        os << 'W';
        break;
    case Direction::NE:
        os << "NE";
        break;
    case Direction::SE:
        os << "SE";
        break;
    case Direction::SW:
        os << "SW";
        break;
    case Direction::NW:
        os << "NW";
        break;
    default:
        throw std::logic_error{"Tried to print unknown direction"};
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, Player player)
{
    switch (player)
    {
    case Player::White:
        os << "white";
        break;
    case Player::Black:
        os << "black";
        break;
    default:
        throw std::logic_error{"Tried to print unknown player"};
    }

    return os;
}
