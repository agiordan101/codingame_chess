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

    Node root_node(board);
    this->_nodes_explored = 0;
    while (!this->is_time_up())
    {
        this->mcts(&root_node, 0);
        this->_nodes_explored++;
        // this->_depth_reached = max_depth;
    }

    for (size_t i = 0; i < moves.size(); i++)
        qualities->at(i) = root_node.children[i]->value;

    float dtime = elapsed_time();
    if (dtime >= _ms_constraint)
        cerr << "MctsAgent: TIMEOUT: dtime=" << dtime << "/" << this->_ms_constraint << "ms"
             << endl;
}

vector<string> MctsAgent::get_stats()
{
    vector<string> stats;

    stats.push_back("version=BbMctsRo-rc");
    stats.push_back("depth=" + to_string(this->_depth_reached));
    stats.push_back("states=" + to_string(this->_nodes_explored));
    cerr << "BbMctsRo-rc\t: stats=" << stats[0] << " " << stats[1] << " " << stats[2] << endl;
    return stats;
}

string MctsAgent::get_name()
{
    return Board::get_name() + ".MctsAgent[" + to_string(this->_ms_constraint) + "ms]";
}

// --- PUBLIC METHODS ---

float MctsAgent::mcts(Node *node, int depth)
{
    if (node->visits != 0)
    {
        // Selection
        Node *selectedNode = select_child(node);
        float evaluation = 1 - mcts(selectedNode, depth + 1);

        // Backpropagation
        selectedNode->value += evaluation;
        selectedNode->visits++;

        return evaluation;
    }

    if (node->board == NULL)
    {
        // Expansion - Create the node board
        Board board = *node->board;
        board.apply_move(*node->move);
        node->board = &board;
    }

    // Expansion - Create the children
    vector<Move> available_moves = node->board->get_available_moves();
    for (Move move : available_moves)
        node->children.push_back(new Node(&move));

    // Simulation -> Rollout | Heuristic
    int player = node->board->is_white_turn() ? 1 : -1;
    return (1 + player * this->_heuristic->evaluate(node->board)) / 2;
}

Node *MctsAgent::select_child(Node *parent)
{
    for (Node *child : parent->children)
    {
        child->uct_value =
            (child->value / (child->visits + UTC_EPSILON)) +
            this->_exploration_constant * sqrt(log(parent->visits) / (child->visits + UTC_EPSILON));
    }

    return *std::max_element(
        parent->children.begin(), parent->children.end(),
        [](Node *a, Node *b) { return a->uct_value < b->uct_value; }
    );

    return nullptr;
}

bool MctsAgent::is_time_up()
{
    return this->elapsed_time() >= this->_ms_turn_stop;
}

float MctsAgent::elapsed_time()
{
    return (float)(clock() - this->_start_time) / CLOCKS_PER_SEC * 1000;
}
