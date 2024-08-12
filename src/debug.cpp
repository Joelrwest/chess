#include "position.hpp"

#include <iostream>

int main(void)
{
    Position position{};

    std::cout << position << '\n';

    // for (const auto &move : position.get_moves())
    // {
    //     std::cout << "Move: " << move << '\n';
    // }

    return 0;
}
