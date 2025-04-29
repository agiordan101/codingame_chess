#include "RolloutHeuristic.hpp"

float RolloutHeuristic::evaluate(Board *board)
{
    float game_state = board->get_game_state();
    while (game_state == GAME_CONTINUE)
    {
        // Generate all possible moves
        vector<Move> moves = board->get_available_moves();

        // Play a random move
        Move move = moves[rand() % moves.size()];
        board->apply_move(move);

        game_state = board->get_game_state();
    }

    return game_state;
}

string RolloutHeuristic::get_name()
{
    return "RolloutHeuristic";
}
