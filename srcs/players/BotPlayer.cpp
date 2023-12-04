#include "BotPlayer.hpp"

BotPlayer::BotPlayer(AbstractAgent *agent) {

    this->_agent = agent;
}

Move BotPlayer::choose_from(Board *board, vector<Move> moves) {

    vector<tuple<Move, float>> *qualities = this->_agent->get_qualities(board, moves);

    // Depending on the player turn, we want to maximize or minimize the heuristic value
    float (BotPlayer::*best_heuristic_choose)(float, float) = board->is_white_turn() ? &BotPlayer::max_float : &BotPlayer::min_float;

    float current_quality;
    float new_best_quality;
    int best_index = 0;
    float best_quality = get<1>(qualities->at(0));
    for (int i = 1; i < qualities->size(); i++)
    {
        current_quality = get<1>(qualities->at(i));
        new_best_quality = (this->*best_heuristic_choose)(best_quality, current_quality);

        // cerr << "Move: " << get<0>(qualities->at(i)).to_uci() << " | Quality: " << current_quality << " | Best quality: " << best_quality << " | New best quality: " << new_best_quality << endl;
        if (best_quality != new_best_quality)
        {
            best_index = i;
            best_quality = new_best_quality;
        }
    }

    return get<0>(qualities->at(best_index));
}

float BotPlayer::max_float(float a, float b) {
    return max(a, b);
}

float BotPlayer::min_float(float a, float b) {
    return min(a, b);
}