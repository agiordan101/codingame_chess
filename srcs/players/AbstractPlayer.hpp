#ifndef ABSTRACTPLAYER_HPP
# define ABSTRACTPLAYER_HPP

#include "../agents/AbstractAgent.hpp"
#include "../chessengine/Board.hpp"
#include "../chessengine/Move.hpp"

#include <vector>

class AbstractPlayer {

    public:
        virtual Move choose_from(Board *board, vector<Move> moves) = 0;
        virtual string get_name() = 0;
};

#endif