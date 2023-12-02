#ifndef ABSTRACTAGENT_HPP
# define ABSTRACTAGENT_HPP

#include "../chessengine/Move.hpp"
#include <vector>

class AbstractAgent {

    public:
        virtual Move choose_from(vector<Move> moves) = 0;
        virtual vector<tuple<Move, float>> get_qualities() = 0;
};

#endif