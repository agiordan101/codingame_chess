#include "MctsAgent.hpp"

MctsAgent::MctsAgent(AbstractHeuristic *heuristic, int ms_constraint)
{
    this->_heuristic = heuristic;
    this->_exploration_constant = 1.4142135623730951;
    this->_ms_constraint = ms_constraint;
    this->_ms_turn_stop = ms_constraint * 0.95;
    this->_depth_reached = 0;
    this->_nodes_explored = 0;
    this->_start_time = 0;
}

// --- PRIVATE METHODS ---

void MctsAgent::get_qualities(Board *board, vector<Move> moves, vector<float> *qualities)
{
    this->_start_time = clock();

    Node root_node;
    root_node.resulting_board = board->clone();
    root_node.visits = 1;
    // root_node.is_expanded = true;
    // root_node.is_over = false;

    expand_node(&root_node);

    this->_nodes_explored = 0;
    while (!this->is_time_up())
    {
        // cerr << "MctsAgent: Starting iteration " << this->_nodes_explored << endl;
        float evaluation = this->mcts(&root_node, 0);
        // cerr << "MctsAgent: End iteration " << this->_nodes_explored << endl;

        // Root node value/visits old the actual winrate estimation
        root_node.value += 1 - evaluation;
        root_node.visits++;
    }

    this->_nodes_explored = root_node.visits;

    // cerr << "MctsAgent: Ending iterations " << this->_nodes_explored << endl;
    // cerr << "MctsAgent: Move count: " << moves.size() << endl;
    // cerr << "MctsAgent: Children count: " << root_node.children.size() << endl;

    // BotPlayer maximizes the score for white and minimizes it for black
    int player = board->is_white_turn() ? 1 : -1;

    for (size_t i = 0; i < moves.size(); i++)
        qualities->push_back(player * root_node.children[i]->visits);

    float dtime = elapsed_time();
    if (dtime >= _ms_constraint)
        cerr << "MctsAgent: TIMEOUT: dtime=" << dtime << "/" << this->_ms_constraint << "ms"
             << endl;
}

vector<string> MctsAgent::get_stats()
{
    vector<string> stats;

    stats.push_back("version=BbMctsPv-3.3.6");
    stats.push_back("depth=" + to_string(this->_depth_reached));
    stats.push_back("states=" + to_string(this->_nodes_explored));
    cerr << "BbMctsPv-3.3.6\t: stats=" << stats[0] << " " << stats[1] << " " << stats[2] << endl;
    return stats;
}

string MctsAgent::get_name()
{
    return Board::get_name() + ".MctsAgent[" + to_string(this->_ms_constraint) + "ms]";
}

// --- PUBLIC METHODS ---

float MctsAgent::mcts(Node *node, int depth)
{
    if (depth > this->_depth_reached)
        this->_depth_reached = depth;

    // Selection
    Node *child_node = select_child(node);

    float evaluation;
    if (child_node->visits == 0)
    {
        child_node->resulting_board = node->resulting_board->clone();
        child_node->resulting_board->apply_move(child_node->move);

        float game_state = child_node->resulting_board->get_game_state();
        if (game_state == GAME_CONTINUE)
        {
            // Expansion
            expand_node(child_node);

            // Simulation -> Rollout | Heuristic
            // Because the node value represent the strength of a move for both white and black,
            //  we must invert the heuristic evaluation for black moves (when the resulting board is
            //  white's turn)
            int player = child_node->resulting_board->is_white_turn() ? -1 : 1;
            evaluation = (1 + player * this->_heuristic->evaluate(child_node->resulting_board)) / 2;
        }
        else
        {
            child_node->is_over = true;
            child_node->end_game_evaluation = game_state == DRAW ? 0.5 : 1;

            evaluation = child_node->end_game_evaluation;
        }
    }
    else if (child_node->is_over)
        evaluation = child_node->end_game_evaluation;
    else
        evaluation = 1 - mcts(child_node, depth + 1);

    // Backpropagation
    child_node->value += evaluation;
    child_node->visits++;

    return evaluation;
}

Node *MctsAgent::select_child(Node *parent)
{
    for (const auto &child : parent->children)
    {
        child->uct_value =
            (child->value / (child->visits + UTC_EPSILON)) +
            this->_exploration_constant * sqrt(log(parent->visits) / (child->visits + UTC_EPSILON));
    }

    auto max_child_it = std::max_element(
        parent->children.begin(), parent->children.end(),
        [](const std::unique_ptr<Node> &a, const std::unique_ptr<Node> &b)
        { return a->uct_value < b->uct_value; }
    );

    return max_child_it->get();
}

void MctsAgent::expand_node(Node *node)
{
    // Create children nodes for each available move
    for (const Move &move : node->resulting_board->get_available_moves())
        node->children.push_back(std::make_unique<Node>(move));
}

bool MctsAgent::is_time_up()
{
    return this->elapsed_time() >= this->_ms_turn_stop;
}

float MctsAgent::elapsed_time()
{
    return (float)(clock() - this->_start_time) / CLOCKS_PER_SEC * 1000;
}
