#include "Move.hpp"
#include "ChessEngine.hpp"
#include <stdio.h>
#include <string.h>
#include <vector>

class Board {
    /*
        Board represent all FEN data
        White = Upper case
        Black = Lower case
    */

    int         board[8][8];
    bool        white_turn;
    int         castles[4];                 // Column index where the castle is available - 2 first for White and 2 last for Black
    int         kings_initial_columns[2];    // First for white king and second for black king
    int         half_turn_rule;
    int         game_turn;

    vector<Move>    available_moves;

    public:
        int         en_passant_x;
        int         en_passant_y;
        bool        en_passant_available;

        Board(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _full_move);
        void    log();
        void    show_board();
        void    reset_board(string new_fen_board);

        vector<Move>    find_moves();
        void    apply_move(Move move);
        void    next_turn();
        int     is_end_game();
        
        bool    operator ==(string fen_board);

    private:


        string  _fen_board; 
        void    _parse_board(string fen_board);
        void    _parse_castling(string castling_fen);
        void    _parse_en_passant(string _en_passant);

        void    _apply_move(int src_x, int src_y, int dst_x, int dst_y, bool castle, int promotion, bool en_passant);

        vector<Move>    _find_moves_pawns(int x, int y);
        vector<Move>    _find_moves_knights(int x, int y);
        vector<Move>    _find_moves_bishops(int x, int y);
        vector<Move>    _find_moves_rooks(int x, int y);
        vector<Move>    _find_moves_queens(int x, int y);
        vector<Move>    _find_moves_king(int x, int y);

};
