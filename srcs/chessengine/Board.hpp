
#ifndef BOARD_HPP
# define BOARD_HPP

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
    char        board[8][8];
    bool        white_turn;
    int         castles[4];                 // Column index where the castle is available - 2 first for White and 2 last for Black
    int         kings_initial_columns[2];   // First for white king and second for black king
    int         en_passant_x;               // En passant coordinates & availability are created after a pawn move of 2 squares
    int         en_passant_y;
    bool        en_passant_available;
    int         half_turn_rule;             // Number of half-turn since the last capture or pawn move (Fifty-Move rule)
    int         game_turn;                  // Game turn, incremented after each black move

    int         game_turn_max = 125;

    // FEN history is used to check the Threefold Repetition rule
    // Each FEN is saved in the history after each move
    string         fen_history[FEN_HISTORY_SIZE];
    int            fen_history_index;

    public:

        vector<Move>    available_moves;
        bool            moves_found;
        bool            check;

        Board(string _fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1", bool chess960_rule = true);
        Board(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _full_move, bool chess960_rule = true);

        void            log();
        vector<Move>    find_moves();
        void            apply_move(Move move);
        float           game_state(); // -1 = Game continue | 0 = Current player lose | 0.5 = Draw | 1 = Current player win
        bool            is_check();
        string          create_fen(bool with_turns = true);
        
        bool    operator ==(Board *test_board);

    private:

        // Function pointer to apply castle depending on the chess960 rule
        bool    chess960_rule;
        bool    (Board::*_handle_castle)(int, int, int, int);

        void    _main_parsing(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _game_turn, bool chess960_rule);
        void    _parse_board(string fen_board);
        void    _parse_castling(string castling_fen);
        void    _parse_en_passant(string _en_passant);

        void    _apply_move(int src_x, int src_y, int dst_x, int dst_y, char promotion);
        bool    _handle_standard_castle(int src_x, int src_y, int dst_x, int _);
        bool    _handle_chess960_castle(int src_x, int src_y, int dst_x, int dst_y);
        void    _apply_castle(int src_x, int src_y, int dst_x, int dst_y);

        void    _update_en_passant();
        void    _update_castling_rights();
        void    _update_fen_history();

        bool    _threefold_repetition_rule();
        bool    _insufficient_material_rule();

        void    _find_moves_pawns(int x, int y);
        void    _add_regular_move_or_promotion(int x, int y, int dx, int dy, int (*case_func)(int));
        void    _find_moves_knights(int x, int y);
        void    _find_moves_bishops(int x, int y);
        void    _find_moves_rooks(int x, int y);
        void    _find_moves_queens(int x, int y);
        void    _find_moves_king(int x, int y);
        void    _find_moves_castle(int x, int y, int castle_index);
        bool    _is_castle_legal(int src_x, int src_y, int dst_x, int trajectory_dx);

        void    _filter_non_legal_moves();
        bool    _is_check();
        bool    _is_check(int x, int y);
};

#endif