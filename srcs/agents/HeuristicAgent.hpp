#ifndef HEURISTICAGENT_HPP
# define HEURISTICAGENT_HPP

#include "AbstractAgent.hpp"

class HeuristicAgent : public AbstractAgent {

    public:
        HeuristicAgent(AbstractHeuristic *heuristic);
        virtual vector<tuple<Move, float>> *get_qualities(Board *board, vector<Move> moves) override;
    
    private:
        AbstractHeuristic* _heuristic;
};

#endif