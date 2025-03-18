#include "PiecesHeuristic.hpp"

float PiecesHeuristic::evaluate(Board *board)
{
    // 8*1 + 2*3 + 2*3.2 + 2*5 + 9 = 41.4
    // Divide by more than the actual max possible sum will stand out end games
    const float max_value = 51;
    float evaluation = 0;

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

    # if BITBOARD_IMPLEMENTATION == 1

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

        evaluation = white_pawn_count - black_pawn_count +\
            3 * (white_knight_count - black_knight_count) +\
            3.2 * (white_bishop_count - black_bishop_count) +\
            5 * (white_rook_count - black_rook_count) +\
            9 * (white_queen_count - black_queen_count);
    # else
        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                char piece = board->get_cell(x, y);

                if (piece != EMPTY_CELL)
                {
                    float piece_value;
                    switch (tolower(piece))
                    {
                        case 'p':
                            piece_value = 1;
                        case 'n':
                            piece_value = 3;
                        case 'b':
                            piece_value = 3.2;
                        case 'r':
                            piece_value = 5;
                        case 'q':
                            piece_value = 9;
                        default:
                            piece_value = 0;
                    }

                    if (isupper(piece))
                        evaluation += piece_value;
                    else
                        evaluation -= piece_value;
                }
            }
        }
    # endif

    float policy;
    if (evaluation > max_value)
        policy = 0.95f;
    else if (evaluation < -max_value)
        policy = -0.95f;
    else
        policy = evaluation / max_value;

    if (policy > 1)
            cerr << get_name() << ": FEN: " << board->create_fen() << " | Pieces sum: " << evaluation << " | Policy: " << policy << endl;

    return policy;
}

string PiecesHeuristic::get_name()
{
    # if BITBOARD_IMPLEMENTATION == 1
        return "PiecesHeuristicBb";
    # else
        return "PiecesHeuristic";
    # endif
}
