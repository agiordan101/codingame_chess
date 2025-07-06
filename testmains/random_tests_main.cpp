#include "../srcs/chessengine/Board.hpp"
#include "../srcs/heuristics/PiecesHeuristic.hpp"
#include "../srcs/agents/MinMaxAlphaBetaAgent.hpp"

int main()
{
    // Why it doesn't take mat in 1 ?
    // Board                *board = new Board("4q3/8/6r1/3k3K/8/8/8/2n5 w - - 60 2");

    // Why promote in knight ?
    Board                *board = new Board("8/5q2/6r1/3k3K/8/8/2p5/8 w - - 60 0");

    // Why does it not understand draw tfr ?
    // Board                *board = new Board("1q6/5K2/8/3k4/8/8/8/2n5 w - - 3 6");
    // board->apply_move(Move("f7e7")); // First seen

    // board->apply_move(Move("b8a8"));
    // board->apply_move(Move("e7d7"));
    // board->apply_move(Move("a8b8"));
    // board->apply_move(Move("d7e7")); // Second seen

    // board->apply_move(Move("b8a8"));
    // board->apply_move(Move("e7d7"));
    // board->apply_move(Move("a8b8"));

    MinMaxAlphaBetaAgent *agent = new MinMaxAlphaBetaAgent(new PiecesHeuristic(), 25);

    board->log(false);

    int turn_played = 0;
    float game_state = GAME_CONTINUE;
    while (game_state == GAME_CONTINUE)
    {
        vector<Move>   moves = board->get_available_moves();
        vector<float> *qualities = new vector<float>(moves.size());
        agent->get_qualities(board, moves, qualities);

        cerr << "\n---\n" << endl;
        board->log(false);
        cerr << "Moves available (" << moves.size() << ") :" << endl;
        for (size_t i = 0; i < moves.size(); i++)
        {
            cerr << moves[i].to_uci() << " : " << qualities->at(i) << endl;
        }

        // White wants to maximize, black wants to minimize
        auto comparator = board->is_white_turn()
                              ? std::max_element(qualities->begin(), qualities->end())
                              : std::min_element(qualities->begin(), qualities->end());

        // Get the index of the result
        size_t selectedIndex = std::distance(qualities->begin(), comparator);

        Move move = moves[selectedIndex];

        cerr << "Move chosen: " << move.to_uci() << endl;
        board->apply_move(move);

        game_state = board->get_game_state();
        turn_played++;

        // if (turn_played > 0)
        //     exit(0);
    }

    cerr << "\nGameEngine: Game is over : " << game_state << endl;
}