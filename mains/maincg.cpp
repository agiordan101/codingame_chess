# include "../srcs/agents/HeuristicAgent.hpp"
# include "../srcs/agents/MinMaxAgent.hpp"
# include "../srcs/agents/RandomAgent.hpp"
# include "../srcs/gameengine/CGGameEngine.hpp"
# include "../srcs/heuristics/PiecesHeuristic.hpp"

using namespace std;

int main()
{
    cerr << "\tChessBot: My chess bot is starting !...!" << endl;

    AbstractHeuristic *heuristic = (AbstractHeuristic *)new PiecesHeuristic();

    // AbstractAgent *agent = (AbstractAgent *)new RandomAgent();
    // AbstractAgent *agent = (AbstractAgent *)new HeuristicAgent(heuristic);
    AbstractAgent *agent = (AbstractAgent *)new MinMaxAgent(heuristic, 3);

    BotPlayer *player = new BotPlayer(agent);
    CGGameEngine *game_engine = new CGGameEngine(player);

    game_engine->infinite_game_loop();
}
