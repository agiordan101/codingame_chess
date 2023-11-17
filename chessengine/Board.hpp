#include "Move.hpp"
#include "ChessEngine.hpp"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <bits/stdc++.h>

// Because of the Fifty-Move rule, a game cannot exceed 50 moves without a capture
// So we can assume that a position cannot be repeated at more than 50 moves away
# define FEN_HISTORY_SIZE 50

class Board {
    /*
        Board represent all FEN data :
            rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1

        White = Upper case = First indexes
        Black = Lower case = Last indexes
    */

    // FEN data:
    int         board[8][8];
    bool        white_turn;
    int         castles[4];                 // Column index where the castle is available - 2 first for White and 2 last for Black
    int         kings_initial_columns[2];   // First for white king and second for black king
    int         en_passant_x;               // En passant coordinates are created in apply_move() after a pawn move of 2 squares
    int         en_passant_y;
    bool        en_passant_available;       // En passant availability is set to TRUE in end_turn() after a pawn move of 2 squares
    int         half_turn_rule;             // Number of half-turn since the last capture or pawn move (Fifty-Move rule)
    int         game_turn;                  // Game turn, incremented after each black move

    // Available moves are created in find_moves() or is_end_game() calls. And reseted after apply_move()
    vector<Move>    available_moves;

    // FEN history is used to check the Threefold Repetition rule
    // Each FEN is saved in the history after each move
    string         fen_history[FEN_HISTORY_SIZE];
    int            fen_history_index;

    public:

        Board(string _fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1");
        Board(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _full_move);
        void    log();
        void    show_board();

        vector<Move>    find_moves();
        void    apply_move(Move move);
        int     is_end_game();
        string  create_fen();
        
        bool    operator ==(Board *test_board);

    private:

        void    _main_parsing(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _game_turn);
        void    _parse_board(string fen_board);
        void    _parse_castling(string castling_fen);
        void    _parse_en_passant(string _en_passant);

        void    _apply_move(int src_x, int src_y, int dst_x, int dst_y, bool castle, char promotion, bool en_passant);
        void    _end_turn();
        void    _update_fen_history();

        vector<Move>    _find_moves_pawns(int x, int y);
        vector<Move>    _find_moves_knights(int x, int y);
        vector<Move>    _find_moves_bishops(int x, int y);
        vector<Move>    _find_moves_rooks(int x, int y);
        vector<Move>    _find_moves_queens(int x, int y);
        vector<Move>    _find_moves_king(int x, int y);
};
