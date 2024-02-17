#ifndef MINMAXITERDEEPAGENT_HPP
# define MINMAXITERDEEPAGENT_HPP

#include "AbstractAgent.hpp"

class MinMaxIterDeepAgent : public AbstractAgent {

    public:
        MinMaxIterDeepAgent(AbstractHeuristic *heuristic, int ms_constraint);
        virtual void get_qualities(Board *board, vector<Move> moves, vector<float> *qualities) override;
        virtual string get_name() override;

    private:
        AbstractHeuristic* _heuristic;
        int _ms_constraint;
        clock_t _start_time;

        float minmax(Board *board, int depth, float alpha, float beta);
        float max_float(float a, float b);
        float min_float(float a, float b);
        bool  alpha_cut(float best_quality, float *alpha, float *beta);
        bool  beta_cut(float best_quality, float *alpha, float *beta);

        bool is_time_up();
};

#endif