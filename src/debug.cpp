#include "position.hpp"

#include <iostream>

int main(void)
{
    const FenParser fen_parser{"8/8/8/4p1K1/2k1P3/8/8/8 b - - 0 1"};
    Position position{fen_parser};

    std::cout << position << '\n';

    const auto moves{position.get_moves()};
    for (const auto &move : moves)
    {
        std::cout << "Move: " << move << '\n';
    }
    std::cout << moves.size() << '\n';

    return 0;
}
