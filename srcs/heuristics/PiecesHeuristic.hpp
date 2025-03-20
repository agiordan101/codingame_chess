#ifndef PIECESHEURISTIC_HPP
#define PIECESHEURISTIC_HPP

#include "AbstractHeuristic.hpp"

class PiecesHeuristic : public AbstractHeuristic
{

    public:
        float  evaluate(Board *board) override;
        string get_name() override;
};

#endif