#ifndef ABSTRACTPLAYER_HPP
# define ABSTRACTPLAYER_HPP

#include "../chessengine/Move.hpp"
#include "../agents/AbstractAgent.hpp"

#include <vector>

class AbstractPlayer {

    public:
        virtual Move choose_from(vector<Move> moves) = 0;
        // Draw ?
        // Resign ?

    protected:
        AbstractAgent* _agent;
};

#endif