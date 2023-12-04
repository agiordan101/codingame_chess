#ifndef ABSTRACTAGENT_HPP
# define ABSTRACTAGENT_HPP

#include "../chessengine/Move.hpp"
#include "../chessengine/Board.hpp"
#include "../heuristics/AbstractHeuristic.hpp"

#include <vector>
#include <tuple>

class AbstractAgent {

    public:
        virtual vector<tuple<Move, float>> *get_qualities(Board *board, vector<Move> moves) = 0;
};

#endif