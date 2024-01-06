# include "../srcs/agents/RandomAgent.hpp"
# include "../srcs/agents/HeuristicAgent.hpp"
# include "../srcs/agents/MinMaxAgent.hpp"
# include "../srcs/chessengine/Board.hpp"
# include "../srcs/gameengine/GameEngine.hpp"
# include "../srcs/heuristics/PiecesHeuristic.hpp"
# include "../srcs/players/BotPlayer.hpp"

int main(int argc, char **argv)
{
    int n_games = 10;

    if (argc == 2)
    {
        n_games = atoi(argv[1]);
    }
    else if (argc > 2)
    {
        cerr << "Usage: 1 parameter representing the number of games" << endl;
        return 1;
    }

    // int t = 1702844902;
    int t = time(0);
    srand(t);
    cerr << "Seed: " << t << endl;

    Board *board = new Board();

    // ----------------------------------------------

    AbstractHeuristic *h1 = (AbstractHeuristic *)new PiecesHeuristic();

    // AbstractAgent *a1 = (AbstractAgent *)new RandomAgent();
    // AbstractAgent *a1 = (AbstractAgent *)new HeuristicAgent(h1);
    AbstractAgent *a1 = (AbstractAgent *)new MinMaxAgent(h1, 2);

    // ----------------------------------------------

    AbstractHeuristic *h2 = (AbstractHeuristic *)new PiecesHeuristic();

    // AbstractAgent *a2 = (AbstractAgent *)new RandomAgent();
    AbstractAgent *a2 = (AbstractAgent *)new HeuristicAgent(h2);
    // AbstractAgent *a2 = (AbstractAgent *)new MinMaxAgent(h2, 3);

    // ----------------------------------------------

    AbstractPlayer *p1 = (AbstractPlayer *)new BotPlayer(a1);
    AbstractPlayer *p2 = (AbstractPlayer *)new BotPlayer(a2);
    GameEngine *game_engine;

    game_engine = new GameEngine(board, p1, p2);
    game_engine->start_games(n_games);
    delete game_engine;

    game_engine = new GameEngine(board, p2, p1);
    game_engine->start_games(n_games);
    delete game_engine;

    // delete a1;
    // delete p1;
    // delete a2;
    // delete p2;
    // delete h2;
    // delete board;

    return 0;
}