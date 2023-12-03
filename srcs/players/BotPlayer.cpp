#include "BotPlayer.hpp"
#include "../agents/AbstractAgent.hpp"
#include "../agents/RandomAgent.hpp"

BotPlayer::BotPlayer(string agent_name) {

    if (agent_name == "random")
        this->_agent = (AbstractAgent *)new RandomAgent();
    
}

Move BotPlayer::choose_from(vector<Move> moves) {
    return this->_agent->choose_from(moves);
}
