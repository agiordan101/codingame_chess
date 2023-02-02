#include "../header/main.hpp"

Board *constructor_unittest()
{
    string fen_board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";;
    string color = "w";
    string castling = "ahAH";
    string en_passant = "e2e4";
    int half_move_clock = 0;
    int full_move = 1;
    
    cerr << "\n[UNITTEST] Board - constructor_unittest() is starting " << endl;
    cerr << "\n[UNITTEST] Board - FEN: " << fen_board << endl;
    Board *board = new Board(fen_board, color, castling, en_passant, half_move_clock, full_move);
    board->log();
    board->show_board();

    return board;
}

int     apply_move_unittest(Board *board)
{
    cerr << "\n[UNITTEST] Board - apply_move_unittest() is starting " << endl;
    // board->apply_move();

    return 0;
}

int main()
{
    int errors = 0;

    cerr << "\n[UNITTEST] Board - Start unit tests " << endl;
    Board *board = constructor_unittest();
    // apply_move_unittest(board);

    cerr << "\n[UNITTEST] Board - Errors count: " << errors << endl;
}
