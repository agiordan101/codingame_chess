#ifndef GAMEENGINEINTTESTS_HPP
# define GAMEENGINEINTTESTS_HPP

#include "../chessengine/Board.hpp"
#include "../players/BotPlayer.hpp"

class GameEngineIntTests {

    public:
        GameEngineIntTests(BotPlayer* player);
        void infinite_game_loop();
    
    private:

        BotPlayer*  _player;
        bool        _lastmove;
        bool        _fen;
        bool        _moves;
        bool        _draw;
        bool        _game;
        bool        _score;

        Move            *_cg_last_move;
        Board           *_cg_board;
        Board           *_board;
        vector<Move>    _possible_moves;
        int             _possible_moves_count;

        void    _parse_first_turn();
        void    _parse_turn();
};

#endif