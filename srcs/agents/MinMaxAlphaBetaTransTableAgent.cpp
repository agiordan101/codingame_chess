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

    stats.push_back("version=BbMmabttPv-3");
    stats.push_back("depth=" + to_string(this->_depth_reached));
    stats.push_back("states=" + to_string(this->_nodes_explored));
    stats.push_back("states_created=" + to_string(this->_nodes_created));
    stats.push_back("states_collisions=" + to_string(this->_nodes_collisions));
    stats.push_back("alpha_cutoffs=" + to_string(this->_alpha_cutoffs));
    stats.push_back("beta_cutoffs=" + to_string(this->_beta_cutoffs));
    cerr << "BbMmabttPv-3\t: stats=" << stats[0] << " " << stats[1] << " " << stats[2] << " "
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

    // Create the node if the position wasn't already seen
    if (node->zobrist_key != zobrist_key)
    {
        this->_nodes_created++;

        if (node->zobrist_key != 0)
        {
            this->_nodes_collisions++;
            // cerr << "MinMaxAlphaBetaTransTableAgent: COLLISION: zobrist_key=" << zobrist_key
            //  << endl;
        }

        node->zobrist_key = zobrist_key;
        node->depth = depth;
        node->last_position_fen = board->get_last_position_fen();
        node->game_turn = board->game_turn;
        node->leaf_node = board->get_game_state() != GAME_CONTINUE;
        node->quality = this->_heuristic->evaluate(board);
        node->alpha = alpha;
        node->beta = beta;
    }
    // else if (node->last_position_fen == board->get_last_position_fen())
    // {
    //     // We can only use alpha and beta values from previous search branches are the same.
    //     // So we rely on the actual zobrist key and the last FEN position

    //     // Compare actual branch beta with the one from previous search, and keep the lowest
    //     if (node->beta < beta)
    //         beta = node->beta;
    //     // else
    //     //     node->beta = beta;

    //     // Compare actual branch alpha with the one from previous search, and keep the lowest
    //     if (node->alpha > alpha)
    //         alpha = node->alpha;
    //     // else
    //     //     node->alpha = alpha;
    // }

    // Stop the search if we reach the max depth, search time is up, or the game is over
    if (depth == max_depth || this->is_time_up() || node->leaf_node)
        return node->quality;

    // Initialize its child data if it wasn't already expanded
    if (node->child_move_quality_pairs.size() == 0)
    {
        vector<Move> child_moves = board->get_available_moves();
        for (size_t i = 0; i < child_moves.size(); ++i)
        {
            node->child_move_quality_pairs.emplace_back(child_moves[i], 0);
        }
    }
    else if (board->is_white_turn())
    {
        // Sort moves in descending order, based on their quality to improve alpha-beta pruning
        // effect
        std::sort(
            node->child_move_quality_pairs.begin(), node->child_move_quality_pairs.end(),
            [](const std::pair<Move, float> &a, const std::pair<Move, float> &b)
            { return a.second > b.second; }
        );
    }
    else
    {
        // Sort moves in ascending order, based on their quality to improve alpha-beta pruning
        // effect
        std::sort(
            node->child_move_quality_pairs.begin(), node->child_move_quality_pairs.end(),
            [](const std::pair<Move, float> &a, const std::pair<Move, float> &b)
            { return a.second < b.second; }
        );
    }

    float quality = board->is_white_turn()
                        ? this->max_node(board, max_depth, depth, alpha, beta, node)
                        : this->min_node(board, max_depth, depth, alpha, beta, node);

    node->quality = quality;
    node->depth = depth;

    return quality;
}

float MinMaxAlphaBetaTransTableAgent::max_node(
    Board *board, int max_depth, int depth, float alpha, float beta, s_MinMaxNode *node
)
{
    // We can only use alpha and beta values from previous search branches are the same.
    // So we rely on the actual zobrist key and the last FEN position
    if (node->depth < depth)
    {
        // Compare actual branch alpha with the one from previous search, and keep the lowest
        alpha = max(alpha, node->alpha);
    }

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

        // TODO: Move this line under the if
        best_quality = max(best_quality, child_quality);

        // Update alpha for the next brother nodes
        alpha = max(alpha, best_quality);

        // Alpha-beta pruning - Stop the search when we know the current node won't be chosen
        // - Beta cut : If we're in a max node and the current child max quality is higher than a
        // brother node (which will be chosen by the parent min-node)
        if (beta <= best_quality)
        {
            this->_beta_cutoffs++;
            break;
        }
    }

    node->alpha = max(node->alpha, alpha);

    return best_quality;
}

float MinMaxAlphaBetaTransTableAgent::min_node(
    Board *board, int max_depth, int depth, float alpha, float beta, s_MinMaxNode *node
)
{
    // We can only use alpha and beta values from previous search branches are the same.
    // So we rely on the actual zobrist key and the last FEN position
    if (node->depth < depth)
    {
        // Compare actual branch beta with the one from previous search, and keep the lowest
        beta = min(beta, node->beta);
    }

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

        // TODO: Move this line under the if
        best_quality = min(best_quality, child_quality);

        // Update beta for the next brother nodes
        beta = min(beta, best_quality);

        // Alpha-beta pruning - Stop the search when we know the current node won't be chosen
        // - Alpha cut : If we're in a min node and the current child min quality is lower than a
        // brother node (which will be chosen by the parent max-node)
        if (alpha >= best_quality)
        {
            this->_alpha_cutoffs++;
            break;
        }
    }

    // Update the node beta if a lower value was found
    if (node->depth < depth)
    {
        node->beta = min(node->beta, beta);
    }

    return best_quality;
}

bool MinMaxAlphaBetaTransTableAgent::is_time_up()
{
    return this->elapsed_time() >= this->_ms_turn_stop;
}

float MinMaxAlphaBetaTransTableAgent::elapsed_time()
{
    return (float)(clock() - this->_start_time) / CLOCKS_PER_SEC * 1000;
}
