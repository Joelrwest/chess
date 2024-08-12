#include "position.hpp"

#include <iostream>

int main(void)
{
    const FenParser fen_parser{"8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8 b - - 99 50"};
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
