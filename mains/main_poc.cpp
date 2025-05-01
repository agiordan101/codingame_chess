
#include "../srcs/agents/MctsAgent.hpp"
#include "../srcs/agents/MinMaxAgent.hpp"
#include "../srcs/gameengine/GameEngine.hpp"
#include "../srcs/heuristics/PiecesHeuristic.hpp"
#include "../srcs/players/BotPlayer.hpp"

using namespace std;

int main()
{
    BotPlayer *player = new BotPlayer(new MctsAgent(new PiecesHeuristic(), 50));

    Board *board = new Board();

    vector<Move> moves = board->get_available_moves();
    Move         move = player->choose_from(board, moves);

    cerr << "Move: " << move.to_uci() << endl;
    board->log();

    return 0;
}
