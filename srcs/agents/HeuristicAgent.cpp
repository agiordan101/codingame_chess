#include "HeuristicAgent.hpp"

HeuristicAgent::HeuristicAgent(AbstractHeuristic *heuristic)
{
    _heuristic = heuristic;
}

vector<tuple<Move, float>> *HeuristicAgent::get_qualities(Board *board, vector<Move> moves)
{
    vector<tuple<Move, float>> *qualities = new vector<tuple<Move, float>>();
    Board *next_turn_board;
    float evaluation;

    for (int i = 0; i < moves.size(); i++)
    {
        Board *next_turn_board = board->clone();
        next_turn_board->apply_move(moves[i]);

        evaluation = this->_heuristic->evaluate(next_turn_board);

        qualities->push_back(make_tuple(moves[i], evaluation));

        delete next_turn_board;
    }

    return qualities;
}
