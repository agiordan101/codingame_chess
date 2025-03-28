
#include "../srcs/agents/MinMaxAgent.hpp"
#include "../srcs/gameengine/GameEngineIntTests.hpp"
#include "../srcs/heuristics/PiecesHeuristic.hpp"
#include "../srcs/players/BotPlayer.hpp"

using namespace std;

int main()
{
    GameEngineIntTests *game_engine =
        new GameEngineIntTests(new BotPlayer(new MinMaxAgent(new PiecesHeuristic(), 50)));
    game_engine->infinite_game_loop();
}
