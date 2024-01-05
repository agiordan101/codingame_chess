#ifndef PIECESHEURISTIC_HPP
# define PIECESHEURISTIC_HPP

#include "AbstractHeuristic.hpp"

class PiecesHeuristic : public AbstractHeuristic {

    public:
        virtual float evaluate(Board *board) override;
        virtual string get_name() override;
};

#endif