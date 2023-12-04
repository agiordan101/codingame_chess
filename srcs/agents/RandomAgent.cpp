#include "RandomAgent.hpp"

vector<tuple<Move, float>> *RandomAgent::get_qualities(Board *board, vector<Move> moves)
{
    vector<tuple<Move, float>> *qualities = new vector<tuple<Move, float>>();
    for (int i = 0; i < moves.size(); i++)
        qualities->push_back(make_tuple(moves[i], (float)1 / moves.size()));

    return qualities;
}
