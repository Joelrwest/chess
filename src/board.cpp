#include "board.hpp"

namespace board
{
Board::Board()
    : white_pawns{STARTING_WHITE_PAWNS}, white_knights{STARTING_WHITE_KNIGHTS}, white_bishops{STARTING_WHITE_BISHOPS},
      white_rooks{STARTING_WHITE_ROOKS}, white_queens{STARTING_WHITE_QUEENS}, white_kings{STARTING_WHITE_KINGS},

      black_pawns{STARTING_BLACK_PAWNS}, black_knights{STARTING_BLACK_KNIGHTS}, black_bishops{STARTING_BLACK_BISHOPS},
      black_rooks{STARTING_BLACK_ROOKS}, black_queens{STARTING_BLACK_QUEENS}, black_kings{STARTING_BLACK_KINGS}
{
}
} // namespace board
