#ifndef MCTSAGENT_HPP
#define MCTSAGENT_HPP

#include "AbstractAgent.hpp"

#define UTC_EPSILON 1e-9

// Nodes hold information about a move made on the parent node board
struct Node
{
        Move  move;
        int   visits;
        float value;
        float uct_value;

        Board *resulting_board;
        bool   is_over;
        float  end_game_evaluation;

        std::vector<std::unique_ptr<Node>> children;
        std::vector<Move>                  available_moves; // Store unexplored moves

        Node()
            : move(Move("a1b1")), visits(0), value(0), uct_value(0), resulting_board(nullptr),
              is_over(false), end_game_evaluation(0.5){};

        Node(Move m)
            : move(m), visits(0), value(0), uct_value(0), resulting_board(nullptr), is_over(false),
              end_game_evaluation(0.5){};

        ~Node()
        {
            if (resulting_board != nullptr)
            {
                delete resulting_board;
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
        float _winrate;

        float mcts(Node *node, int depth);
        Node *select_child(Node *node);
        void  expand_node(Node *node);

        bool  is_time_up();
        float elapsed_time();
};

#endif