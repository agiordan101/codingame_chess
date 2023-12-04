#include "BotPlayer.hpp"

BotPlayer::BotPlayer(AbstractAgent *agent) {

    this->_agent = agent;
}

Move BotPlayer::choose_from(Board *board, vector<Move> moves) {

    vector<tuple<Move, float>> *qualities = this->_agent->get_qualities(board, moves);

    int best_index = 0;
    float best_quality = get<1>(qualities->at(0));
    float current_quality;
    for (int i = 1; i < qualities->size(); i++)
    {
        current_quality = get<1>(qualities->at(i));
        if (current_quality > best_quality)
        {
            best_index = i;
            best_quality = current_quality;
        }
    }

    return get<0>(qualities->at(best_index));
}
