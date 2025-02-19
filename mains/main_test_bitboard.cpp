# include "../srcs/chessengine/Board.hpp"

using namespace std;

int main()
{
    cerr << "Standard rules ->" << endl;
    Board *board = new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", false);
    board->log();

    cerr << "Chess960 rules ->" << endl;
    board = new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1", true);
    board->log();
}
