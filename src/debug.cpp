#include "position.hpp"

#include <iostream>

int main(void)
{
    const FenParser fen_parser{"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"};
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
