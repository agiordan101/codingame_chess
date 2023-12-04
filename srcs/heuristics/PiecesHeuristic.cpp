#include "PiecesHeuristic.hpp"

float PiecesHeuristic::evaluate(Board *board)
{
    float evaluation = 0;

    float state = board->game_state();
    if (state != -1)
    {
        if (state == 0)
            return -1000;
        else if (state == 0.5)
            return 0;
        else
            return 1000;
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

    // cerr << "FEN: " << board->create_fen() << " | PiecesHeuristic: " << evaluation << endl;
    // Should be between -1 and 1
    return evaluation;
}