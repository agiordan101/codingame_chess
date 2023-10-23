# include "chessengine/Board.hpp"

int main()
{
    int successCount = 0;
    string fen_board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    string color = "w";
    string castling = "ahAH";
    string en_passant = "e2e4";
    int half_move_clock = 0;
    int full_move = 1;
    
    Board *board = new Board(fen_board, color, castling, en_passant, half_move_clock, full_move);

    cout << "Start 10 random move from available ones ..." << endl;
    for (int i = 0; i < 10; i++)
    {
        vector<Move> moves = board->find_moves();

        cout << "Turn " << i << " - Move available count: " << moves.size() << endl;
        for(Move m : moves)
        {
            cout << endl;
        } 

        if (board->is_end_game())
            break;

        Move move = moves[rand() % 10];
        board->apply_move(move);

        board->log();
    }
}