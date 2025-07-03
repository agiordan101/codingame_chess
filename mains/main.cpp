
#include "../srcs/agents/MinMaxAlphaBetaAgent.hpp"
#include "../srcs/gameengine/GameEngine.hpp"
#include "../srcs/heuristics/BestHeuristic.hpp"

using namespace std;

int main()
{
    GameEngine *game_engine = new GameEngine(new MinMaxAlphaBetaAgent(new BestHeuristic(), 25));
    game_engine->infinite_game_loop();
}
