# include "chessengine/Board.hpp"

int main()
{
    // Board *board = new Board("8/2Q5/k3P3/1R3p2/P4P2/8/8/2N4K b - - 6 83");
    Board *board = new Board("bbqrkrnn/pppppppp/8/8/P7/8/1PPPPPPP/BBQRKRNN b DFdf a3 0 1");
    board->log();

    int nbMove = 1000;
    int game_state = board->game_state();
    cout << "\nStart " << nbMove << " random move from available ones ..." << endl;
    for (int i = 0; i < nbMove; i++)
    {
        vector<Move> moves = board->find_moves();
        cout << "\nTurn " << i << " - Move available count: " << moves.size() << endl;

        if (game_state != -1)
        {
            cout << "\n\n---- Game over: " << game_state << endl;
            break;
        }
        if (moves.size() == 0)
        {
            cout << "\n\n---- NO MOVES BUT GAME STILL RUNNING ---- " << endl;
            break;
        }

        Move move = moves[rand() % moves.size()];
        cout << "Turn " << i << " - Move chose: " << endl;
        move.log();

        board->apply_move(move);

        board->log();

        game_state = board->game_state();
        if (game_state != -1)
        {
            cout << "\n\n---- Game over: " << game_state << endl;
            break;
        }

        // sleep(1);
    }
}