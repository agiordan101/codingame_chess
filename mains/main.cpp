
#include "../srcs/agents/MinMaxAlphaBetaAgent.hpp"
#include "../srcs/gameengine/GameEngine.hpp"
#include "../srcs/heuristics/PiecesHeuristic.hpp"
#include "../srcs/players/BotPlayer.hpp"

using namespace std;

int main()
{
    GameEngine *game_engine =
        new GameEngine(new BotPlayer(new MinMaxAlphaBetaAgent(new PiecesHeuristic(), 10)));
    game_engine->infinite_game_loop();
}
