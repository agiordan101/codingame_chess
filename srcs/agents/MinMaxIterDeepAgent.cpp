#include "MinMaxIterDeepAgent.hpp"

MinMaxIterDeepAgent::MinMaxIterDeepAgent(AbstractHeuristic *heuristic, int ms_constraint)
{
    this->_heuristic = heuristic;
    this->_ms_constraint = ms_constraint;
}

void MinMaxIterDeepAgent::get_qualities(Board *board, vector<Move> moves, vector<float> *qualities)
{
    Board *new_board;

    this->_start_time = clock();
    for (Move move : moves)
    {
        new_board = board->clone();
        new_board->apply_move(move);

        float move_quality = this->minmax(new_board, 1, -1, 1);
        // cerr << "Move: " << move.to_uci() << " - Quality: " << move_quality << endl;

        qualities->push_back(move_quality);

        delete new_board;
    }
}

string MinMaxIterDeepAgent::get_name()
{
    return "MinMaxIterDeepAgent[" + to_string(this->_ms_constraint) + "ms]." + this->_heuristic->get_name();
}

float MinMaxIterDeepAgent::minmax(Board *board, int depth, float alpha, float beta)
{
    // If we reach the max depth or the game is over, we evaluate the board
    if (this->is_time_up() || board->get_game_state() != GAME_CONTINUE)
        return this->_heuristic->evaluate(board);

    float best_quality;
    float (MinMaxIterDeepAgent::*child_quality_comp)(float, float);
    bool (MinMaxIterDeepAgent::*alphabeta_cut)(float, float *, float *);
    if (board->is_white_turn())
    {
        // White wants to maximize the heuristic value
        best_quality = -1;
        child_quality_comp = &MinMaxIterDeepAgent::max_float;
        alphabeta_cut = &MinMaxIterDeepAgent::beta_cut;
    }
    else
    {
        // Black wants to minimize the heuristic value
        best_quality = 1;
        child_quality_comp = &MinMaxIterDeepAgent::min_float;
        alphabeta_cut = &MinMaxIterDeepAgent::alpha_cut;
    }

    // Go deeper in each child nodes and keep the best one
    float child_quality;
    vector<Move> moves = board->get_available_moves();
    for (Move move : moves)
    {
        Board *new_board = board->clone();
        new_board->apply_move(move);

        // Compute child best quality
        child_quality = this->minmax(new_board, depth + 1, alpha, beta);

        // Update best quality depending on the current node type - Maximize or minimize
        best_quality = (this->*child_quality_comp)(best_quality, child_quality);

        if (this->is_time_up())
            return best_quality;

        // Alpha-beta pruning - Stop the search when we know the current node won't be chosen - 2 case :
        // - Alpha cut : If we're in a min node and the current child min quality is lower than a brother node
        // - Beta cut : If we're in a max node and the current child max quality is higher than a brother node
        if ((this->*alphabeta_cut)(best_quality, &alpha, &beta))
            return best_quality;

        delete new_board;
    }

    return best_quality;
}

float MinMaxIterDeepAgent::max_float(float a, float b) {
    return max(a, b);
}

float MinMaxIterDeepAgent::min_float(float a, float b) {
    return min(a, b);
}

bool MinMaxIterDeepAgent::alpha_cut(float best_quality, float *alpha, float *beta)
{
    if (*alpha >= best_quality)
        return true;

    // Update beta for the next brother nodes
    *beta = min(*beta, best_quality);
    return false;
}

bool MinMaxIterDeepAgent::beta_cut(float best_quality, float *alpha, float *beta)
{
    if (*beta <= best_quality)
        return true;

    // Update alpha for the next brother nodes
    *alpha = max(*alpha, best_quality);
    return false;
}

bool MinMaxIterDeepAgent::is_time_up()
{
    // return true;
    float elapsed_time = (float)(clock() - this->_start_time) / CLOCKS_PER_SEC * 1000;
    return elapsed_time >= (float)this->_ms_constraint * 0.95;
}
