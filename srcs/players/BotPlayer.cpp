#include "BotPlayer.hpp"

BotPlayer::BotPlayer(AbstractAgent *agent)
{

    this->_agent = agent;
}

Move BotPlayer::choose_from(Board *board, vector<Move> moves)
{
    vector<float> qualities;
    this->_agent->get_qualities(board, moves, &qualities);

    // Should be in the constructor
    // Depending on the player turn, we want to maximize or minimize the heuristic value
    float (BotPlayer::*best_heuristic_choose)(float, float) =
        board->is_white_turn() ? &BotPlayer::max_float : &BotPlayer::min_float;

    int   best_index = 0;
    float best_quality = qualities.at(0);
    float new_best_quality;
    // TODO: Use std::max_element()
    for (size_t i = 1; i < qualities.size(); i++)
    {
        new_best_quality = (this->*best_heuristic_choose)(best_quality, qualities.at(i));

        // cerr << "Move: " << moves.at(i).to_uci() << " | Quality: " << qualities.at(i) << " | Best
        // quality: " << best_quality << " | New best quality: " << new_best_quality << endl;
        if (best_quality != new_best_quality)
        {
            best_index = i;
            best_quality = new_best_quality;
        }
    }

    return moves.at(best_index);
}

vector<string> BotPlayer::get_stats()
{
    return this->_agent->get_stats();
}

string BotPlayer::get_name()
{
    return "Bot." + this->_agent->get_name();
}

float BotPlayer::max_float(float a, float b)
{
    return max(a, b);
}

float BotPlayer::min_float(float a, float b)
{
    return min(a, b);
}
