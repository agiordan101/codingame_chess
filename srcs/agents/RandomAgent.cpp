#include "RandomAgent.hpp"

RandomAgent::RandomAgent()
{
    last_possible_moves = vector<Move>();
}

Move RandomAgent::choose_from(vector<Move> moves) {
    last_possible_moves = moves;
    return moves[rand() % moves.size()];
}

vector<tuple<Move, float>> RandomAgent::get_qualities()
{
    vector<tuple<Move, float>> qualities = vector<tuple<Move, float>>();
    for (int i = 0; i < last_possible_moves.size(); i++)
    {
        qualities.push_back(make_tuple(last_possible_moves[i], (float)1 / last_possible_moves.size()));
    }

    return qualities;
}
