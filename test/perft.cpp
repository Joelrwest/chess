#include <gtest/gtest.h>

#include "fen_parser.hpp"
#include "position.hpp"

#include <cstdint>
#include <span>
#include <string_view>

void test_position(std::string_view fen, std::span<const std::uint64_t> num_nodes);
std::uint64_t perft_impl(Position &position, std::uint8_t depth);

TEST(perft, starting_position)
{
    static constexpr auto FEN{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
    static const std::vector<std::uint64_t> NUM_NODES{1,       20,        400,        8902,        197281,
                                                      4865609, 119060324, 3195901860, 84998978956, 2439530234167};
    test_position(FEN, NUM_NODES);
}

void test_position(std::string_view fen, std::span<const std::uint64_t> num_nodes)
{
    const FenParser fen_parser{fen};
    Position position{fen_parser};

    for (std::uint8_t depth{0}; depth < num_nodes.size(); ++depth)
    {
        EXPECT_EQ(num_nodes[depth], perft_impl(position, depth));
    }
}

std::uint64_t perft_impl(Position &position, std::uint8_t depth)
{
    if (depth == 0)
    {
        return 1;
    }

    const auto moves{position.get_moves()};
    if (depth == 1)
    {
        return moves.size();
    }

    std::uint64_t node_count{0};
    for (const auto &move : moves)
    {
        (void)move;
        /*
        TODO
        */
        // position.make_move(move);
        // node_count += perft_impl(position, depth - 1);
        // position.unmake_move(move);
    }

    return node_count;
}
