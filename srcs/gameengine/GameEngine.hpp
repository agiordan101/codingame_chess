#ifndef GAMEENGINE_HPP
# define GAMEENGINE_HPP

#include "../chessengine/Board.hpp"
#include "../players/AbstractPlayer.hpp"

class GameEngine {

    public:
        GameEngine(Board* board, AbstractPlayer* white_player, AbstractPlayer* black_player);
        float start_games(int n_games, int progress_bar_size = 40);

    private:

        Board*          _board;
        AbstractPlayer* _white_player;
        AbstractPlayer* _black_player;
        AbstractPlayer* _players[2];
        int             _player_turn; // 0 for white, 1 for black

        float game_loop();

};

#endif