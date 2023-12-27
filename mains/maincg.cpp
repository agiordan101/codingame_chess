# include "../srcs/agents/HeuristicAgent.hpp"
# include "../srcs/agents/RandomAgent.hpp"
# include "../srcs/gameengine/CGGameEngine.hpp"
# include "../srcs/heuristics/PiecesHeuristic.hpp"

using namespace std;

int main()
{
    cerr << "\tChessBot: My chess bot is starting !...!" << endl;

    // AbstractAgent *agent = (AbstractAgent *)new RandomAgent();
    // BotPlayer *player = new BotPlayer(agent);

    AbstractHeuristic *heuristic = (AbstractHeuristic *)new PiecesHeuristic();
    AbstractAgent *agent = (AbstractAgent *)new HeuristicAgent(heuristic);
    BotPlayer *player = new BotPlayer(agent);

    CGGameEngine *game_engine = new CGGameEngine(player, false, true);

    game_engine->infinite_game_loop();
}
