#ifndef RANDOMAGENT_HPP
# define RANDOMAGENT_HPP

#include "../chessengine/Move.hpp"
#include "AbstractAgent.hpp"
#include <vector>

class RandomAgent : public AbstractAgent {

    public:
        RandomAgent();
        Move choose_from(vector<Move> moves) override;
        vector<tuple<Move, float>> get_qualities();

    private:
        vector<Move> last_possible_moves;
};

#endif