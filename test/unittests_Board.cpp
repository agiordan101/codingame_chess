#include "../header/main.hpp"

int    constructor_unittest()
{
    string fen_board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";;
    string color = "w";
    string castling = "ahAH";
    string en_passant = "e2e4";
    int half_move_clock = 0;
    int full_move = 1;
    
    cerr << "\n[UNITTEST] Board - constructor_unittest() is starting " << endl;
    Board *board = new Board(fen_board, color, castling, en_passant, half_move_clock, full_move);
    board->log();

    return 0;
}

int main()
{
    int errors = 0;

    cerr << "\n[UNITTEST] Board - Start unit tests " << endl;
    errors += constructor_unittest();

    cerr << "\n[UNITTEST] Board - Errors count: " << errors << endl;
}
