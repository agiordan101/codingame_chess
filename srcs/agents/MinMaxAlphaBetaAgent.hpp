#ifndef MINMAXITERDEEPAGENT_HPP
#define MINMAXITERDEEPAGENT_HPP

#include "AbstractAgent.hpp"

class MinMaxAlphaBetaAgent : public AbstractAgent
{

    public:
        MinMaxAlphaBetaAgent(AbstractHeuristic *heuristic, int ms_constraint);
        virtual string get_name() override;
        vector<string> get_stats() override;
        virtual Move   choose_from(Board *board, clock_t turn_start_clock) override;
        void  get_qualities(Board *board, vector<Move> moves, vector<float> *qualities);

    private:
        AbstractHeuristic *_heuristic;

        int     _ms_constraint;
        float   _ms_turn_stop;
        clock_t _turn_start_clock;

        int _depth_reached;
        int _nodes_explored;

        float minmax(Board *board, int max_depth, int depth, float alpha, float beta);
        float max_node(
            Board *board, vector<Move> *moves, int max_depth, int depth, float alpha, float beta
        );
        float min_node(
            Board *board, vector<Move> *moves, int max_depth, int depth, float alpha, float beta
        );

        bool  is_time_up();
        float elapsed_time(clock_t clock_start);
};

#endif