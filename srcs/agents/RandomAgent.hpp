#ifndef RANDOMAGENT_HPP
# define RANDOMAGENT_HPP

#include "AbstractAgent.hpp"

class RandomAgent : public AbstractAgent {

    public:
        RandomAgent();
        Move choose_from(vector<Move> moves) override;
        vector<tuple<Move, float>> get_qualities();
};

#endif