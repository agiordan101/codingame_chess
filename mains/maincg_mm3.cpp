# include "../srcs/agents/MinMaxAgent.hpp"
# include "../srcs/gameengine/CGGameEngine.hpp"
# include "../srcs/heuristics/PiecesHeuristic.hpp"
# include "../srcs/players/BotPlayer.hpp"

using namespace std;

int main()
{
    CGGameEngine *game_engine = new CGGameEngine(
        new BotPlayer(
            new MinMaxAgent(
                new PiecesHeuristic(),
                3
            )
        )
    );
    game_engine->infinite_game_loop();
}
