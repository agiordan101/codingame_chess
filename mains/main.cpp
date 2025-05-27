
#include "../srcs/agents/MctsAgent.hpp"
#include "../srcs/gameengine/GameEngine.hpp"
#include "../srcs/heuristics/PiecesHeuristic.hpp"

using namespace std;

int main()
{
    GameEngine *game_engine = new GameEngine(new MctsAgent(new PiecesHeuristic(), 50));
    game_engine->infinite_game_loop();
}
