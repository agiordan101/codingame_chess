#include "MinMaxAlphaBetaTransTableAgent.hpp"

MinMaxAlphaBetaTransTableAgent::MinMaxAlphaBetaTransTableAgent(
    AbstractHeuristic *heuristic, int ms_constraint
)
{
    this->_heuristic = heuristic;
    this->_ms_constraint = ms_constraint;
    this->_ms_turn_stop = ms_constraint * 0.95;
    this->_depth_reached = 0;
    this->_nodes_explored = 0;
    this->_nodes_created = 0;
    this->_nodes_collisions = 0;
    this->_alpha_cutoffs = 0;
    this->_beta_cutoffs = 0;
    this->_start_time = 0;

    this->_tt = new TranspositionTable();
}

void MinMaxAlphaBetaTransTableAgent::get_qualities(
    Board *board, vector<Move> moves, vector<float> *qualities
)
{
    this->_start_time = clock();

    this->_nodes_explored = 0;
    this->_nodes_created = 0;
    this->_nodes_collisions = 0;
    this->_alpha_cutoffs = 0;
    this->_beta_cutoffs = 0;

    for (size_t i = 0; i < moves.size(); i++)
        qualities->push_back(0);

    int max_depth = 1;
    while (!this->is_time_up())
    {
        for (size_t i = 0; i < moves.size(); i++)
        {
            Board new_board = *board;
            new_board.apply_move(moves[i]);

            float move_quality = this->minmax(&new_board, max_depth, 1, -1, 1);

            // If time is up, it shouldn't update the current move quality, because we can't say how
            // much children it has explored So it would end up having a random quality
            if (this->is_time_up())
                break;

            qualities->at(i) = move_quality;
        }

        max_depth++;
    }

    this->_depth_reached = max_depth;

    float dtime = elapsed_time();
    if (dtime >= _ms_constraint)
        cerr << "MinMaxAlphaBetaTransTableAgent: TIMEOUT: dtime=" << dtime << "/"
             << this->_ms_constraint << "ms" << endl;
}

vector<string> MinMaxAlphaBetaTransTableAgent::get_stats()
{
    vector<string> stats;

    stats.push_back("version=BbMmabttPv-rc");
    stats.push_back("depth=" + to_string(this->_depth_reached));
    stats.push_back("states=" + to_string(this->_nodes_explored));
    stats.push_back("states_created=" + to_string(this->_nodes_created));
    stats.push_back("states_collisions=" + to_string(this->_nodes_collisions));
    stats.push_back("alpha_cutoffs=" + to_string(this->_alpha_cutoffs));
    stats.push_back("beta_cutoffs=" + to_string(this->_beta_cutoffs));
    cerr << "BbMmabttPv-rc\t: stats=" << stats[0] << " " << stats[1] << " " << stats[2] << " "
         << stats[3] << " " << stats[4] << " " << stats[5] << " " << stats[6] << endl;
    return stats;
}

string MinMaxAlphaBetaTransTableAgent::get_name()
{
    return Board::get_name() + "MinMaxAlphaBetaTransTableAgent[" + to_string(this->_ms_constraint) +
           "ms]." + this->_heuristic->get_name();
}

float MinMaxAlphaBetaTransTableAgent::minmax(
    Board *board, int max_depth, int depth, float alpha, float beta
)
{
    this->_nodes_explored++;

    int           zobrist_key = board->get_zobrist_key();
    s_MinMaxNode *node = this->_tt->get_node(zobrist_key);

    // Initialize the node if the position wasn't already seen
    if (node->zobrist_key != zobrist_key)
    {
        this->_nodes_created++;

        if (node->zobrist_key != 0)
            this->_nodes_collisions++;

        node->zobrist_key = zobrist_key;
        node->depth = depth;
        node->leaf_node = board->get_game_state() != GAME_CONTINUE;

        vector<Move> child_moves = board->get_available_moves();
        for (size_t i = 0; i < child_moves.size(); i++)
            node->child_move_quality_pairs.emplace_back(child_moves[i], 0);
    }

    // Stop the search if we reach the max depth, search time is up, or the game is over
    if (depth == max_depth || this->is_time_up() || node->leaf_node)
        return this->_heuristic->evaluate(board);

    if (board->is_white_turn())
        node->quality = this->max_node(board, max_depth, depth, alpha, beta, node);
    else
        node->quality = this->min_node(board, max_depth, depth, alpha, beta, node);

    return node->quality;
}

