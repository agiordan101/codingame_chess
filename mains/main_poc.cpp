
#include "../srcs/agents/MctsAgent.hpp"
#include "../srcs/agents/MinMaxAgent.hpp"
#include "../srcs/gameengine/GameEngine.hpp"
#include "../srcs/heuristics/PiecesHeuristic.hpp"

using namespace std;

int main()
{
    MctsAgent *agent = new MctsAgent(new PiecesHeuristic(), 50);

    Board *board = new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1");

    float game_state = GAME_CONTINUE;
    while (game_state == GAME_CONTINUE)
    {
        vector<Move> moves = board->get_available_moves();
        Move         move = agent->choose_from(board, moves);

        cerr << "Move: " << move.to_uci() << endl;
        board->apply_move(move);
        board->log();

        game_state = board->get_game_state();
    }

    cerr << "\nGameEngine: Game is over : " << game_state << endl;

    return 0;
}
