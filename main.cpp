# include "srcs/chessengine/Board.hpp"
# include "srcs/gameengine/GameEngine.hpp"
# include "srcs/agents/RandomAgent.hpp"

int main()
{
    srand(time(0));

    Board *board = new Board();
    AbstractAgent *a1 = (AbstractAgent *)new RandomAgent();
    AbstractAgent *a2 = (AbstractAgent *)new RandomAgent();
    AbstractPlayer *p1 = (AbstractPlayer *)new BotPlayer(a1);
    AbstractPlayer *p2 = (AbstractPlayer *)new BotPlayer(a2);
    GameEngine *game_engine = new GameEngine(board, p1, p2);

    float white_score = game_engine->start_games(1000);
    cout << "Final white score: " << white_score << endl;

    return 0;
}