#ifndef ABSTRACTAGENT_HPP
# define ABSTRACTAGENT_HPP

#include "../chessengine/Move.hpp"

#include <vector>
#include <tuple>

class AbstractAgent {

    public:
        virtual Move choose_from(vector<Move> moves) = 0;
        virtual vector<tuple<Move, float>> get_qualities() = 0;

    protected:
        vector<Move> last_possible_moves;
};

#endif