
#ifndef BOARD_HPP
# define BOARD_HPP

// --- BASIC BOARD IMPLEMENTATION---

# pragma region Board

# include "Move.hpp"
# include "ChessEngine.hpp"
# include <stdio.h>
# include <string.h>
# include <vector>
# include <bits/stdc++.h>

// Because of the Fifty-Move rule, a game cannot exceed 50 moves without a capture
// So we can assume that a position cannot be repeated at more than 50 moves away
# define FEN_HISTORY_SIZE 50

# if BITBOARD_IMPLEMENTATION == 1

class Board {
    /*
    Board represent all FEN data :
    rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1
    
    White = Upper case = First indexes
    Black = Lower case = Last indexes
    */

    bool        chess960_rule;

    // FEN data: Pieces
    uint64_t    white_pawns;
    uint64_t    white_knights;
    uint64_t    white_bishops;
    uint64_t    white_rooks;
    uint64_t    white_queens;
    uint64_t    white_king;
    uint64_t    black_pawns;
    uint64_t    black_knights;
    uint64_t    black_bishops;
    uint64_t    black_rooks;
    uint64_t    black_queens;
    uint64_t    black_king;
    
    // FEN data: Player turn
    bool        white_turn;
    
    // FEN data: Castling info
    uint64_t        white_castles;              // Positions where the castle is available
    uint64_t        black_castles;
    
    // FEN data: en passant and turns
    uint64_t        en_passant;                 // En passant position is created after a pawn move of 2 squares. 0 means no en passant available
    uint64_t        next_turn_en_passant;                 // En passant position is created after a pawn move of 2 squares. 0 means no en passant available
    int             half_turn_rule;             // Number of half-turn since the last capture or pawn move (Fifty-Move rule)
    int             game_turn_max = 125;

    public:
        int         game_turn;                  // Game turn, incremented after each black move

        Board(string _fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1", bool chess960_rule = true);
        Board(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _full_move, bool chess960_rule = true);

        inline bool     is_white_turn() { return white_turn; }
        char            get_cell(int x, int y);
        float           get_game_state();
        bool            get_check_state();
        int             get_castling_rights();

        string          create_fen(bool with_turns = true);
        Board           *clone();

        vector<Move>    get_available_moves();
        void            apply_move(Move move);

        void            log();
        void            log_history(int turns = -1);

        bool            operator ==(Board *test_board);

    private:
        vector<Move>    available_moves;
        bool            moves_computed;
        bool            check;
        bool            check_computed;
        float           game_state;
        bool            game_state_computed;

        // Engine variables 
        // uint64_t   uncheck_mask; // Full set of bits to 1 means there is no check
        // uint64_t   attacked_cells_mask; // Squares attacked by the opponent
        uint64_t   white_pieces_mask; // All white pieces on the board
        uint64_t   black_pieces_mask; // All black pieces on the board
        uint64_t   not_white_pieces_mask; // All cells that are not a white piece
        uint64_t   not_black_pieces_mask; // All cells that are not a black piece
        uint64_t   all_pieces_mask; // All pieces on the board
        uint64_t   empty_cells_mask; // All empty cells on the board
        uint64_t   capturable_by_white_pawns_mask;
        uint64_t   capturable_by_black_pawns_mask;

        // FEN history is used to check the Threefold Repetition rule
        // Each FEN is saved in the history after each move
        string         fen_history[FEN_HISTORY_SIZE];
        int            fen_history_index;

        // // Function pointer to apply castle depending on the chess960 rule
        // bool    (Board::*_handle_castle)(int, int, int, int);

        void    _main_parsing(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _game_turn, bool chess960_rule);
        void    _initialize_bitboards();
        void    _parse_board(string fen_board);
        void    _parse_castling(string castling_fen);
        
        char    _get_cell(uint64_t mask);

        void    _create_fen_for_standard_castling(char *fen, int *fen_i);
        void    _create_fen_for_chess960_castling(char *fen, int *fen_i);

        void    _apply_regular_white_move(uint64_t src, uint64_t dst, uint64_t *piece_mask);
        void    _apply_regular_black_move(uint64_t src, uint64_t dst, uint64_t *piece_mask);
        void    _move_white_pawn(uint64_t src, uint64_t dst, char promotion);
        void    _move_black_pawn(uint64_t src, uint64_t dst, char promotion);
        void    _move_white_king(uint64_t src, uint64_t dst, castle_info_e castle_info);
        void    _move_black_king(uint64_t src, uint64_t dst, castle_info_e castle_info);
        void    _capture_white_pieces(uint64_t dst);
        void    _capture_black_pieces(uint64_t dst);
        
        void        _find_moves();
        void        _find_white_pawns_moves(uint64_t src);
        void        _find_white_knights_moves(uint64_t src);
        void        _find_white_bishops_moves(uint64_t src);
        void        _find_white_rooks_moves(uint64_t src);
        void        _find_white_queens_moves(uint64_t src);
        void        _find_white_king_moves();
        void        _find_white_castle_moves(uint64_t dst);
        void        _find_black_pawns_moves(uint64_t src);
        void        _find_black_knights_moves(uint64_t src);
        void        _find_black_bishops_moves(uint64_t src);
        void        _find_black_rooks_moves(uint64_t src);
        void        _find_black_queens_moves(uint64_t src);
        void        _find_black_king_moves();
        void        _find_black_castle_moves(uint64_t dst);
        
        void        _add_regular_move_or_promotion(char piece, uint64_t src, uint64_t dst);

        // void    _generate_attacked_cells();
        // void    _generate_pin_masks();
        
        // void     _find_moves_castle(uint64_t src, int castle_index);
        // bool     _is_castle_legal(int src_x, int src_y, int dst_x, int trajectory_dx);
        // void    _filter_non_legal_moves();
        // bool    _is_check();
        // bool    _is_check(uint64_t src);

        void    _update_engine_data();
        void    _update_fen_history();

        // float   _find_game_state();
        // bool    _threefold_repetition_rule();
        // bool    _insufficient_material_rule();
        
        // BIT OPERATIONS

        uint64_t    _get_diagonal_rays(uint64_t src);
        uint64_t    _get_line_rays(uint64_t src);
        uint64_t    _compute_sliding_piece_positive_ray(uint64_t src, ray_dir_e dir);
        uint64_t    _compute_sliding_piece_negative_ray(uint64_t src, ray_dir_e dir);

        void        _create_piece_moves(char piece, uint64_t src, uint64_t legal_moves);
        void        _apply_function_on_all_pieces(uint64_t bitboard, std::function<void(uint64_t)> func);
        uint64_t    _get_most_significant_bit(uint64_t bitboard);
        uint64_t    _get_least_significant_bit(uint64_t bitboard);
        uint64_t    _count_trailing_zeros(uint64_t bitboard);

        // LOOKUP TABLES

        uint64_t pawn_captures_lookup[64][2]; // 0: white, 1: black
        uint64_t knight_lookup[64];
        uint64_t sliding_lookup[64][8]; // 0: N, 1: NE, 2: E, 3: SE, 4: S, 5: SW, 6: W, 7: NW
        uint64_t king_lookup[64];

        void    _create_lookup_tables();
        void    _create_pawn_captures_lookup_table(int y, int x, uint64_t position, int lkt_i);
        void    _create_knight_lookup_table(int y, int x, uint64_t position, int lkt_i);
        void    _create_sliding_lookup_table(int y, int x, uint64_t position, int lkt_i);
        void    _create_king_lookup_table(int y, int x, uint64_t position, int lkt_i);
};

# else

class Board {
    /*
        Board represent all FEN data :
            rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1

        White = Upper case = First indexes
        Black = Lower case = Last indexes
    */

