#include "BotPlayer.hpp"
#include "../agents/AbstractAgent.hpp"
#include "../agents/RandomAgent.hpp"

BotPlayer::BotPlayer(AbstractAgent *agent) {

    this->_agent = agent;
}

Move BotPlayer::choose_from(vector<Move> moves) {
    return this->_agent->choose_from(moves);
}
