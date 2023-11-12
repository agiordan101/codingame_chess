# include "chessengine/Board.hpp"

int main()
{
    // Board *board = new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1");
    Board *board = new Board();
    board->log();

    int nbMove = 1;
    cout << "\nStart " << nbMove << " random move from available ones ..." << endl;
    for (int i = 0; i < nbMove; i++)
    {
        vector<Move> moves = board->find_moves();
        cout << "\nTurn " << i << " - Move available count: " << moves.size() << endl;

        Move move = moves[rand() % moves.size()];
        cout << "Turn " << i << " - Move chose: " << endl;
        move.log();

        board->apply_move(move);

        board->log();

        if (board->is_end_game())
            break;
    }
}