#include "MctsAgent.hpp"

MctsAgent::MctsAgent(AbstractHeuristic *heuristic, int ms_constraint)
{
    this->_heuristic = heuristic;
    this->_exploration_constant = 2;
    this->_ms_constraint = ms_constraint;
    this->_ms_turn_stop = ms_constraint * 0.90;
    this->_root_node = NULL;
    this->_depth_reached = 0;
    this->_nodes_explored = 0;
    this->_winrate = 0.5;
    this->_turn_start_clock = 0;
}

// --- PRIVATE METHODS ---

Move MctsAgent::choose_from(Board *board, clock_t turn_start_clock)
{
    this->_turn_start_clock = turn_start_clock;

    vector<float> qualities;
    get_qualities(board, &qualities);

    // Determine whether to find the max or min element based on the player's turn
    auto best_it = board->is_white_turn() ? std::max_element(qualities.begin(), qualities.end())
                                          : std::min_element(qualities.begin(), qualities.end());

    // Calculate the index of the best quality
    size_t best_index = std::distance(qualities.begin(), best_it);

    // Update root node to the best child node
    this->_root_node = this->_root_node->children_nodes.at(best_index);

    return this->_root_node->move;
}

void MctsAgent::get_qualities(Board *board, vector<float> *qualities)
{
    if (this->_root_node == NULL)
    {
        this->_root_node = new Node();
        this->_root_node->resulting_board = board->clone();
        this->_root_node->visits = 1;
        this->_root_node->utc_parent_exploration = 1;
    }
    else
    {
        this->_root_node = find_child_node_played(board);

        // if (this->_root_node == NULL)
        // {
        //     cerr << "MctsAgent: ERROR: find_child_node_played returned NULL" << endl;
        // }
        // if (this->_root_node->children_nodes.size() == 0)
        // {
        //     cerr << "MctsAgent: ERROR: find_child_node_played returned a node with no children"
        //          << endl;
        // }
    }

    if (this->_root_node == NULL || this->_root_node->children_nodes.size() == 0)
    {
        expand_node(this->_root_node);
    }

    this->_nodes_explored = 0;
    while (!this->is_time_up())
    {
        // cerr << "MctsAgent: Starting iteration " << this->_root_node->visits << endl;
        float evaluation = this->mcts(this->_root_node, 0);
        // cerr << "MctsAgent: End iteration " << this->_root_node->visits << endl;

        // Save children move evaluations in root node
        this->_root_node->value += evaluation;
        this->_root_node->visits++;
        this->_root_node->utc_parent_exploration =
            this->_exploration_constant * log(this->_root_node->visits);
    }

    this->_nodes_explored = this->_root_node->visits;
    this->_winrate = this->_root_node->value / this->_root_node->visits;

    // BotPlayer maximizes the score for white and minimizes it for black
    int player = board->is_white_turn() ? 1 : -1;

    // Use child->utc_exploitation instead of visits ?
    for (size_t i = 0; i < this->_root_node->children_nodes.size(); i++)
        qualities->push_back(player * this->_root_node->children_nodes[i]->visits);

    // if (qualities->size() != this->_root_node->children_nodes.size())
    // {
    //     cerr << "MctsAgent: ERROR: qualities.size() != this->_root_node->children_nodes.size() "
    //          << endl;
    //     cerr << "qualities.size() = " << qualities->size() << endl;
    //     cerr << "this->_root_node->children_nodes.size() = "
    //          << this->_root_node->children_nodes.size() << endl;
    // }

    float dtime = elapsed_time(this->_turn_start_clock);

    if (dtime >= _ms_constraint)
        cerr << "MctsAgent: TIMEOUT: dtime=" << dtime << "/" << this->_ms_constraint << "ms"
             << endl;
}

