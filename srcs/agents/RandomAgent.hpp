#ifndef RANDOMAGENT_HPP
# define RANDOMAGENT_HPP

#include "AbstractAgent.hpp"

class RandomAgent : public AbstractAgent {

    public:
        virtual vector<tuple<Move, float>> *get_qualities(Board *board, vector<Move> moves) override;
};

#endif