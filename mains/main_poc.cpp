
#include "../srcs/agents/MinMaxAgent.hpp"
#include "../srcs/gameengine/GameEngine.hpp"
#include "../srcs/heuristics/PiecesHeuristic.hpp"
#include "../srcs/players/BotPlayer.hpp"

using namespace std;

int main()
{
    Board *board = new Board();
    board->apply_move(Move("d2d4"));
    vector<Move> moves = board->get_available_moves();

    // cerr << "Available board moves: " << board->available_moves.size() << endl;
    // for (Move move : board->available_moves)
    // {
    //     cerr << move.to_uci() << endl;
    // }
    // cerr << endl;

    // cerr << "Fen history index: " << board->fen_history_index << endl;
    // cerr << board->fen_history[board->fen_history_index - 1] << endl;
    // cerr << board->fen_history[board->fen_history_index - 2] << endl;

    Board boardcpy = *board;
    boardcpy.apply_move(Move("e7e5"));
    vector<Move> movescpy = boardcpy.get_available_moves();

    // cerr << "Fen history index: " << boardcpy.fen_history_index << endl;
    // cerr << boardcpy.fen_history[boardcpy.fen_history_index - 1] << endl;
    // cerr << boardcpy.fen_history[boardcpy.fen_history_index - 2] << endl;

    // cerr << "Fen history index: " << board->fen_history_index << endl;
    // cerr << board->fen_history[board->fen_history_index] << endl;
    // cerr << board->fen_history[board->fen_history_index - 1] << endl;
    // cerr << board->fen_history[board->fen_history_index - 2] << endl;

    // cerr << "Available board copy moves count: " << boardcpy.available_moves.size() << endl;
    // for (Move move : boardcpy.available_moves)
    // {
    //     cerr << move.to_uci() << endl;
    // }
    // cerr << endl;

    // cerr << "Available board moves: " << board->available_moves.size() << endl;
    // for (Move move : board->available_moves)
    // {
    //     cerr << move.to_uci() << endl;
    // }
    // cerr << endl;

    board->log();
    boardcpy.log();

    return 0;
}
