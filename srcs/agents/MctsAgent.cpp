#include "MctsAgent.hpp"

MctsAgent::MctsAgent(AbstractHeuristic *heuristic, int ms_constraint)
{
    this->_heuristic = heuristic;
    this->_exploration_constant = 2;
    this->_ms_constraint = ms_constraint;
    this->_ms_turn_stop = ms_constraint * 0.95;
    this->_depth_reached = 0;
    this->_nodes_explored = 0;
    this->_winrate = 0.5;
    this->_start_time = 0;

    this->_ms_board_selection = 0;
    this->_ms_board_cloning = 0;
    this->_ms_board_applying = 0;
    this->_ms_board_expansion = 0;
    this->_ms_board_simulation = 0;
    this->_ms_total = 0;
}

// --- PRIVATE METHODS ---

void MctsAgent::get_qualities(Board *board, vector<Move> moves, vector<float> *qualities)
{
    this->_start_time = clock();

    Node root_node;
    root_node.resulting_board = board->clone();
    root_node.visits = 1;
    root_node.utc_parent_exploration = 1;

    expand_node(&root_node);

    this->_nodes_explored = 0;
    while (!this->is_time_up())
    {
        // cerr << "MctsAgent: Starting iteration " << this->_nodes_explored << endl;
        float evaluation = this->mcts(&root_node, 0);
        // cerr << "MctsAgent: End iteration " << this->_nodes_explored << endl;

        // Save children move evaluations in root node
        root_node.value += evaluation;
        root_node.visits++;
        root_node.utc_parent_exploration = this->_exploration_constant * log(root_node.visits);
    }

    this->_nodes_explored = root_node.visits;
    this->_winrate = root_node.value / root_node.visits;

    // BotPlayer maximizes the score for white and minimizes it for black
    int player = board->is_white_turn() ? 1 : -1;

    for (size_t i = 0; i < moves.size(); i++)
        qualities->push_back(player * root_node.children_nodes[i]->visits);

    float dtime = elapsed_time(this->_start_time);
    // this->_ms_total += dtime;

    if (dtime >= _ms_constraint)
        cerr << "MctsAgent: TIMEOUT: dtime=" << dtime << "/" << this->_ms_constraint << "ms"
             << endl;
}

vector<string> MctsAgent::get_stats()
{
    vector<string> stats;

    stats.push_back("version=BbMctsPv-rc");
    stats.push_back("depth=" + to_string(this->_depth_reached));
    stats.push_back("states=" + to_string(this->_nodes_explored));
    stats.push_back("winrate=" + to_string(this->_winrate));
    cerr << "BbMctsPv-rc\t: stats=" << stats[0] << " " << stats[1] << " " << stats[2] << " "
         << stats[3] << endl;

    // int percent_selection = (this->_ms_board_selection / this->_ms_total) * 100;
    // int percent_cloning = (this->_ms_board_cloning / this->_ms_total) * 100;
    // int percent_applying = (this->_ms_board_applying / this->_ms_total) * 100;
    // int percent_expansion = (this->_ms_board_expansion / this->_ms_total) * 100;
    // int percent_simulation = (this->_ms_board_simulation / this->_ms_total) * 100;
    // int percent_remaining = 100 - percent_selection - percent_cloning - percent_applying -
    //                         percent_expansion - percent_simulation;
    // cerr << "BbMctsPv-3.7.6\t: times: Se=" << percent_selection << "% - Cl=" << percent_cloning
    //      << "% - Ap=" << percent_applying << "% - Ex=" << percent_expansion
    //      << "% - Si=" << percent_simulation << "% - Remaining=" << percent_remaining << "%" <<
    //      endl;
    return stats;
}

string MctsAgent::get_name()
{
    return Board::get_name() + ".MctsAgent[" + to_string(this->_ms_constraint) + "ms]." +
           this->_heuristic->get_name();
}

// --- PUBLIC METHODS ---

float MctsAgent::mcts(Node *parent_node, int depth)
{
    // clock_t start_time;

    if (depth > this->_depth_reached)
        this->_depth_reached = depth;

    // Selection
    // start_time = clock();
    Node *node = select_child(parent_node);
    // this->_ms_board_selection += elapsed_time(start_time);

    float evaluation;
    if (node->visits == 0)
    {
        // Tree leaf reached
        // start_time = clock();
        node->resulting_board = parent_node->resulting_board->clone();
        // this->_ms_board_cloning += elapsed_time(start_time);

        // start_time = clock();
        node->resulting_board->apply_move(node->move);
        // this->_ms_board_applying += elapsed_time(start_time);

        float game_state = node->resulting_board->get_game_state();
        if (game_state == GAME_CONTINUE)
        {
            // Expansion
            // start_time = clock();
            expand_node(node);
            // this->_ms_board_expansion += elapsed_time(start_time);

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
    return this->elapsed_time(this->_start_time) >= this->_ms_turn_stop;
}

float MctsAgent::elapsed_time(clock_t clock_start)
{
    return (float)(clock() - clock_start) / CLOCKS_PER_SEC * 1000;
}