    // FEN data:
    char        board[8][8];                // 0 0 coordinate is the up left corner
    bool        white_turn;
    int         castles[4];                 // Rook column index where the castle is available - 2 first for White and 2 last for Black
    int         kings_initial_columns[2];   // First for white king and second for black king
    int         en_passant_x;               // En passant coordinates & availability are created after a pawn move of 2 squares
    int         en_passant_y;
    bool        en_passant_available;
    int         half_turn_rule;             // Number of half-turn since the last capture or pawn move (Fifty-Move rule)

    int         game_turn_max = 125;

    // FEN history is used to check the Threefold Repetition rule
    // Each FEN is saved in the history after each move
    string         fen_history[FEN_HISTORY_SIZE];
    int            fen_history_index;

    public:
        int         game_turn;                  // Game turn, incremented after each black move

        Board(string _fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1", bool chess960_rule = true);
        Board(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _full_move, bool chess960_rule = true);

        bool            is_white_turn();
        char            get_cell(int x, int y);
        float           get_game_state();
        bool            get_check_state();
        int             get_castling_rights();

        string          create_fen(bool with_turns = true);
        Board           *clone();
        
        vector<Move>    get_available_moves();
        void            apply_move(Move move);
        
        void            log();
        void            log_history(int turns = -1);

        bool            operator ==(Board *test_board);

    private:

        vector<Move>    available_moves;
        bool            moves_computed;
        bool            check;
        bool            check_computed;
        float           game_state;
        bool            game_state_computed;

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

        float   _find_game_state();
        bool    _threefold_repetition_rule();
        bool    _insufficient_material_rule();

        vector<Move>    _find_moves();
        void            _find_moves_pawns(int x, int y);
        void            _add_regular_move_or_promotion(int x, int y, int dx, int dy);
        void            _find_moves_knights(int x, int y);
        void            _find_moves_bishops(int x, int y);
        void            _find_moves_rooks(int x, int y);
        void            _find_moves_queens(int x, int y);
        void            _find_moves_king(int x, int y);
        void            _find_moves_castle(int x, int y, int castle_index);
        bool            _is_castle_legal(int src_x, int src_y, int dst_x, int trajectory_dx);

        void    _filter_non_legal_moves();
        bool    _is_check();
        bool    _is_check(int x, int y);
};

# endif

#endif
