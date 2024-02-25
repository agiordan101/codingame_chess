#ifndef MINMAXITERDEEPAGENTTT_HPP
# define MINMAXITERDEEPAGENTTT_HPP

# include "AbstractAgent.hpp"
# include "../transpositiontable/TranspositionTable.hpp"

class MinMaxIterDeepTTAgent : public AbstractAgent {

    public:
        MinMaxIterDeepTTAgent(AbstractHeuristic *heuristic, int ms_constraint);
        virtual void            get_qualities(Board *board, vector<Move> moves, vector<float> *qualities) override;
        virtual string          get_name() override;
        vector<string>          get_stats() override;

    private:
        AbstractHeuristic* _heuristic;
        TranspositionTable* _tt;

        int _ms_constraint;
        float _ms_turn_stop;
        clock_t _start_time;

        int _depth_max_reached;
        int _nodes_explored;

        float minmax(Board *board, int max_depth, int depth, int zobrist_key);
        float max_node(Board *board, int max_depth, int depth, s_MinMaxNode *node);
        float min_node(Board *board, int max_depth, int depth, s_MinMaxNode *node);

        bool is_time_up();
        float elapsed_time();
};

#endif