vector<string> MctsAgent::get_stats()
{
    vector<string> stats;

    stats.push_back("version=BbMctsPv-3.9.8");
    stats.push_back("depth=" + to_string(this->_depth_reached));
    stats.push_back("states=" + to_string(this->_nodes_explored));
    stats.push_back("winrate=" + to_string(this->_winrate));
    cerr << "BbMctsPv-3.9.8\t: stats=" << stats[0] << " " << stats[1] << " " << stats[2] << " "
         << stats[3] << endl;
    return stats;
}

string MctsAgent::get_name()
{
    return Board::get_name() + ".MctsAgent[" + to_string(this->_ms_constraint) + "ms]." +
           this->_heuristic->get_name();
}

// --- PUBLIC METHODS ---

Node *MctsAgent::find_child_node_played(Board *board)
{
    string actual_fen = board->get_fen();

    for (Node *child : this->_root_node->children_nodes)
    {
        if (actual_fen == child->resulting_board->get_fen())
        {
            return child;
        }
    }

    cerr << "MctsAgent: find_child_node_played(): ERROR: child not found" << endl;
    return NULL;
}

float MctsAgent::mcts(Node *parent_node, int depth)
{
    if (depth > this->_depth_reached)
        this->_depth_reached = depth;

    // Selection
    Node *node = select_child(parent_node);

    float evaluation;
    if (node->visits == 0)
    {
        // Tree leaf reached
        node->resulting_board = parent_node->resulting_board->clone();
        node->resulting_board->apply_move(node->move);

        float game_state = node->resulting_board->get_game_state();
        if (game_state == GAME_CONTINUE)
        {
            // Expansion
            expand_node(node);

            // Simulation -> Not a rollout, but a simple Heuristic
            // Because the node value represent the strength of a move for both white and black,
            //  we must invert the heuristic evaluation for black moves (when the resulting board is
            //  white's turn)
            evaluation = simulation(node);
        }
        else
        {
            // New terminal state encountered - No simulation needed
            node->is_over = true;
            node->end_game_evaluation = game_state == DRAW ? 0.5 : 1;

            evaluation = node->end_game_evaluation;
        }
    }
    else if (node->is_over)
        evaluation = node->end_game_evaluation;
    else
        evaluation = 1 - mcts(node, depth + 1);

    // Backpropagation
    node->value += evaluation;
    node->visits++;
    node->utc_exploitation = node->value / node->visits;
    node->utc_parent_exploration = this->_exploration_constant * log(node->visits);

    return evaluation;
}

Node *MctsAgent::select_child(Node *parent)
{
    for (const auto &child : parent->children_nodes)
    {
        if (child->visits == 0)
            return child;

        child->uct_value =
            child->utc_exploitation + sqrt(parent->utc_parent_exploration / child->visits);
    }

    Node *max_child_it = *max_element(
        parent->children_nodes.begin(), parent->children_nodes.end(),
        [](Node *a, Node *b) { return a->uct_value < b->uct_value; }
    );

    return max_child_it;
}

void MctsAgent::expand_node(Node *node)
{
    // Create children_nodes nodes for each available move
    for (const Move &move : node->resulting_board->get_available_moves())
        node->children_nodes.push_back(new Node(move));
}

float MctsAgent::simulation(Node *node)
{
    float evaluation;

    // Sigmoid version (Between 0 and 1)
    if (node->resulting_board->is_white_turn())
        evaluation = 1 - this->_heuristic->evaluate(node->resulting_board);
    else
        evaluation = this->_heuristic->evaluate(node->resulting_board);

    // First Linear version (Between -1 and 1)
    // int player = node->resulting_board->is_white_turn() ? -1 : 1;
    // evaluation = (1 + player * this->_heuristic->evaluate(node->resulting_board)) / 2;

    return evaluation;
}

bool MctsAgent::is_time_up()
{
    return this->elapsed_time(this->_turn_start_clock) >= this->_ms_turn_stop;
}

float MctsAgent::elapsed_time(clock_t clock_start)
{
    return (float)(clock() - clock_start) / CLOCKS_PER_SEC * 1000;
}
