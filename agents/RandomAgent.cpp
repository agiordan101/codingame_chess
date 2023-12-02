#include "RandomAgent.hpp"

RandomAgent::RandomAgent()
{
}

Move RandomAgent::choose_from(vector<Move> moves)
{
    return moves[rand() % moves.size()];
}
