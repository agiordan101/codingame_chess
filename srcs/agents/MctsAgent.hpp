#ifndef MCTSAGENT_HPP
#define MCTSAGENT_HPP

#include "AbstractAgent.hpp"

#define UTC_EPSILON 1e-9

struct Node
{
        Move  last_move;
        Board board;

        int   visits;
        int   value;
        float uct_value;

        vector<Node *> children;

        Node(Move m, Board b) : last_move(m), board(b), visits(0), value(0)
        {
        };
};

class MctsAgent : public AbstractAgent
{
    public:
        MctsAgent(AbstractHeuristic *heuristic, int ms_constraint);
        virtual void
        get_qualities(Board *board, vector<Move> moves, vector<float> *qualities) override;
        virtual string get_name() override;
        vector<string> get_stats() override;

    private:
        AbstractHeuristic *_heuristic;
        float              _exploration_constant;

        int     _ms_constraint;
        float   _ms_turn_stop;
        clock_t _start_time;

        int _depth_reached;
        int _nodes_explored;

        float mcts(Node *node, int depth);
        Node *select_child(Node *node);

        bool  is_time_up();
        float elapsed_time();
};

#endif