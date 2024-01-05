#ifndef ABSTRACTHEURISTIC_HPP
# define ABSTRACTHEURISTIC_HPP

#include "../chessengine/Board.hpp"

class AbstractHeuristic {

    public:
        virtual float evaluate(Board *board) = 0;
        virtual string get_name() = 0;
};


#endif