
#include "../srcs/chessengine/Board.hpp"

int main()
{
    Board *board = new Board("b4r2/r3np2/2p1k2b/p1P1p1Q1/K1P1P2p/3R1B2/1q3P2/1N1R4 b - - 0 15");

    bool draw;

    cerr << "Starting test" << endl;

    // clock_t start = clock();
    // for (int i = 0; i < 20000; i++)
    //     draw = board->_threefold_repetition_rule_lazy();
    // clock_t end = clock();

    // cerr << "Time of board->_threefold_repetition_rule_lazy() 15: "
    //      << (end - start) / (double)CLOCKS_PER_SEC * 1000 << "ms" << endl;

    // start = clock();
    // for (int i = 0; i < 20000; i++)
    //     draw = board->_threefold_repetition_rule();
    // end = clock();

    // cerr << "Time of board->_threefold_repetition_rule() 15: "
    //      << (end - start) / (double)CLOCKS_PER_SEC * 1000 << "ms" << endl;

    // board = new Board("b4r2/r3np2/2p1k2b/p1P1p1Q1/K1P1P2p/3R1B2/1q3P2/1N1R4 b - - 0 75");

    // start = clock();
    // for (int i = 0; i < 20000; i++)
    //     draw = board->_threefold_repetition_rule();
    // end = clock();

    // cerr << "Time of board->_threefold_repetition_rule() 75: "
    //      << (end - start) / (double)CLOCKS_PER_SEC * 1000 << "ms" << endl;

    // cerr << "Game state: " << draw << endl;

    // float game_state = GAME_CONTINUE;
    // while (game_state == GAME_CONTINUE)
    // {
    //     vector<Move> moves = board->get_available_moves();
    //     Move         move = agent->choose_from(board, moves);

    //     cerr << "Move: " << move.to_uci() << endl;
    //     board->apply_move(move);
    //     board->log();

    //     game_state = board->get_game_state();
    // }

    // cerr << "\nGameEngine: Game is over : " << game_state << endl;

    return 0;
}
