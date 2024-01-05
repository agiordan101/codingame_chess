#include "HeuristicAgent.hpp"

HeuristicAgent::HeuristicAgent(AbstractHeuristic *heuristic)
{
    _heuristic = heuristic;
}

void HeuristicAgent::get_qualities(Board *board, vector<Move> moves, vector<float> *qualities)
{
    Board *next_turn_board;
    float evaluation;

    for (int i = 0; i < moves.size(); i++)
    {
        next_turn_board = board->clone();
        next_turn_board->apply_move(moves[i]);

        evaluation = this->_heuristic->evaluate(next_turn_board);

        qualities->push_back(evaluation);

        delete next_turn_board;
    }
}

string HeuristicAgent::get_name()
{
    return "HeuristicAgent." + this->_heuristic->get_name();
}
