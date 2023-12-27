#include "../srcs/agents/RandomAgent.hpp"
#include "../srcs/gameengine/CGGameEngine.hpp"

using namespace std;

int main()
{
    cerr << "\tChessBot: My chess bot is starting !...!" << endl;

    AbstractAgent *agent = (AbstractAgent *)new RandomAgent();
    BotPlayer *player = new BotPlayer(agent);

    CGGameEngine *game_engine = new CGGameEngine(player, true, true, true);

    game_engine->infinite_game_loop();
}
