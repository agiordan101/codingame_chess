#include "PiecesHeuristic.hpp"

float PiecesHeuristic::evaluate(Board *board)
{
    // 8*1 + 2*3 + 2*3.2 + 2*5 + 9 = 41.4
    // Divide by more than the actual max possible sum will stand out end games
    const float max_value = 50;
    float evaluation = 0;

    float state = board->game_state();
    if (state != -1)
    {
        if (state == 0)
            return -1;
        else if (state == 0.5)
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

    float policy = evaluation / max_value;
    // float policy = min(evaluation / max_value, 1);

    if (policy > 1)
        cerr << "PiecesHeuristic: FEN: " << board->create_fen() << " | Pieces sum: " << evaluation << " | Policy: " << policy << endl;

    return policy;
}
