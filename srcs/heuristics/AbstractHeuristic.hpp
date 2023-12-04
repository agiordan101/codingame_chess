#ifndef ABSTRACTHEURISTIC_HPP
# define ABSTRACTHEURISTIC_HPP

#include "../chessengine/Board.hpp"

/*
    Heuristic values should be between -1 and 1.
        -1 : Black is winning
        0 : Balanced game
        1 : White is winning
*/

class AbstractHeuristic {

    public:
        virtual float evaluate(Board *board) = 0;
};

#endif