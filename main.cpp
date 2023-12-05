# include "srcs/chessengine/Board.hpp"
# include "srcs/gameengine/GameEngine.hpp"
# include "srcs/agents/RandomAgent.hpp"

int main()
{
    srand(time(0));

    Board *board = new Board();
    AbstractPlayer *p1 = (AbstractPlayer *)new RandomAgent();
    AbstractPlayer *p2 = (AbstractPlayer *)new RandomAgent();
    GameEngine *game_engine = new GameEngine(board, p1, p2);

    float white_score = game_engine->start_games(100);
    cerr << "Final white score: " << white_score << endl;

    return 0;
}