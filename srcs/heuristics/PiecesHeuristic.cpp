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

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            char piece = board->get_cell(x, y);

            if (piece != EMPTY_CELL)
            {
                float piece_value = get_piece_value(piece);

                if (isupper(piece))
                    evaluation += piece_value;
                else
                    evaluation -= piece_value;
            }
        }
    }

    float policy;
    if (evaluation > max_value)
        policy = 0.95f;
    else if (evaluation < -max_value)
        policy = -0.95f;
    else
        policy = evaluation / max_value;

    if (policy > 1)
            cerr << "PiecesHeuristic: FEN: " << board->create_fen() << " | Pieces sum: " << evaluation << " | Policy: " << policy << endl;

    return policy;
}

string PiecesHeuristic::get_name()
{
    return "PiecesHeuristic";
}
