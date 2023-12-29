#ifndef HEURISTICAGENT_HPP
# define HEURISTICAGENT_HPP

#include "AbstractAgent.hpp"

class HeuristicAgent : public AbstractAgent {

    public:
        HeuristicAgent(AbstractHeuristic *heuristic);
        virtual void get_qualities(Board *board, vector<Move> moves, vector<float> *qualities) override;
    
    private:
        AbstractHeuristic* _heuristic;
};

#endif