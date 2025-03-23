#include "MinMaxAgent.hpp"

MinMaxAgent::MinMaxAgent(AbstractHeuristic *heuristic, int ms_constraint)
{
    this->_heuristic = heuristic;
    this->_ms_constraint = ms_constraint;
    this->_ms_turn_stop = ms_constraint * 0.95;
    this->_depth_reached = 0;
    this->_nodes_explored = 0;
    this->_start_time = 0;
}

void MinMaxAgent::get_qualities(Board *board, vector<Move> moves, vector<float> *qualities)
{
    this->_start_time = clock();

    // Debugging information for CG
    cerr << std::bitset<64>(board->get_castling_rights()) << endl;

    for (size_t i = 0; i < moves.size(); i++)
        qualities->push_back(0);

    // If the max_depth is too low, it would recalculate too much moves !
    // All position at depth max_depth MUST be computed, else some moves won't have a quality
    int max_depth = 2;
    this->_nodes_explored = 0;
    while (!this->is_time_up())
    {
        for (size_t i = 0; i < moves.size(); i++)
        {
            Board new_board = *board;
            new_board.apply_move(moves[i]);

            float move_quality = this->minmax(&new_board, max_depth, 1);

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
        cerr << "MinMaxAgent: TIMEOUT: dtime=" << dtime << "/" << this->_ms_constraint << "ms"
             << endl;
}

vector<string> MinMaxAgent::get_stats()
{
    vector<string> stats;

    stats.push_back("version=BbMmPv-5");
    stats.push_back("depth=" + to_string(this->_depth_reached));
    stats.push_back("states=" + to_string(this->_nodes_explored));
    cerr << "BbMmPv-5\t: stats=" << stats[0] << " " << stats[1] << " " << stats[2] << endl;
    return stats;
}

string MinMaxAgent::get_name()
{
    return Board::get_name() + ".MinMaxAgent[" + to_string(this->_ms_constraint) + "ms]." +
           this->_heuristic->get_name();
}

float MinMaxAgent::minmax(Board *board, int max_depth, int depth)
{
    this->_nodes_explored++;

    // If we reach the max depth or the game is over, we evaluate the board
    if (depth == max_depth || this->is_time_up() || board->get_game_state() != GAME_CONTINUE)
        return this->_heuristic->evaluate(board);

    vector<Move> moves = board->get_available_moves();

    // Go deeper in each child nodes and keep the best one
    float best_quality;
    if (board->is_white_turn())
    {
        best_quality = this->max_node(board, &moves, max_depth, depth);
    }
    else
    {
        best_quality = this->min_node(board, &moves, max_depth, depth);
    }

    return best_quality;
}

float MinMaxAgent::max_node(Board *board, vector<Move> *moves, int max_depth, int depth)
{
    // White wants to maximize the heuristic value
    float best_quality = -1;
    for (Move move : *moves)
    {
        Board new_board = *board;
        new_board.apply_move(move);

        float child_quality = this->minmax(&new_board, max_depth, depth + 1);

        best_quality = max(best_quality, child_quality);

        if (this->is_time_up())
            break;
    }

    return best_quality;
}

float MinMaxAgent::min_node(Board *board, vector<Move> *moves, int max_depth, int depth)
{
    // Black wants to minimize the heuristic value
    float best_quality = 1;
    for (Move move : *moves)
    {
        Board new_board = *board;
        new_board.apply_move(move);

        float child_quality = this->minmax(&new_board, max_depth, depth + 1);

        best_quality = min(best_quality, child_quality);

        if (this->is_time_up())
            break;
    }

    return best_quality;
}

bool MinMaxAgent::is_time_up()
{
    return this->elapsed_time() >= this->_ms_turn_stop;
}

float MinMaxAgent::elapsed_time()
{
    return (float)(clock() - this->_start_time) / CLOCKS_PER_SEC * 1000;
}
