#include "MinMaxIterDeepAgent.hpp"

MinMaxIterDeepAgent::MinMaxIterDeepAgent(AbstractHeuristic *heuristic, int ms_constraint)
{
    this->_heuristic = heuristic;
    this->_ms_constraint = ms_constraint;
    this->_ms_turn_stop = ms_constraint * 0.95;
    this->_depth_max_reached = 0;
    this->_nodes_explored = 0;
    this->_start_time = 0;
}

void MinMaxIterDeepAgent::get_qualities(Board *board, vector<Move> moves, vector<float> *qualities)
{
    Board *new_board;

    this->_start_time = clock();
    this->_nodes_explored = 0;

    // Initialize qualities with 0
    for (int i = 0; i < moves.size(); i++)
        qualities->push_back(0);

    int max_depth = 0;
    while (!this->is_time_up())
    {
        for (int i = 0; i < moves.size(); i++)
        {
            new_board = board->clone();
            new_board->apply_move(moves[i]);

            float move_quality = this->minmax(new_board, max_depth, 0);
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
        cerr << "MinMaxIterDeepAgent: TIMEOUT: dtime=" << dtime << "/" << this->_ms_constraint << "ms" << endl;
}

vector<string> MinMaxIterDeepAgent::get_stats()
{
    vector<string> stats;

    stats.push_back("depth=" + to_string(this->_depth_max_reached));
    stats.push_back("states=" + to_string(this->_nodes_explored));
    cerr << "MinMaxIterDeepAgent: stats=" << stats[0] << " " << stats[1] << endl;
    return stats;
}

string MinMaxIterDeepAgent::get_name()
{
    return "MinMaxIterDeepAgent[" + to_string(this->_ms_constraint) + "ms]." + this->_heuristic->get_name();
}

float MinMaxIterDeepAgent::minmax(Board *board, int max_depth, int depth)
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

float MinMaxIterDeepAgent::max_node(Board *board, vector<Move>* moves, int max_depth, int depth)
{
    // White wants to maximize the heuristic value
    float best_quality = -1;
    for (Move move : *moves)
    {
        Board *new_board = board->clone();
        new_board->apply_move(move);

        float child_quality = this->minmax(new_board, max_depth, depth + 1);
        delete new_board;

        best_quality = max(best_quality, child_quality);

        if (this->is_time_up())
            break;
    }

    return best_quality;
}

float MinMaxIterDeepAgent::min_node(Board *board, vector<Move>* moves, int max_depth, int depth)
{
    // Black wants to minimize the heuristic value
    float best_quality = 1;
    for (Move move : *moves)
    {
        Board *new_board = board->clone();
        new_board->apply_move(move);

        float child_quality = this->minmax(new_board, max_depth, depth + 1);
        delete new_board;

        best_quality = min(best_quality, child_quality);

        if (this->is_time_up())
            break;
    }

    return best_quality;
}

bool MinMaxIterDeepAgent::is_time_up()
{
    // return true;
    return this->elapsed_time() >= this->_ms_turn_stop;
}

float MinMaxIterDeepAgent::elapsed_time()
{
    return (float)(clock() - this->_start_time) / CLOCKS_PER_SEC * 1000;
}

