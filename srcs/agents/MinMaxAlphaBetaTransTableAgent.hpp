#ifndef MINMAXITERDEEPAGENTTT_HPP
#define MINMAXITERDEEPAGENTTT_HPP

#include "../transpositiontable/TranspositionTable.hpp"
#include "AbstractAgent.hpp"

class MinMaxAlphaBetaTransTableAgent : public AbstractAgent
{
    public:
        MinMaxAlphaBetaTransTableAgent(AbstractHeuristic *heuristic, int ms_constraint);
        virtual void
        get_qualities(Board *board, vector<Move> moves, vector<float> *qualities) override;
        virtual string get_name() override;
        vector<string> get_stats() override;

    private:
        AbstractHeuristic  *_heuristic;
        TranspositionTable *_tt;

        int     _ms_constraint;
        float   _ms_turn_stop;
        clock_t _start_time;

        int _depth_reached;
        int _nodes_explored;
        int _nodes_created;
        int _nodes_collisions;
        int _alpha_cutoffs;
        int _beta_cutoffs;

        float minmax(Board *board, int max_depth, int depth, float alpha, float beta);
        float max_node(
            Board *board, int max_depth, int depth, float alpha, float beta, s_MinMaxNode *node
        );
        float min_node(
            Board *board, int max_depth, int depth, float alpha, float beta, s_MinMaxNode *node
        );
        void sort_child_move_quality_pairs_descending(s_MinMaxNode *node);
        void sort_child_move_quality_pairs_ascending(s_MinMaxNode *node);

        bool  is_time_up();
        float elapsed_time();
};

#endif