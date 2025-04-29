#ifndef ROLLOUTHEURISTIC_HPP
#define ROLLOUTHEURISTIC_HPP

#include "AbstractHeuristic.hpp"

class RolloutHeuristic : public AbstractHeuristic
{

    public:
        float  evaluate(Board *board) override;
        string get_name() override;
};

#endif