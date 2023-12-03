# include "srcs/chessengine/Board.hpp"
#include "srcs/gameengine/GameEngine.hpp"
#include "srcs/agents/RandomAgent.hpp"

// int main()
// {
//     // Board *board = new Board("3Nr3/p4P1k/P7/3P2p1/3b1R1p/8/2K4P/N5R1 w - - 6 65");
//     // // Board *board = new Board("bbqrkrnn/pppppppp/8/8/P7/8/1PPPPPPP/BBQRKRNN b DFdf a3 0 1");
//     // board->log();

//     // int nbMove = 1;
//     // float game_state = board->game_state();
//     // cout << "\nStart " << nbMove << " random move from available ones ..." << endl;
//     // for (int i = 0; i < nbMove; i++)
//     // {
//     //     vector<Move> moves = board->find_moves();
//     //     cout << "\nTurn " << i << " - Move available count: " << moves.size() << endl;

//     //     if (game_state != -1)
//     //     {
//     //         cout << "\n\n---- Game over: " << game_state << endl;
//     //         break;
//     //     }
//     //     if (moves.size() == 0)
//     //     {
//     //         cout << "\n\n---- NO MOVES BUT GAME STILL RUNNING ---- " << endl;
//     //         break;
//     //     }

//     //     // Display all moves 
//     //     for (int j = 0; j < moves.size(); j++)
//     //     {
//     //         if (moves[j].promotion)
//     //         {
//     //             cout << "Available move " << j << ": " << moves[j].to_uci() << endl;
//     //             moves[j].log();
//     //         }
//     //     }

//     //     Move move = moves[rand() % moves.size()];
//     //     cout << "Turn " << i << " - Move chose: " << endl;
//     //     move.log();

//     //     board->apply_move(move);

//     //     board->log();

//     //     game_state = board->game_state();
//     //     if (game_state != -1)
//     //     {
//     //         cout << "\n\n---- Game over: " << game_state << endl;
//     //         break;
//     //     }
//     // }
// }

int main()
{
    Board *board = new Board();
    AbstractPlayer *p1 = (AbstractPlayer *)new RandomAgent();
    AbstractPlayer *p2 = (AbstractPlayer *)new RandomAgent();
    GameEngine *game_engine = new GameEngine(board, p1, p2);

    float white_score = game_engine->start_games(3);
    cout << "Final white score: " << white_score << endl;

    return 0;
}