#include "PiecesHeuristic.hpp"

float PiecesHeuristic::evaluate(Board *board)
{
    float state = board->get_game_state();
    if (state != GAME_CONTINUE)
    {
        if (state == BLACK_WIN)
            return -1;
        else if (state == DRAW)
            return 0;
        else
            return 1;
    }

    int white_pawn_count = _count_bits(board->white_pawns);
    int white_knight_count = _count_bits(board->white_knights);
    int white_bishop_count = _count_bits(board->white_bishops);
    int white_rook_count = _count_bits(board->white_rooks);
    int white_queen_count = _count_bits(board->white_queens);
    int black_pawn_count = _count_bits(board->black_pawns);
    int black_knight_count = _count_bits(board->black_knights);
    int black_bishop_count = _count_bits(board->black_bishops);
    int black_rook_count = _count_bits(board->black_rooks);
    int black_queen_count = _count_bits(board->black_queens);

    float evaluation =
        white_pawn_count - black_pawn_count + 3 * (white_knight_count - black_knight_count) +
        3.2 * (white_bishop_count - black_bishop_count) +
        5 * (white_rook_count - black_rook_count) + 9 * (white_queen_count - black_queen_count);

    if (evaluation > 0)
        return 1 - 1.0 / (1 + evaluation);
    return -1 - 1.0 / (-1 + evaluation);
}

string PiecesHeuristic::get_name()
{
    return "PiecesHeuristic";
}
