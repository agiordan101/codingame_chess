#include "MinMaxIterDeepTTAgent.hpp"

MinMaxIterDeepTTAgent::MinMaxIterDeepTTAgent(AbstractHeuristic *heuristic, int ms_constraint)
{
    this->_heuristic = heuristic;
    this->_ms_constraint = ms_constraint;
    this->_ms_turn_stop = ms_constraint * 0.95;
    this->_depth_max_reached = 0;
    this->_nodes_explored = 0;
    this->_start_time = 0;

    this->_tt = new TranspositionTable();
}

void MinMaxIterDeepTTAgent::get_qualities(Board *board, vector<Move> moves, vector<float> *qualities)
{
    Board *new_board;

    this->_start_time = clock();
    this->_nodes_explored = 0;
    int zobrist_key = this->_tt->create_zobrist_key(board);

    // Initialize qualities with 0
    for (int i = 0; i < moves.size(); i++)
        qualities->push_back(0);

    int max_depth = 1;
    while (!this->is_time_up())
    {
        for (int i = 0; i < moves.size(); i++)
        {
            new_board = board->clone();

            // Apply the move and update the zobrist key
            int new_zobrist_key = this->_tt->update_key(new_board, &moves[i], zobrist_key);
            new_board->apply_move(moves[i]);

            float move_quality = this->minmax(new_board, max_depth, 1, new_zobrist_key);
            // cerr << "Move: " << move.to_uci() << " - Quality: " << move_quality << endl;

            qualities->at(i) = move_quality;
            delete new_board;
        }

        max_depth++;
    }

    if (max_depth > this->_depth_max_reached)
        this->_depth_max_reached = max_depth;

    float dtime = elapsed_time();
    if (dtime >= _ms_constraint)
        cerr << "MinMaxIterDeepTTAgent: TIMEOUT: dtime=" << dtime << "/" << this->_ms_constraint << "ms" << endl;
}

vector<string> MinMaxIterDeepTTAgent::get_stats()
{
    vector<string> stats;

    stats.push_back("depth=" + to_string(this->_depth_max_reached));
    stats.push_back("states=" + to_string(this->_nodes_explored));
    cerr << this->get_name() << ": stats=" << stats[0] << " " << stats[1] << endl;
    return stats;
}

string MinMaxIterDeepTTAgent::get_name()
{
    return "MinMaxIterDeepTTAgent[" + to_string(this->_ms_constraint) + "ms]." + this->_heuristic->get_name();
}

float MinMaxIterDeepTTAgent::minmax(Board *board, int max_depth, int depth, int zobrist_key)
{
    this->_nodes_explored++;

    s_MinMaxNode *node = this->_tt->get_node(zobrist_key);
    bool end_recursion = depth == max_depth || this->is_time_up();

    if (node)
    {
        if (end_recursion || node->leaf_node)
            return node->quality;

        if (node->child_moves.size() == 0)
        {
            node->child_moves = board->get_available_moves();
            node->child_qualities = vector<float>(node->child_moves.size(), 0);
            node->child_zobrist_keys = vector<int>(node->child_moves.size(), 0);
        }
    }
    else
    {
        node = new s_MinMaxNode();
        node->zobrist_key = zobrist_key;
        node->depth = depth;
        node->leaf_node = board->get_game_state() != GAME_CONTINUE;
        node->quality = this->_heuristic->evaluate(board);

        // If we reach the max depth, search time is up, or the game is over, we evaluate the board
        if (end_recursion || node->leaf_node)
        {
            this->_tt->insert_node(zobrist_key, node);
            return node->quality;
        }

        node->child_moves = board->get_available_moves();
        node->child_qualities = vector<float>(node->child_moves.size(), 0);
        node->child_zobrist_keys = vector<int>(node->child_moves.size(), 0);
    }

    // Go deeper in each child nodes and keep the best one
    float best_quality;
    if (board->is_white_turn())
    {
        best_quality = this->max_node(board, max_depth, depth, node);
    }
    else
    {
        best_quality = this->min_node(board, max_depth, depth, node);
    }

    return best_quality;
}

float MinMaxIterDeepTTAgent::max_node(Board *board, int max_depth, int depth, s_MinMaxNode *node)
{
    // White wants to maximize the heuristic value
    float best_quality = -1;
    for (int i = 0; i < node->child_moves.size(); i++)
    {
        Move move = node->child_moves.at(i);
        Board *new_board = board->clone();

        // Apply the move and update the zobrist key
        int new_zobrist_key = this->_tt->update_key(new_board, &move, node->zobrist_key);
        new_board->apply_move(move);

        float child_quality = this->minmax(new_board, max_depth, depth + 1, new_zobrist_key);
        node->child_qualities.at(i) = child_quality;
        node->child_zobrist_keys.at(i) = new_zobrist_key;

        best_quality = max(best_quality, child_quality);

        delete new_board;
        if (this->is_time_up())
            break;
    }

    return best_quality;
}

float MinMaxIterDeepTTAgent::min_node(Board *board, int max_depth, int depth, s_MinMaxNode *node)
{
    // Black wants to minimize the heuristic value
    float best_quality = 1;
    for (int i = 0; i < node->child_moves.size(); i++)
    {
        Move move = node->child_moves.at(i);
        Board *new_board = board->clone();

        // Apply the move and update the zobrist key
        int new_zobrist_key = this->_tt->update_key(new_board, &move, node->zobrist_key);
        new_board->apply_move(move);

        float child_quality = this->minmax(new_board, max_depth, depth + 1, new_zobrist_key);
        node->child_qualities.at(i) = child_quality;
        node->child_zobrist_keys.at(i) = new_zobrist_key;

        best_quality = min(best_quality, child_quality);

        delete new_board;
        if (this->is_time_up())
            break;
    }

    return best_quality;
}

bool MinMaxIterDeepTTAgent::is_time_up()
{
    return this->elapsed_time() >= this->_ms_turn_stop;
}

float MinMaxIterDeepTTAgent::elapsed_time()
{
    return (float)(clock() - this->_start_time) / CLOCKS_PER_SEC * 1000;
}

