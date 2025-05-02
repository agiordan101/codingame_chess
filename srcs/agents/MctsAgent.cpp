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

    Node root_node(Move("a1b1"));
    root_node.board = board->clone();
    root_node.available_moves = moves;
    root_node.is_expanded = true;

    this->_nodes_explored = 0;
    while (!this->is_time_up())
    {
        // cerr << "MctsAgent: Starting iteration " << this->_nodes_explored << endl;
        this->mcts(&root_node, 0);
        // cerr << "MctsAgent: End iteration " << this->_nodes_explored << endl;

        this->_nodes_explored++;
        // this->_depth_reached = max_depth;
    }

    // cerr << "MctsAgent: Ending iterations " << this->_nodes_explored << endl;
    // cerr << "MctsAgent: Move count: " << moves.size() << endl;
    // cerr << "MctsAgent: Children count: " << root_node.children.size() << endl;

    for (size_t i = 0; i < moves.size(); i++)
        qualities->push_back(root_node.children[i]->value);

    float dtime = elapsed_time();
    if (dtime >= _ms_constraint)
        cerr << "MctsAgent: TIMEOUT: dtime=" << dtime << "/" << this->_ms_constraint << "ms"
             << endl;
}

vector<string> MctsAgent::get_stats()
{
    vector<string> stats;

    stats.push_back("version=BbMctsPv-3.1.6");
    stats.push_back("depth=" + to_string(this->_depth_reached));
    stats.push_back("states=" + to_string(this->_nodes_explored));
    cerr << "BbMctsPv-3.1.6\t: stats=" << stats[0] << " " << stats[1] << " " << stats[2] << endl;
    return stats;
}

string MctsAgent::get_name()
{
    return Board::get_name() + ".MctsAgent[" + to_string(this->_ms_constraint) + "ms]";
}

// --- PUBLIC METHODS ---

float MctsAgent::mcts(Node *node, int depth)
{
    float evaluation;

    if (depth > this->_depth_reached)
        this->_depth_reached = depth;

    if (node->visits == 0)
    {
        node->game_state = node->board->get_game_state();
        node->is_over = node->game_state != GAME_CONTINUE;
        if (node->is_over)
        {
            evaluation = (1 + node->game_state) / 2;
        }
        else
        {
            // cerr << "MctsAgent: expand_children start" << endl;
            expand_children(node);
            // cerr << "MctsAgent: expand_children end " << endl;

            // Simulation -> Rollout | Heuristic
            // cerr << "MctsAgent: Depth " << depth << " Simulation " << endl;
            // cerr << "MctsAgent: Simulation start" << endl;

            int player = node->board->is_white_turn() ? 1 : -1;
            evaluation = (1 + player * this->_heuristic->evaluate(node->board)) / 2;
            // cerr << "MctsAgent: Simulation end " << endl;
        }
    }
    else
    {
        if (node->is_over)
        {
            evaluation = (1 + node->game_state) / 2;
        }
        else
        {
            // Selection
            // cerr << "MctsAgent: Depth " << depth << " Selection after " << node->visits << "
            // visits"
            //  << endl;

            Node *child_node = select_child(node);

            // cerr << "MctsAgent: Lazy expansion start" << endl;
            if (!child_node->is_expanded)
            {
                create_child_board(node, child_node);
            }
            // cerr << "MctsAgent: Lazy expansion end " << endl;

            evaluation = mcts(child_node, depth + 1);
        }
    }

    // Backpropagation
    // cerr << "MctsAgent: Depth " << depth << " Backpropagation - Eval: " << evaluation << endl;

    // cerr << "MctsAgent: Backpropagation start" << endl;
    node->value += evaluation;
    node->visits++;
    // cerr << "MctsAgent: Backpropagation end " << endl;

    return evaluation;
}

Node *MctsAgent::select_child(Node *parent)
{
    // cerr << "MctsAgent: Selection - UCT - start" << endl;
    for (const auto &child : parent->children)
    {
        child->uct_value =
            (child->value / (child->visits + UTC_EPSILON)) +
            this->_exploration_constant * sqrt(log(parent->visits) / (child->visits + UTC_EPSILON));
    }
    // cerr << "MctsAgent: Selection - UCT - end " << endl;

    // cerr << "MctsAgent: Selection - Comparaison - start " << parent->children.size() << endl;
    auto max_child_it = std::max_element(
        parent->children.begin(), parent->children.end(),
        [](const std::unique_ptr<Node> &a, const std::unique_ptr<Node> &b)
        { return a->uct_value < b->uct_value; }
    );
    // cerr << "MctsAgent: Selection - Comparaison - end " << endl;

    return max_child_it->get();
}

void MctsAgent::expand_children(Node *parent)
{
    for (const Move &move : parent->available_moves)
    {
        parent->children.push_back(std::make_unique<Node>(move));
    }
}

void MctsAgent::create_child_board(Node *parent, Node *node)
{
    node->board = parent->board->clone();
    node->board->apply_move(node->last_move);
    node->available_moves = node->board->get_available_moves();
    node->is_expanded = true;
}

bool MctsAgent::is_time_up()
{
    return this->elapsed_time() >= this->_ms_turn_stop;
}

float MctsAgent::elapsed_time()
{
    return (float)(clock() - this->_start_time) / CLOCKS_PER_SEC * 1000;
}
