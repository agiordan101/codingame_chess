#ifndef BOTPLAYER_HPP
# define BOTPLAYER_HPP

#include "../chessengine/Move.hpp"
#include "../agents/AbstractAgent.hpp"
#include <vector>

class BotPlayer {

    public:
        BotPlayer(string agent_name);
        Move choose_from(vector<Move> moves);
        // Draw ?
        // Resign ?

    private:
        AbstractAgent* _agent;
};

#endif