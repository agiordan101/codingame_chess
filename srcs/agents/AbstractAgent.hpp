#ifndef ABSTRACTAGENT_HPP
# define ABSTRACTAGENT_HPP

#include "../chessengine/Move.hpp"
#include "../chessengine/Board.hpp"
#include "../heuristics/AbstractHeuristic.hpp"

#include <vector>
#include <tuple>

class AbstractAgent {

    public:
        virtual void get_qualities(Board *board, vector<Move> moves, vector<float> *qualities) = 0;
        virtual string get_name() = 0;
};

#endif