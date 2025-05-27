#ifndef ABSTRACTAGENT_HPP
#define ABSTRACTAGENT_HPP

#include "../chessengine/Board.hpp"
#include "../chessengine/Move.hpp"
#include "../heuristics/AbstractHeuristic.hpp"

#include <tuple>
#include <vector>

class AbstractAgent
{

    public:
        virtual Move   choose_from(Board *board, vector<Move> moves) = 0;
        virtual string get_name() = 0;

        virtual vector<string> get_stats()
        {
            return vector<string>{};
        };
};

#endif