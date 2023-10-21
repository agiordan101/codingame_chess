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

    vector<Move> moves = board->find_moves();

    cout << "Move available count: " << moves.size() << endl;
    for(Move m : moves)
    {
        cout << "Move src_x = " << m.src_x << endl;
        cout << "Move src_y = " << m.src_y << endl;
        cout << "Move dst_x = " << m.dst_x << endl;
        cout << "Move dst_y = " << m.dst_y << endl;
        cout << endl;
    } 
}