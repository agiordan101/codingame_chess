#ifndef MCTSAGENT_HPP
#define MCTSAGENT_HPP

#include "AbstractAgent.hpp"

// Nodes hold information about a move made on the parent node board
struct Node
{
        Move  move;
        int   visits;
        float value;

        float utc_exploitation;
        float utc_exploration;
        float utc_parent_exploration;
        float uct_value;

        Board *resulting_board;
        bool   is_over;
        float  end_game_evaluation;

        std::vector<Node *> children_nodes;

        // TODO: Set visits & utc_parent_exploration to 1
        Node()
            : move(Move("a1b1")), visits(0), value(0), utc_exploitation(0), utc_exploration(0),
              utc_parent_exploration(0), uct_value(std::numeric_limits<float>::infinity()),
              resulting_board(nullptr), is_over(false), end_game_evaluation(0.5){};

        Node(Move m)
            : move(m), visits(0), value(0), utc_exploitation(0), utc_exploration(0),
              utc_parent_exploration(0), uct_value(std::numeric_limits<float>::infinity()),
              resulting_board(nullptr), is_over(false), end_game_evaluation(0.5){};

        ~Node(){
            // if (resulting_board != nullptr)
            // {
            //     delete resulting_board;
            // }
            // for (Node *child : children_nodes)
            //     delete child;
            // children_nodes.clear();
        };
};

class MctsAgent : public AbstractAgent
{
    public:
        MctsAgent(AbstractHeuristic *heuristic, int ms_constraint);
        virtual string get_name() override;
        vector<string> get_stats() override;
        virtual Move   choose_from(Board *board, clock_t _turn_start_clock) override;

    private:
        AbstractHeuristic *_heuristic;
        float              _exploration_constant;

        int     _ms_constraint;
        float   _ms_turn_stop;
        clock_t _turn_start_clock;

        Node *_root_node;
        int   _depth_reached;
        int   _nodes_explored;
        float _winrate;

        // TODO: Add underscores
        void  create_root_node(Board *board);
        void  get_qualities(Board *board);
        Node *find_child_node_played(Board *board);

        float mcts(Node *node, int depth);
        Node *select_child(Node *node);
        void  expand_node(Node *node);
        float simulation(Node *node);

        bool  is_time_up();
        float elapsed_time(clock_t clock_start);
};

#endif