float MinMaxAlphaBetaTransTableAgent::max_node(
    Board *board, int max_depth, int depth, float alpha, float beta, s_MinMaxNode *node
)
{
    // Sort moves by their quality, in descending order, to improve alpha-beta pruning effect
    this->sort_child_move_quality_pairs_descending(node);

    // White wants to maximize the heuristic value
    float best_quality = -1;
    for (size_t i = 0; i < node->child_move_quality_pairs.size(); i++)
    {
        Move  move = node->child_move_quality_pairs.at(i).first;
        Board new_board = *board;

        // Apply the move and update the zobrist key
        new_board.apply_move(move);

        float child_quality = this->minmax(&new_board, max_depth, depth + 1, alpha, beta);

        // Stop the search if we run out of time, the actual branch won't be used anyway
        if (this->is_time_up())
            break;

        node->child_move_quality_pairs.at(i).second = child_quality;

        best_quality = max(best_quality, child_quality);

        // Alpha-beta pruning - Stop the search when we know the current node won't be chosen
        // - Beta cut : If we're in a max node and the current child max quality is higher than a
        // brother node (which will be chosen by the parent min-node)
        if (beta <= best_quality)
        {
            this->_beta_cutoffs++;
            return best_quality;
        }

        // Update alpha for the next brother nodes
        alpha = max(alpha, best_quality);
    }

    return best_quality;
}

float MinMaxAlphaBetaTransTableAgent::min_node(
    Board *board, int max_depth, int depth, float alpha, float beta, s_MinMaxNode *node
)
{
    // Sort moves by their quality, in ascending order, to improve alpha-beta pruning effect
    this->sort_child_move_quality_pairs_ascending(node);

    // Black wants to minimize the heuristic value
    float best_quality = 1;
    for (size_t i = 0; i < node->child_move_quality_pairs.size(); i++)
    {
        Move  move = node->child_move_quality_pairs.at(i).first;
        Board new_board = *board;

        // Apply the move and update the zobrist key
        new_board.apply_move(move);

        float child_quality = this->minmax(&new_board, max_depth, depth + 1, alpha, beta);

        // Stop the search if we run out of time, the actual branch won't be used anyway
        if (this->is_time_up())
            break;

        node->child_move_quality_pairs.at(i).second = child_quality;

        best_quality = min(best_quality, child_quality);

        // Alpha-beta pruning - Stop the search when we know the current node won't be chosen
        // - Alpha cut : If we're in a min node and the current child min quality is lower than a
        // brother node (which will be chosen by the parent max-node)
        if (alpha >= best_quality)
        {
            this->_alpha_cutoffs++;
            return best_quality;
        }

        // Update beta for the next brother nodes
        beta = min(beta, best_quality);
    }

    return best_quality;
}

void MinMaxAlphaBetaTransTableAgent::sort_child_move_quality_pairs_descending(s_MinMaxNode *node)
{
    std::sort(
        node->child_move_quality_pairs.begin(), node->child_move_quality_pairs.end(),
        [](const std::pair<Move, float> &a, const std::pair<Move, float> &b)
        {
            return a.second > b.second; // Sort in descending order of quality
        }
    );
}

void MinMaxAlphaBetaTransTableAgent::sort_child_move_quality_pairs_ascending(s_MinMaxNode *node)
{
    std::sort(
        node->child_move_quality_pairs.begin(), node->child_move_quality_pairs.end(),
        [](const std::pair<Move, float> &a, const std::pair<Move, float> &b)
        {
            return a.second < b.second; // Sort in descending order of quality
        }
    );
}

bool MinMaxAlphaBetaTransTableAgent::is_time_up()
{
    return this->elapsed_time() >= this->_ms_turn_stop;
}

float MinMaxAlphaBetaTransTableAgent::elapsed_time()
{
    return (float)(clock() - this->_start_time) / CLOCKS_PER_SEC * 1000;
}
