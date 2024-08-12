#pragma once

#include "types.hpp"

#include <array>
#include <optional>
#include <utility>
#include <vector>

class FenParser
{
  public:
    FenParser(std::string_view fen);

    const std::array<std::optional<std::pair<Player, Piece>>, BOARD_SQUARES> &get_board_array() const;
    Player get_current_player() const;

  private:
    static std::vector<std::string_view> split(std::string_view string, char delimiter);

    std::array<std::optional<std::pair<Player, Piece>>, BOARD_SQUARES> board_array;
    Player current_player;
    // TODO: Caslting, en passant, halfmove clock, fullmove counter
};
