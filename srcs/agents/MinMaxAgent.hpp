#ifndef MINMAXAGENT_HPP
# define MINMAXAGENT_HPP

#include "AbstractAgent.hpp"

class MinMaxAgent : public AbstractAgent {

    public:
        MinMaxAgent(AbstractHeuristic *heuristic, int depth);
        virtual void get_qualities(Board *board, vector<Move> moves, vector<float> *qualities) override;
        virtual string get_name() override;

    private:
        AbstractHeuristic* _heuristic;
        int _max_depth;

        float minmax(Board *board, int depth, float alpha, float beta);
        float max_float(float a, float b);
        float min_float(float a, float b);
        bool  alpha_cut(float best_quality, float *alpha, float *beta);
        bool  beta_cut(float best_quality, float *alpha, float *beta);
};

#endif