#ifndef RANDOMAGENT_HPP
# define RANDOMAGENT_HPP

#include "AbstractAgent.hpp"
#include <random>
#include <algorithm>

class RandomAgent : public AbstractAgent {

    public:
        virtual void get_qualities(Board *board, vector<Move> moves, vector<float> *qualities) override;
};

#endif