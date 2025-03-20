#ifndef MINMAXALPHABETAOLDAGENT_HPP
#define MINMAXALPHABETAOLDAGENT_HPP

#include "AbstractAgent.hpp"

class MinMaxAlphaBetaOldAgent : public AbstractAgent
{
    public:
        MinMaxAlphaBetaOldAgent(AbstractHeuristic *heuristic, int depth);
        virtual void
        get_qualities(Board *board, vector<Move> moves, vector<float> *qualities) override;
        virtual string get_name() override;
        vector<string> get_stats() override;

    private:
        AbstractHeuristic *_heuristic;
        int                _max_depth;
        int                _nodes_explored;

        float minmax(Board *board, int depth, float alpha, float beta);
        float max_float(float a, float b);
        float min_float(float a, float b);
        bool  alpha_cut(float best_quality, float *alpha, float *beta);
        bool  beta_cut(float best_quality, float *alpha, float *beta);
};

#endif