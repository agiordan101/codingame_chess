#ifndef CGGAMEENGINE_HPP
# define CGGAMEENGINE_HPP

#include "../chessengine/Board.hpp"
#include "../players/BotPlayer.hpp"

class CGGameEngine {

    public:
        CGGameEngine(BotPlayer* player, bool lastmove = true, bool fen = false, bool moves = false, bool draw = false, bool game = false, bool score = false);
        void infinite_game_loop();
    
    private:

        BotPlayer*  _player;
        bool        _lastmove;
        bool        _fen;
        bool        _moves;
        bool        _draw;
        bool        _game;
        bool        _score;

        Move            *_last_move;
        Board           *_board;
        Board           *_cg_board;
        vector<Move>    _possible_moves;
        int             _possible_moves_count;

        void    parse_first_turn();
        void    set_requested_inputs();
        void    parse_turn();
};

#endif