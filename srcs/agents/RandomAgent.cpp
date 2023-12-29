#include "RandomAgent.hpp"

void RandomAgent::get_qualities(Board *board, vector<Move> moves, vector<float> *qualities)
{
    // Send a random float between 0 and 1 for each move
    for (int i = 0; i < moves.size(); i++)
    {
        qualities->push_back((float)rand() / RAND_MAX);
    }
}
