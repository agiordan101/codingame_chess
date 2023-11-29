# include "chessengine/Board.hpp"

int main()
{
    Board *board = new Board();
    board->log();

    int nbMove = 1000;
    cout << "\nStart " << nbMove << " random move from available ones ..." << endl;
    for (int i = 0; i < nbMove; i++)
    {
        vector<Move> moves = board->find_moves();
        cout << "\nTurn " << i << " - Move available count: " << moves.size() << endl;

        if (moves.size() == 0)
            break;

        Move move = moves[rand() % moves.size()];
        cout << "Turn " << i << " - Move chose: " << endl;
        move.log();

        board->apply_move(move);

        board->log();

        int game_state = board->game_state();
        if (game_state != -1)
        {
            cout << "\n\n---- Game over: " << game_state << endl;
            break;
        }

        sleep(1);
    }
}