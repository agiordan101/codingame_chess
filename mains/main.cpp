
#include "../srcs/agents/MinMaxIterDeepAgent.hpp"
#include "../srcs/gameengine/GameEngine.hpp"
#include "../srcs/heuristics/PiecesHeuristic.hpp"
#include "../srcs/players/BotPlayer.hpp"

using namespace std;

int main()
{
    GameEngine *game_engine =
        new GameEngine(new BotPlayer(new MinMaxIterDeepAgent(new PiecesHeuristic(), 50)));
    game_engine->infinite_game_loop();
}
