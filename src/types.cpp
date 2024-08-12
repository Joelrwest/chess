#include "types.hpp"

Rank &operator++(Rank &rank)
{
    ++reinterpret_cast<std::underlying_type_t<Rank> &>(rank);
    return rank;
}

Rank operator++(Rank &rank, int)
{
    const auto saved_rank{reinterpret_cast<std::underlying_type_t<Rank> &>(rank)++};
    return Rank{saved_rank};
}

Rank &operator--(Rank &rank)
{
    --reinterpret_cast<std::underlying_type_t<Rank> &>(rank);
    return rank;
}

Rank operator--(Rank &rank, int)
{
    const auto saved_rank{reinterpret_cast<std::underlying_type_t<Rank> &>(rank)--};
    return Rank{saved_rank};
}

std::ostream &operator<<(std::ostream &os, Rank rank)
{
    os << rank + 1;
    return os;
}

File &operator++(File &file)
{
    ++reinterpret_cast<std::underlying_type_t<File> &>(file);
    return file;
}

File operator++(File &file, int)
{
    const auto saved_file{reinterpret_cast<std::underlying_type_t<File> &>(file)++};
    return File{saved_file};
}

std::ostream &operator<<(std::ostream &os, File file)
{
    os << static_cast<char>('A' + file);
    return os;
}

Square &operator++(Square &square)
{
    ++reinterpret_cast<std::underlying_type_t<Square> &>(square);
    return square;
}

Square operator++(Square &square, int)
{
    const auto saved_square{reinterpret_cast<std::underlying_type_t<Square> &>(square)++};
    return Square{saved_square};
}

std::ostream &operator<<(std::ostream &os, Square square)
{
    const auto file{static_cast<char>('A' + (square % BOARD_WIDTH))};
    const auto rank{square / BOARD_WIDTH + 1};
    os << file << rank;

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
