#ifndef MCTSAGENT_HPP
#define MCTSAGENT_HPP

#include "AbstractAgent.hpp"

#define UTC_EPSILON 1e-9

struct Node
{
        Move last_move;
        bool is_expanded;

        Board *board;
        bool   is_over;
        float  end_game_evaluation;

        int   visits;
        float value;
        float uct_value;

        std::vector<std::unique_ptr<Node>> children;
        std::vector<Move>                  available_moves; // Store unexplored moves

        Node()
            : last_move(Move("a1b1")), is_expanded(false), board(nullptr), is_over(false),
              end_game_evaluation(0.5), visits(0), value(0), uct_value(0){};

        Node(Move m)
            : last_move(m), is_expanded(false), board(nullptr), is_over(false),
              end_game_evaluation(0.5), visits(0), value(0), uct_value(0){};

        ~Node()
        {
            if (is_expanded && board != nullptr)
            {
                delete board;
            }
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
        void  expand_node(Node *node);

        bool  is_time_up();
        float elapsed_time();
};

#endif