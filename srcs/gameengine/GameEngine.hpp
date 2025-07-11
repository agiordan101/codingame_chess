#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include "../chessengine/Board.hpp"
#include "../srcs/agents/AbstractAgent.hpp"

class GameEngine
{

    public:
        GameEngine(AbstractAgent *agent);
        void infinite_game_loop();

    private:
        // Inputs from CG protocol - First turn
        bool crazy_house;
        int  max_moves;

        // Inputs from CG protocol - Other turns
        bool _lastmove;
        bool _fen;
        bool _moves;
        bool _draw;
        bool _game;
        bool _score;

        clock_t _turn_start_clock;

        AbstractAgent *_agent;
        Board         *_board;
        vector<Move>   _possible_moves;
        int            _possible_moves_count;

        void _parse_first_turn();
        void _parse_turn();
};

#endif