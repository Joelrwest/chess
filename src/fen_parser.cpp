#include "fen_parser.hpp"

FenParser::FenParser(std::string_view fen) : board_array{}, current_player{}
{
    const auto segments{split(fen, ' ')};

    if (segments.size() != 6)
    {
        throw std::logic_error{"FEN expects 6 segments"};
    }

    const auto piece_placement{split(segments[0], '/')};
    std::size_t board_array_idx{0};
    for (auto rank_it{piece_placement.rbegin()}; rank_it != piece_placement.rend(); ++rank_it)
    {
        for (const auto token : *rank_it)
        {
            if (std::isdigit(token))
            {
                for (auto count{0}; count < token - '0'; ++count)
                {
                    board_array[board_array_idx] = std::nullopt;
                    ++board_array_idx;
                }
            }
            else
            {
                Player player{std::isupper(token) ? Player::White : Player::Black};
                Piece piece{};
                switch (std::toupper(token))
                {
                case 'P':
                    piece = Piece::Pawn;
                    break;
                case 'N':
                    piece = Piece::Knight;
                    break;
                case 'B':
                    piece = Piece::Bishop;
                    break;
                case 'R':
                    piece = Piece::Rook;
                    break;
                case 'Q':
                    piece = Piece::Queen;
                    break;
                case 'K':
                    piece = Piece::King;
                    break;
                default:
                    throw std::logic_error{"FEN piece wasn't any of expected"};
                }
                board_array[board_array_idx].emplace(player, piece);
                ++board_array_idx;
            }
        }
    }

    if (board_array_idx != BOARD_SQUARES)
    {
        throw std::logic_error{"FEN piece placement didn't contain correct amount of squares"};
    }

    const auto side_to_move{segments[1]};
    if (side_to_move.size() != 1)
    {
        throw std::logic_error{"FEN did not have a single character side to move field"};
    }

    switch (*side_to_move.begin())
    {
    case 'w':
        current_player = Player::White;
        break;
    case 'b':
        current_player = Player::Black;
        break;
    default:
        throw std::logic_error{"FEN contained side to move field that was not 'w' or 'b'"};
    }

    // TODO: Parse remaining segments
    const auto castling_ability{segments[2]};
    const auto en_passant_target_squares{segments[3]};
    const auto halfmove_clock{segments[4]};
    const auto fullmove_counter{segments[5]};
}

const std::array<std::optional<std::pair<Player, Piece>>, BOARD_SQUARES> &FenParser::get_board_array() const
{
    return board_array;
}

Player FenParser::get_current_player() const
{
    return current_player;
}

std::vector<std::string_view> FenParser::split(std::string_view string, char delimiter)
{
    std::vector<std::string_view> tokens{};
    std::size_t begin_idx{0};
    for (std::size_t end_idx{string.find(delimiter)};
         (end_idx = string.find(delimiter, begin_idx)) != std::string_view::npos; begin_idx = end_idx + 1)
    {
        tokens.push_back(string.substr(begin_idx, end_idx - begin_idx));
    }
    tokens.push_back(string.substr(begin_idx));

    return tokens;
}
