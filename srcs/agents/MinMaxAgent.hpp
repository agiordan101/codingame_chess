#ifndef MINMAXITERDEEPAGENT_HPP
#define MINMAXITERDEEPAGENT_HPP

#include "AbstractAgent.hpp"

class MinMaxAgent : public AbstractAgent
{

    public:
        MinMaxAgent(AbstractHeuristic *heuristic, int ms_constraint);
        virtual void
        get_qualities(Board *board, vector<Move> moves, vector<float> *qualities) override;
        virtual string get_name() override;
        vector<string> get_stats() override;

    private:
        AbstractHeuristic *_heuristic;

        int     _ms_constraint;
        float   _ms_turn_stop;
        clock_t _start_time;

        int _depth_reached;
        int _nodes_explored;

        float minmax(Board *board, int max_depth, int depth);
        float max_node(Board *board, vector<Move> *moves, int max_depth, int depth);
        float min_node(Board *board, vector<Move> *moves, int max_depth, int depth);

        bool  is_time_up();
        float elapsed_time();
};

#endif