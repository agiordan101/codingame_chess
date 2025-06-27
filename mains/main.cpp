
#include "../srcs/agents/MinMaxAlphaBetaAgent.hpp"
#include "../srcs/gameengine/GameEngine.hpp"
#include "../srcs/heuristics/PiecesHeuristic.hpp"

using namespace std;

int main()
{
    GameEngine *game_engine = new GameEngine(new MinMaxAlphaBetaAgent(new PiecesHeuristic(), 25));
    game_engine->infinite_game_loop();
}
