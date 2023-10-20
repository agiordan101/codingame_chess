#include "Move.hpp"
#include "ChessEngine.hpp"
#include <stdio.h>
#include <string.h>

class Board {
    /*
        Board represent all FEN data
        White =  1 = Upper case
        Black = -1 = Lower case
    */

    int         board[8][8];
    int         player_turn;
    int         castles[4];     // 2 first for White | 2 last for Black
    string      en_passant;
    int         half_turn_remaning;
    int         game_turn;
    Move        moves[100];
    int                 moves_count;

    public:

        Board(string _board, string _color, string _castling, string _en_passant, int _half_move_clock, int _full_move);
        void    log();
        void    show_board();
        void    reset_board(string new_fen_board);

        void    find_moves();
        void    apply_move(int src_x, int src_y, int dst_x, int dst_y, bool castle, int promotion, bool en_passant);
        
        bool    operator ==(string fen_board);

    private:

        string  _fen_board; 
        void    _parse_board(string fen_board);
        void    _parse_castling(string castling_fen);
        
        void    _find_moves_pawns(int x, int y);
        void    _find_moves_knights(int x, int y);
        void    _find_moves_bishops(int x, int y);
        void    _find_moves_rooks(int x, int y);
        void    _find_moves_queens(int x, int y);
        void    _find_moves_king(int x, int y);

};
