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
    root_node.board = board->clone();
    root_node.available_moves = moves;
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

    stats.push_back("version=BbMctsPv-rc");
    stats.push_back("depth=" + to_string(this->_depth_reached));
    stats.push_back("states=" + to_string(this->_nodes_explored));
    cerr << "BbMctsPv-rc\t: stats=" << stats[0] << " " << stats[1] << " " << stats[2] << endl;
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

    // Selection
    // cerr << "MctsAgent: Depth " << depth << " Selection after " << node->visits << "
    // visits"
    //  << endl;
    Node *child_node = select_child(node);

    if (child_node->visits == 0)
    {
        child_node->board = node->board->clone();
        child_node->board->apply_move(child_node->last_move);

        // cerr << "MctsAgent: Lazy expansion start" << endl;
        expand_node(child_node);
        // cerr << "MctsAgent: Lazy expansion end " << endl;

        // Simulation -> Rollout | Heuristic
        // cerr << "MctsAgent: Depth " << depth << " Simulation " << endl;
        // cerr << "MctsAgent: Simulation start" << endl;

        if (child_node->is_over)
            evaluation = child_node->end_game_evaluation;
        else
        {
            // PLayer = -1 because we are rewarding the last opponent move.
            // If Turn white -> Heuristic is made for white -> We revert it to get the black
            // evaluation If Turn black -> Same logic

            // int player = child_node->board->is_white_turn() ? 1 : -1;
            int player = -1;
            evaluation = (1 + player * this->_heuristic->evaluate(child_node->board)) / 2;
        }
        // cerr << "MctsAgent: Simulation end " << endl;
    }
    else
    {
        if (child_node->is_over)
            evaluation = child_node->end_game_evaluation;
        else
            evaluation = 1 - mcts(child_node, depth + 1);
    }

    // Backpropagation
    // cerr << "MctsAgent: Depth " << depth << " Backpropagation - Eval: " << evaluation << endl;

    // cerr << "MctsAgent: Backpropagation start" << endl;
    child_node->value += evaluation;
    child_node->visits++;
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

void MctsAgent::expand_node(Node *node)
{
    float game_state = node->board->get_game_state();
    if (game_state == GAME_CONTINUE)
    {
        // cerr << "MctsAgent: Expansion - Game contine - start " << endl;
        node->is_over = false;

        // Create children nodes for each available move
        node->available_moves = node->board->get_available_moves();
        for (const Move &move : node->available_moves)
        {
            node->children.push_back(std::make_unique<Node>(move));
        }
        // cerr << "MctsAgent: Expansion - Game contine - end " << endl;
    }
    else
    {
        // cerr << "MctsAgent: Expansion - Game over - start " << endl;
        node->is_over = true;

        // int player = node->board->is_white_turn() ? 1 : -1;
        // turn white . win black -> reward 1
        // turn white . win white -> reward -1
        // turn black . win black -> reward -1
        // turn black . win white -> reward 1
        if (game_state == DRAW)
        {
            node->end_game_evaluation = 0.5;
        }
        else
        {
            if (game_state == BLACK_WIN && node->board->is_white_turn() == false)
            {
                cerr << "MctsAgent: Expansion - Game over - BLACK_WIN on turn black ???" << endl;
            }
            if (game_state == WHITE_WIN && node->board->is_white_turn() == true)
            {
                cerr << "MctsAgent: Expansion - Game over - WHITE_WIN on turn white ???" << endl;
            }

            node->end_game_evaluation = 1;
            // node->end_game_evaluation = (1 + player * game_state) / 2;
        }
        // cerr << "MctsAgent: Expansion - Game over - end " << endl;
    }

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
