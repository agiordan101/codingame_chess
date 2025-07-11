
#pragma GCC optimize("O3")
#pragma GCC optimize("inline")
#pragma GCC optimize("omit-frame-pointer")

/*
        Content of 'srcs/chessengine/ChessEngine.hpp'
*/

#ifndef CHESSENGINE_HPP
#define CHESSENGINE_HPP

using namespace std;

#include <bits/stdc++.h>
#include <iostream>
#include <locale>
#include <stdio.h>
#include <string.h>
#include <vector>

#define CHESS960_RULES 1

#define USE_VISUAL_BOARD 0
#define PRINT_DEBUG_DATA 1 & USE_VISUAL_BOARD
#define PRINT_TURNS      0 & USE_VISUAL_BOARD

#define EMPTY_CELL '-'

#define BITMASK_ALL_CELLS   0xFFFFFFFFFFFFFFFFUL
#define BITMASK_WHITE_CELLS 0xAA55AA55AA55AA55UL
#define BITMASK_BLACK_CELLS 0x55AA55AA55AA55AAUL

#define BITMASK_LINE_8  0x00000000000000FFUL
#define BITMASK_LINE_7  0x000000000000FF00UL
#define BITMASK_LINE_65 0x00000000FFFF0000UL
#define BITMASK_LINE_43 0x0000FFFF00000000UL
#define BITMASK_LINE_2  0x00FF000000000000UL
#define BITMASK_LINE_1  0xFF00000000000000UL

#define BITMASK_CASTLE_BLACK_LEFT_KING  0x0000000000000004UL
#define BITMASK_CASTLE_BLACK_LEFT_ROOK  0x0000000000000008UL
#define BITMASK_CASTLE_BLACK_RIGHT_KING 0x0000000000000040UL
#define BITMASK_CASTLE_BLACK_RIGHT_ROOK 0x0000000000000020UL
#define BITMASK_CASTLE_WHITE_LEFT_KING  0x0400000000000000UL
#define BITMASK_CASTLE_WHITE_LEFT_ROOK  0x0800000000000000UL
#define BITMASK_CASTLE_WHITE_RIGHT_KING 0x4000000000000000UL
#define BITMASK_CASTLE_WHITE_RIGHT_ROOK 0x2000000000000000UL

/* ENUMS */

#define GAME_CONTINUE -2
#define BLACK_WIN     -1
#define DRAW          0
#define WHITE_WIN     1

enum castle_info_e
{
    NOINFO,
    NOTCASTLE,
    WHITELEFT,
    WHITERIGHT,
    BLACKLEFT,
    BLACKRIGHT
};

enum ray_dir_e
{
    NORTH,
    NORTHEAST,
    EAST,
    SOUTHEAST,
    SOUTH,
    SOUTHWEST,
    WEST,
    NORTHWEST
};

/* PIECES FUNCTIONS */

inline wchar_t convert_piece_to_unicode(char piece)
{
    switch (piece)
    {
    case 'P':
        return L'♟';
    case 'N':
        return L'♞';
    case 'B':
        return L'♝';
    case 'R':
        return L'♜';
    case 'Q':
        return L'♛';
    case 'K':
        return L'♚';
    case 'p':
        return L'♙';
    case 'n':
        return L'♘';
    case 'b':
        return L'♗';
    case 'r':
        return L'♖';
    case 'q':
        return L'♕';
    case 'k':
        return L'♔';
    default:
        return piece;
    }
}

/* BITWISE OPERATIONS */

inline uint64_t _count_bits(uint64_t bitboard)
{
    return __builtin_popcountll(bitboard);
}

inline uint64_t _count_trailing_zeros(uint64_t bitboard)
{
    return __builtin_ctzll(bitboard);
}

inline uint64_t _count_leading_zeros(uint64_t bitboard)
{
    return 63 - __builtin_clzll(bitboard);
}

inline uint64_t _get_least_significant_bit(uint64_t bitboard)
{
    return 1UL << _count_trailing_zeros(bitboard);
}

inline uint64_t _get_most_significant_bit(uint64_t bitboard)
{
    return 1UL << _count_leading_zeros(bitboard);
}

/* NOTATION FUNCTIONS */

inline int column_name_to_index(char column_name)
{
    return tolower(column_name) - 'a';
}

inline char column_index_to_name(int column_id)
{
    return column_id + 'a';
}

inline int line_number_to_index(char line_number)
{
    return 8 - (line_number - '0');
}

inline char line_index_to_number(int line_index)
{
    return (8 - line_index) + '0';
}

inline void algebraic_to_coord(string algebraic, int *x, int *y)
{
    const char *algebraic_char = algebraic.c_str();
    *x = column_name_to_index(algebraic_char[0]);
    *y = line_number_to_index(algebraic_char[1]);
}

inline string coord_to_algebraic(int x, int y)
{
    char algebraic[3];
    algebraic[0] = (char)column_index_to_name(x);
    algebraic[1] = (char)line_index_to_number(y);
    algebraic[2] = '\0';

    return string(algebraic);
}

inline uint64_t algebraic_to_bitboard(string algebraic)
{
    int x, y;
    algebraic_to_coord(algebraic, &x, &y);
    return 1UL << (y * 8 + x);
}

inline string bitboard_to_algebraic(uint64_t bitboard)
{
    uint64_t pos = _count_trailing_zeros(bitboard);
    return coord_to_algebraic(pos % 8, pos / 8);
}

#endif

/*
        Content of 'srcs/chessengine/Move.hpp'
*/

#ifndef MOVE_HPP
#define MOVE_HPP

using namespace std;

class Move
{

    public:
        string        uci;
        uint64_t      src;
        uint64_t      dst;
        char          promotion;
        char          piece;
        castle_info_e castle_info;
        Move(string _uci);
        Move(
            char          _piece,
            uint64_t      src,
            uint64_t      dst,
            char          _promotion = 0,
            castle_info_e _castle_info = NOTCASTLE
        );

        void   log();
        string to_uci();
        string to_uci(bool chess960_rules, bool castling);

        bool operator==(Move *other);

        static bool compare_move_vector(vector<Move> movelst1, vector<Move> movelst2);

    private:
        static bool _is_move_in_movelst(Move *move, vector<Move> movelst);
};

#endif

/*
        Content of 'srcs/chessengine/Board.hpp'
*/

#ifndef BOARD_HPP
#define BOARD_HPP

#pragma region Board

typedef struct s_serialized_fen
{
        __int128 serialized_pawns;
        __int128 serialized_knights;
        __int128 serialized_bishops;
        __int128 serialized_rooks;
        __int128 serialized_queens;
        __int128 serialized_kings;
        uint32_t serialized_last_info;
} t_serialized_fen;

#define FEN_HISTORY_SIZE        50
#define SIZEOF_T_SERIALIZED_FEN sizeof(t_serialized_fen)

class Board
{
        /*
        Board represent all FEN data :
        rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1

        White = Upper case = First indexes
        Black = Lower case = Last indexes
        */

        bool chess960_rule;
        bool codingame_rule;

        bool white_turn;

        uint64_t white_castles;
        uint64_t black_castles;

        uint8_t serialized_white_castles;
        uint8_t serialized_black_castles;

        uint64_t en_passant;
        uint64_t next_turn_en_passant;
        int      half_turn_rule;

    public:
#if USE_VISUAL_BOARD == 1
        VisualBoard visual_board;
#endif

        int      game_turn;
        uint64_t white_pawns;
        uint64_t white_knights;
        uint64_t white_bishops;
        uint64_t white_rooks;
        uint64_t white_queens;
        uint64_t white_king;
        uint64_t black_pawns;
        uint64_t black_knights;
        uint64_t black_bishops;
        uint64_t black_rooks;
        uint64_t black_queens;
        uint64_t black_king;

        uint64_t all_pieces_mask;
        uint64_t empty_cells_mask;
        uint64_t white_pieces_mask;
        uint64_t black_pieces_mask;
        uint64_t not_white_pieces_mask;
        uint64_t not_black_pieces_mask;
        uint64_t uncheck_mask;
        uint64_t pawn_uncheck_mask;
        uint64_t attacked_by_white_mask;
        uint64_t attacked_by_black_mask;
        uint64_t pin_masks[64];
        Board();
        Board(string _fen, bool chess960_rule = true, bool codingame_rule = true);
        Board(
            string _board,
            string _color,
            string _castling,
            string _en_passant,
            int    _half_turn_rule,
            int    _full_move,
            bool   chess960_rule = true,
            bool   codingame_rule = true
        );

        inline bool is_white_turn()
        {
            return white_turn;
        }
        char          get_cell(int x, int y);
        float         get_game_state();
        bool          get_check_state();
        uint64_t      get_castling_rights();
        static string get_name();

        string create_fen(bool with_turns = true);
        Board *clone();

        vector<Move> get_available_moves();
        void         apply_move(Move move);

        void log(bool raw = false);

        bool operator==(Board *test_board);

    private:
        bool         check_state;
        bool         double_check;
        bool         engine_data_updated;
        vector<Move> available_moves;
        bool         moves_computed;
        float        game_state;
        bool         game_state_computed;

        uint64_t ally_king;
        uint64_t ally_pieces;

        uint64_t enemy_pawns;
        uint64_t enemy_knights;
        uint64_t enemy_bishops;
        uint64_t enemy_rooks;
        uint64_t enemy_queens;
        uint64_t enemy_king;
        uint64_t enemy_pieces;
        uint64_t enemy_pieces_sliding_diag;
        uint64_t enemy_pieces_sliding_line;
        uint64_t capturable_by_white_pawns_mask;
        uint64_t capturable_by_black_pawns_mask;

        int               current_sfen_history_index;
        t_serialized_fen  serialized_fen_history[FEN_HISTORY_SIZE];
        t_serialized_fen *current_sfen;

        void _main_parsing(
            string _board,
            string _color,
            string _castling,
            string _en_passant,
            int    _half_turn_rule,
            int    _game_turn,
            bool   _chess960_rule,
            bool   _codingame_rule
        );
        void _initialize_bitboards();
        void _parse_board(string fen_board);
        void _parse_castling(string castling_fen);

        char _get_cell(uint64_t mask);
        void _create_fen_for_standard_castling(char *fen, int *fen_i);
        void _create_fen_for_chess960_castling(char *fen, int *fen_i);

        void _apply_regular_white_move(uint64_t src, uint64_t dst, uint64_t *piece_mask);
        void _apply_regular_black_move(uint64_t src, uint64_t dst, uint64_t *piece_mask);
        void _move_white_pawn(uint64_t src, uint64_t dst, char promotion);
        void _move_black_pawn(uint64_t src, uint64_t dst, char promotion);
        void _move_white_king(uint64_t src, uint64_t dst, castle_info_e castle_info);
        void _move_black_king(uint64_t src, uint64_t dst, castle_info_e castle_info);
        void _capture_white_pieces(uint64_t dst);
        void _capture_black_pieces(uint64_t dst);

        uint8_t _serialize_en_passant();

        void _update_engine_at_turn_end();
        void _update_engine_at_turn_start();
        void _update_check_and_pins();
        void _update_pawn_check(int king_lkt_i);
        void _update_attacked_cells_masks();
        void _update_serialized_fen_history();

        void _find_white_pawns_attacks(uint64_t src);
        void _find_white_knights_attacks(uint64_t src);
        void _find_white_bishops_attacks(uint64_t src);
        void _find_white_rooks_attacks(uint64_t src);
        void _find_white_queens_attacks(uint64_t src);
        void _find_white_king_attacks();
        void _find_black_pawns_attacks(uint64_t src);
        void _find_black_knights_attacks(uint64_t src);
        void _find_black_bishops_attacks(uint64_t src);
        void _find_black_rooks_attacks(uint64_t src);
        void _find_black_queens_attacks(uint64_t src);
        void _find_black_king_attacks();

        void _find_moves();
        void _find_white_pawns_moves(uint64_t src);
        void _find_white_knights_moves(uint64_t src);
        void _find_white_bishops_moves(uint64_t src);
        void _find_white_rooks_moves(uint64_t src);
        void _find_white_queens_moves(uint64_t src);
        void _find_white_king_moves();
        void _find_white_castle_moves(uint64_t dst);
        void _find_black_pawns_moves(uint64_t src);
        void _find_black_knights_moves(uint64_t src);
        void _find_black_bishops_moves(uint64_t src);
        void _find_black_rooks_moves(uint64_t src);
        void _find_black_queens_moves(uint64_t src);
        void _find_black_king_moves();
        void _find_black_castle_moves(uint64_t dst);

        void _create_white_pawn_promotions(char piece, uint64_t src, uint64_t dst);
        void _create_black_pawn_promotions(char piece, uint64_t src, uint64_t dst);
        void _create_promotion_move(char piece, uint64_t src, uint64_t dst, char promotion);
        void _create_move(char piece, uint64_t src, uint64_t dst);

        using methodAddrWith1Params = void (Board::*)(uint64_t);
        using methodAddrWith3Params = void (Board::*)(char, uint64_t, uint64_t);

        void _apply_function_on_all_pieces(methodAddrWith1Params func, uint64_t bitboard);
        void _apply_function_on_all_pieces(
            methodAddrWith3Params func, uint64_t bitboard, char param1, uint64_t param2
        );
        uint64_t _get_diagonal_rays(uint64_t src, uint64_t piece_to_ignore = 0UL);
        uint64_t _get_line_rays(uint64_t src, uint64_t piece_to_ignore = 0UL);
        uint64_t
        _compute_sliding_piece_positive_ray(uint64_t src, ray_dir_e dir, uint64_t piece_to_ignore);
        uint64_t
        _compute_sliding_piece_negative_ray(uint64_t src, ray_dir_e dir, uint64_t piece_to_ignore);
        void _compute_sliding_piece_positive_ray_checks_and_pins(
            uint64_t king_pos, ray_dir_e dir, uint64_t potential_attacker
        );
        void _compute_sliding_piece_negative_ray_checks_and_pins(
            uint64_t king_pos, ray_dir_e dir, uint64_t potential_attacker
        );
        bool     _is_sliding_piece_positive_diagonal_ray_behind(uint64_t pawn_pos, ray_dir_e dir);
        bool     _is_sliding_piece_negative_diagonal_ray_behind(uint64_t pawn_pos, ray_dir_e dir);
        uint64_t _compute_castling_positive_path(uint64_t src, uint64_t dst);
        uint64_t _compute_castling_negative_path(uint64_t src, uint64_t dst);

        float _compute_game_state();
        bool  _threefold_repetition_rule();
        bool  _insufficient_material_rule();

        static bool     lookup_tables_initialized;
        static uint64_t pawn_captures_lookup[64][2];
        static uint64_t knight_lookup[64];
        static uint64_t sliding_lookup[64][8];
        static uint64_t king_lookup[64];

        static void _initialize_lookup_tables();
        static void _create_pawn_captures_lookup_table(int y, int x, uint64_t position, int lkt_i);
        static void _create_knight_lookup_table(int y, int x, uint64_t position, int lkt_i);
        static void _create_sliding_lookup_table(int y, int x, uint64_t position, int lkt_i);
        static void _create_king_lookup_table(int y, int x, uint64_t position, int lkt_i);
};

#endif

/*
        Content of 'srcs/heuristics/AbstractHeuristic.hpp'
*/

#ifndef ABSTRACTHEURISTIC_HPP
#define ABSTRACTHEURISTIC_HPP

class AbstractHeuristic
{

    public:
        virtual float  evaluate(Board *board) = 0;
        virtual string get_name() = 0;
};

#endif

/*
        Content of 'srcs/heuristics/PiecesHeuristic.hpp'
*/

#ifndef PIECESHEURISTIC_HPP
#define PIECESHEURISTIC_HPP

#define EVALUATION_WINRATE_MAP_SIZE 40000

class PiecesHeuristic : public AbstractHeuristic
{

    public:
        PiecesHeuristic();

        float  evaluate(Board *board) override;
        string get_name() override;

    private:
        float _evaluation_winrate_map[EVALUATION_WINRATE_MAP_SIZE];

        int _material_evaluation(Board *board, int *white_material, int *black_material);
        int _piece_positions_evaluation(
            Board *board, float white_eg_coefficient, float black_eg_coefficient
        );

        int _lookup_bonuses_for_all_pieces(int *bonus_table, uint64_t bitboard);
        int _lookup_bonuses_for_all_pieces(
            int *sg_bonus_table, int *eg_bonus_table, float eg_coef, uint64_t bitboard
        );

        typedef enum t_piece_values
        {
            PAWN_VALUE = 100,
            KNIGHT_VALUE = 305,
            BISHOP_VALUE = 333,
            ROOK_VALUE = 563,
            QUEEN_VALUE = 950
        } e_piece_values;

        const int control_value_for_empty_cell = 5;
        const int control_value_for_enemy_cell = 10;
        const int control_value_for_ally_cell = 15;

        const int material_start_game =
            10 * PAWN_VALUE + 2 * KNIGHT_VALUE + 2 * BISHOP_VALUE + 2 * ROOK_VALUE + QUEEN_VALUE;
        const int material_end_game = QUEEN_VALUE + ROOK_VALUE + 3 * PAWN_VALUE;
        const int material_start_end_game_diff = material_start_game - material_end_game;

        // clang-format off
                                int white_pawn_sg_bonus_table[64] = {
             0,  0,  0,  0,  0,  0,  0,  0,
            20, 20, 20, 20, 20, 20, 20, 20,
             0,  0,  0,  0,  0,  0,  0,  0,
             0,  0,  0,  0,  0,  0,  0,  0,
             0,  0,  0, 20, 20,  0,  0,  0,
             0,  0, 10,  0,  0, 10,  0,  0,
             5,  5, 5, -20,-20,  5,  5,  5,
             0,  0,  0,  0,  0,  0,  0,  0
        };

        int white_pawn_eg_bonus_table[64] = {
             0,  0,  0,  0,  0,  0,  0,  0,
            80, 80, 80, 80, 80, 80, 80, 80,
            50, 50, 50, 50, 50, 50, 50, 50,
            30, 30, 30, 30, 30, 30, 30, 30,
            20, 20, 20, 20, 20, 20, 20, 20,
            10, 10, 10, 10, 10, 10, 10, 10,
            10, 10, 10, 10, 10, 10, 10, 10,
             0,  0,  0,  0,  0,  0,  0,  0,
        };
        
        int black_pawn_sg_bonus_table[64] = {
             0,  0,  0,  0,  0,  0,  0,  0,
             5,  5, 5, -20,-20,  5,  5,  5,
             0,  0, 10,  0,  0, 10,  0,  0,
             0,  0,  0, 20, 20,  0,  0,  0,
             0,  0,  0,  0,  0,  0,  0,  0,
             0,  0,  0,  0,  0,  0,  0,  0,
            20, 20, 20, 20, 20, 20, 20, 20,
             0,  0,  0,  0,  0,  0,  0,  0
        };

        int black_pawn_eg_bonus_table[64] = {
             0,  0,  0,  0,  0,  0,  0,  0,
            10, 10, 10, 10, 10, 10, 10, 10,
            10, 10, 10, 10, 10, 10, 10, 10,
            20, 20, 20, 20, 20, 20, 20, 20,
            30, 30, 30, 30, 30, 30, 30, 30,
            50, 50, 50, 50, 50, 50, 50, 50,
            80, 80, 80, 80, 80, 80, 80, 80,
             0,  0,  0,  0,  0,  0,  0,  0
        };

        int white_knight_bonus_table[64] = {
            -20,-20,-10,-10,-10,-10,-20,-20,
            -20,-20,  0,  5,  5,  0,-20,-20,
            -10,  5, 10, 15, 15, 10,  5,-10,
            -10,  5, 15, 20, 20, 15,  5,-10,
            -10,  5, 15, 20, 20, 15,  5,-10,
            -10,  5, 10, 15, 15, 10,  5,-10,
            -20,-20,  0,  0,  0,  0,-20,-20,
            -20,-20,-10,-10,-10,-10,-20,-20
        };

        int black_knight_bonus_table[64] = {
            -50,-40,-30,-30,-30,-30,-40,-50,
            -40,-20,  0,  0,  0,  0,-20,-40,
            -30,  5, 10, 15, 15, 10,  5,-30,
            -30,  5, 15, 20, 20, 15,  5,-30,
            -30,  5, 15, 20, 20, 15,  5,-30,
            -30,  5, 10, 15, 15, 10,  5,-30,
            -40,-20,  0,  5,  5,  0,-20,-40,
            -50,-40,-30,-30,-30,-30,-40,-50,
        };

        int white_bishop_bonus_table[64] = {
            -20,-10,-10,-10,-10,-10,-10,-20,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -10,  0,  5,  5,  5,  5,  0,-10,
            -10,  0,  5, 10, 10,  5,  0,-10,
            -10,  0,  5, 10, 10,  5,  0,-10,
            -10,  0,  5,  5,  5,  5,  0,-10,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -20,-10,-10,-10,-10,-10,-10,-20
        };

        int black_bishop_bonus_table[64] = {
            -20,-10,-10,-10,-10,-10,-10,-20,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -10,  0,  5,  5,  5,  5,  0,-10,
            -10,  0,  5, 10, 10,  5,  0,-10,
            -10,  0,  5, 10, 10,  5,  0,-10,
            -10,  0,  5,  5,  5,  5,  0,-10,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -20,-10,-10,-10,-10,-10,-10,-20
        };

        int white_rook_bonus_table[64] = {
             0,  0,  0,  0,  0,  0,  0,  0,
             5, 10, 10, 10, 10, 10, 10,  5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
             0,  0, 10, 10, 10, 10,  0,  0
        };

        int black_rook_bonus_table[64] = {
             0,  0, 10, 10, 10, 10,  0,  0,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
             5, 10, 10, 10, 10, 10, 10,  5,
             0,  0,  0,  0,  0,  0,  0,  0,
        };

        int white_queen_bonus_table[64] = {
            -20,-10,-10, -5, -5,-10,-10,-20,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -10,  0, 10, 10, 10, 10,  0,-10,
            -5,   0, 10, 15, 15, 10,  0, -5,
            -5,   0, 10, 15, 15, 10,  0, -5,
            -10,  0, 10, 10, 10, 10,  0,-10,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -20,-10,-10, -5, -5,-10,-10,-20
        };

        int black_queen_bonus_table[64] = {
            -20,-10,-10, -5, -5,-10,-10,-20,
            -10,  0,  5,  0,  0,  0,  0,-10,
            -10,  0, 10, 10, 10, 10,  0,-10,
            -5,   0, 10, 15, 15, 10,  0, -5,
            -5,   0, 10, 15, 15, 10,  0, -5,
            -10,  0, 10, 10, 10, 10,  0,-10,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -20,-10,-10, -5, -5,-10,-10,-20
        };
 
        int white_king_sg_bonus_table[64] = {
            -60,-60,-60,-60,-60,-60,-60,-60,
            -50,-50,-50,-50,-50,-50,-50,-50,
            -40,-40,-40,-40,-40,-40,-40,-40,
            -30,-30,-30,-30,-30,-30,-30,-30,
            -20,-20,-20,-20,-20,-20,-20,-20,
            -10,-10,-10,-10,-10,-10,-10,-10,
              0,  0,  0,  0,  0,  0,  0,  0,
              5,  5, 30,  0,  0,  0, 30,  5
        };

        int white_king_eg_bonus_table[64] = {
            -50,-40,-20,-20,-20,-20,-40,-50,
            -40,-20,  0,  0,  0,  0,-20,-40,
            -20,  0,  5,  5,  5,  5,  0,-20,
            -20,  0,  5, 10, 10,  5,  0,-20,
            -20,  0,  5, 10, 10,  5,  0,-20,
            -20,  0,  5,  5,  5,  5,  0,-20,
            -40,-20,  0,  0,  0,  0,-20,-40,
            -50,-40,-20,-20,-20,-20,-40,-50
        };

        int black_king_sg_bonus_table[64] = {
              5,  5, 30,  0,  0,  0, 30,  5,
              0,  0,  0,  0,  0,  0,  0,  0,
            -10,-10,-10,-10,-10,-10,-10,-10,
            -20,-20,-20,-20,-20,-20,-20,-20,
            -30,-30,-30,-30,-30,-30,-30,-30,
            -40,-40,-40,-40,-40,-40,-40,-40,
            -50,-50,-50,-50,-50,-50,-50,-50,
            -60,-60,-60,-60,-60,-60,-60,-60,
        };

        int black_king_eg_bonus_table[64] = {
            -50,-40,-20,-20,-20,-20,-40,-50,
            -40,-20,  0,  0,  0,  0,-20,-40,
            -20,  0,  5,  5,  5,  5,  0,-20,
            -20,  0,  5, 10, 10,  5,  0,-20,
            -20,  0,  5, 10, 10,  5,  0,-20,
            -20,  0,  5,  5,  5,  5,  0,-20,
            -40,-20,  0,  0,  0,  0,-20,-40,
            -50,-40,-20,-20,-20,-20,-40,-50
        };
        // clang-format on
};

#endif

/*
        Content of 'srcs/agents/AbstractAgent.hpp'
*/

#ifndef ABSTRACTAGENT_HPP
#define ABSTRACTAGENT_HPP

#include <tuple>
#include <vector>

class AbstractAgent
{

    public:
        virtual Move   choose_from(Board *board, clock_t turn_start_clock) = 0;
        virtual string get_name() = 0;

        virtual vector<string> get_stats()
        {
            return vector<string>{};
        };
};

#endif

/*
        Content of 'srcs/agents/MinMaxAlphaBetaAgent.hpp'
*/

#ifndef MINMAXITERDEEPAGENT_HPP
#define MINMAXITERDEEPAGENT_HPP

class MinMaxAlphaBetaAgent : public AbstractAgent
{

    public:
        MinMaxAlphaBetaAgent(AbstractHeuristic *heuristic, int ms_constraint);
        virtual string get_name() override;
        vector<string> get_stats() override;
        virtual Move   choose_from(Board *board, clock_t turn_start_clock) override;

    private:
        AbstractHeuristic *_heuristic;

        int     _ms_constraint;
        float   _ms_turn_stop;
        clock_t _turn_start_clock;

        int _depth_reached;
        int _nodes_explored;

        void  get_qualities(Board *board, vector<Move> moves, vector<float> *qualities);
        float minmax(Board *board, int max_depth, int depth, float alpha, float beta);
        float max_node(
            Board *board, vector<Move> *moves, int max_depth, int depth, float alpha, float beta
        );
        float min_node(
            Board *board, vector<Move> *moves, int max_depth, int depth, float alpha, float beta
        );

        bool  is_time_up();
        float elapsed_time(clock_t clock_start);
};

#endif

/*
        Content of 'srcs/gameengine/GameEngine.hpp'
*/

#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

class GameEngine
{

    public:
        GameEngine(AbstractAgent *agent);
        void infinite_game_loop();

    private:
        bool crazy_house;
        int  max_moves;

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

/*
        Content of 'srcs/gameengine/GameEngine.cpp'
*/

GameEngine::GameEngine(AbstractAgent *agent)
{
    this->_agent = agent;
    this->_board = NULL;
}

void GameEngine::infinite_game_loop()
{
    this->_parse_first_turn();
    cout << "lastmove fen" << endl;

    while (1)
    {
        _parse_turn();

        Move move = this->_agent->choose_from(this->_board, this->_turn_start_clock);
        cout << move.to_uci();

        vector<string> stats = this->_agent->get_stats();
        for (string stat : stats)
            cout << " " << stat;
        cout << endl;

        this->_board->apply_move(move);

        float game_state = this->_board->get_game_state();
        if (game_state != GAME_CONTINUE)
        {
            cerr << "\nGameEngine: Game is over : " << game_state << endl;
            this->_board->log(true);
        }
    }
}

void GameEngine::_parse_first_turn()
{
    int    constants_count;
    string name;
    string value;
    bool   crazy_house = false;
    int    max_moves = 0;

    cin >> constants_count;
    cin.ignore();

    for (int i = 0; i < constants_count; i++)
    {
        cin >> name >> value;
        cin.ignore();

        if (name == "crazyHouse")
            crazy_house = value == "1";
        else if (name == "maxMoves")
            max_moves = stoi(value);
    }

    this->crazy_house = crazy_house;
    this->max_moves = max_moves;
}

void GameEngine::_parse_turn()
{
    string move;
    cin >> move;

    string board;
    string color;
    string castling;
    string en_passant;
    string half_move_clock_str;
    string full_move_str;

    cin >> board >> color >> castling >> en_passant >> half_move_clock_str >> full_move_str;

    this->_turn_start_clock = clock();

    int half_move_clock = stoi(half_move_clock_str);
    int full_move = stoi(full_move_str);

    if (full_move == 1)
        this->_board = new Board(board, color, castling, en_passant, half_move_clock, full_move);
    else
        this->_board->apply_move(Move(move));

    cin.ignore();
}

/*
        Content of 'srcs/chessengine/Board.cpp'
*/

bool     Board::lookup_tables_initialized = false;
uint64_t Board::pawn_captures_lookup[64][2];
uint64_t Board::knight_lookup[64];
uint64_t Board::sliding_lookup[64][8];
uint64_t Board::king_lookup[64];

Board::Board()
{
}

Board::Board(string _fen, bool _chess960_rule, bool _codingame_rule)
{
    stringstream ss(_fen);
    string       board;
    string       color;
    string       castling;
    string       en_passant;
    string       half_turn_rule;
    string       game_turn;

    getline(ss, board, ' ');
    getline(ss, color, ' ');
    getline(ss, castling, ' ');
    getline(ss, en_passant, ' ');
    getline(ss, half_turn_rule, ' ');
    getline(ss, game_turn, ' ');

    _main_parsing(
        board, color, castling, en_passant, stoi(half_turn_rule), stoi(game_turn), _chess960_rule,
        _codingame_rule
    );
}

Board::Board(
    string _board,
    string _color,
    string _castling,
    string _en_passant,
    int    _half_turn_rule,
    int    _game_turn,
    bool   _chess960_rule,
    bool   _codingame_rule
)
{
    _main_parsing(
        _board, _color, _castling, _en_passant, _half_turn_rule, _game_turn, _chess960_rule,
        _codingame_rule
    );
}

void Board::log(bool raw)
{
    uint64_t rook;

    string   white_castles_pos[2] = {"N/A", "N/A"};
    int      white_castles_pos_i = 0;
    uint64_t castle_tmp = white_castles;
    while (castle_tmp)
    {
        rook = _get_least_significant_bit(castle_tmp);
        white_castles_pos[white_castles_pos_i++] = bitboard_to_algebraic(rook);

        castle_tmp ^= rook;
    }

    string black_castles_pos[2] = {"N/A", "N/A"};
    int    black_castles_pos_i = 0;
    castle_tmp = black_castles;
    while (castle_tmp)
    {
        rook = _get_least_significant_bit(castle_tmp);
        black_castles_pos[black_castles_pos_i++] = bitboard_to_algebraic(rook);

        castle_tmp ^= rook;
    }

    cerr << "Board: FEN: " << create_fen() << endl;
    cerr << "Board: Turn: " << (white_turn ? "White" : "Black") << endl;
    cerr << "Board: White castling: " << white_castles_pos[0] << " " << white_castles_pos[1]
         << endl;
    cerr << "Board: Black castling: " << black_castles_pos[0] << " " << black_castles_pos[1]
         << endl;
    cerr << "Board: En passant: " << (en_passant ? bitboard_to_algebraic(en_passant) : "N/A")
         << endl;
    cerr << "Board: half_turn_rule: " << to_string(half_turn_rule) << endl;
    cerr << "Board: game_turn: " << to_string(game_turn) << endl;

#if USE_VISUAL_BOARD == 1
    if (raw)
        this->visual_board.printRawBoard();
    else
        this->visual_board.printBoard();
#else
    raw = raw;
#endif
}

void Board::apply_move(Move move)
{
    if (!this->engine_data_updated)
        _update_engine_at_turn_start();

    char piece = move.piece == EMPTY_CELL ? _get_cell(move.src) : move.piece;
    if (piece == 'P')
        _move_white_pawn(move.src, move.dst, move.promotion);
    else if (piece == 'N')
        _apply_regular_white_move(move.src, move.dst, &white_knights);
    else if (piece == 'B')
        _apply_regular_white_move(move.src, move.dst, &white_bishops);
    else if (piece == 'R')
    {
        _apply_regular_white_move(move.src, move.dst, &white_rooks);

        white_castles &= ~move.src;

        if (_count_trailing_zeros(move.src) % 8 <= 3)
        {
            serialized_white_castles &= 0b00001111;
        }
        else
        {
            serialized_white_castles &= 0b11110000;
        }
    }
    else if (piece == 'Q')
        _apply_regular_white_move(move.src, move.dst, &white_queens);
    else if (piece == 'K')
        _move_white_king(move.src, move.dst, move.castle_info);
    else if (piece == 'p')
        _move_black_pawn(move.src, move.dst, move.promotion);
    else if (piece == 'n')
        _apply_regular_black_move(move.src, move.dst, &black_knights);
    else if (piece == 'b')
        _apply_regular_black_move(move.src, move.dst, &black_bishops);
    else if (piece == 'r')
    {
        _apply_regular_black_move(move.src, move.dst, &black_rooks);

        black_castles &= ~move.src;

        if (_count_trailing_zeros(move.src) % 8 <= 3)
        {
            serialized_black_castles &= 0b00001111;
        }
        else
        {
            serialized_black_castles &= 0b11110000;
        }
    }
    else if (piece == 'q')
        _apply_regular_black_move(move.src, move.dst, &black_queens);
    else if (piece == 'k')
        _move_black_king(move.src, move.dst, move.castle_info);

#if USE_VISUAL_BOARD == 1
    this->visual_board.resetBoard();
    this->visual_board.updateBoard('P', white_pawns);
    this->visual_board.updateBoard('N', white_knights);
    this->visual_board.updateBoard('B', white_bishops);
    this->visual_board.updateBoard('R', white_rooks);
    this->visual_board.updateBoard('Q', white_queens);
    this->visual_board.updateBoard('K', white_king);
    this->visual_board.updateBoard('p', black_pawns);
    this->visual_board.updateBoard('n', black_knights);
    this->visual_board.updateBoard('b', black_bishops);
    this->visual_board.updateBoard('r', black_rooks);
    this->visual_board.updateBoard('q', black_queens);
    this->visual_board.updateBoard('k', black_king);

    if (PRINT_TURNS)
        this->visual_board.printBoard();
#endif

    _update_engine_at_turn_end();
}

float Board::get_game_state()
{
    if (!this->game_state_computed)
    {
        if (!this->engine_data_updated)
            _update_engine_at_turn_start();

        this->game_state = _compute_game_state();
    }

    return this->game_state;
}

bool Board::get_check_state()
{
    if (!this->engine_data_updated)
        _update_engine_at_turn_start();

    return this->check_state;
}

char Board::get_cell(int x, int y)
{
    uint64_t pos_mask = 1UL << (y * 8 + x);

    return _get_cell(pos_mask);
}

uint64_t Board::get_castling_rights()
{
    return white_castles | black_castles;
}

vector<Move> Board::get_available_moves()
{
    if (!this->moves_computed)
    {
        if (!this->engine_data_updated)
            _update_engine_at_turn_start();

        _find_moves();
    }

    return this->available_moves;
}

string Board::get_name()
{
    return "BitBoard";
}

string Board::create_fen(bool with_turns)
{
    char fen[85];
    int  fen_i = 0;

    bzero(fen, 85);

    int      empty_cells_count = 0;
    uint64_t pos_mask = 1UL;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            char piece = _get_cell(pos_mask);

            if (piece == EMPTY_CELL)
            {
                empty_cells_count++;
                pos_mask <<= 1;
                continue;
            }

            if (empty_cells_count > 0)
            {
                fen[fen_i++] = '0' + empty_cells_count;
                empty_cells_count = 0;
            }

            fen[fen_i++] = piece;
            pos_mask <<= 1;
        }

        if (empty_cells_count > 0)
        {
            fen[fen_i++] = '0' + empty_cells_count;
            empty_cells_count = 0;
        }

        if (y != 7)
            fen[fen_i++] = '/';
    }
    fen[fen_i++] = ' ';

    fen[fen_i++] = white_turn ? 'w' : 'b';
    fen[fen_i++] = ' ';

    if (white_castles || black_castles)
    {
        if (this->chess960_rule)
            _create_fen_for_chess960_castling(fen, &fen_i);
        else
            _create_fen_for_standard_castling(fen, &fen_i);
    }
    else
        fen[fen_i++] = '-';
    fen[fen_i++] = ' ';

    if (en_passant)
    {
        string en_passant_str = bitboard_to_algebraic(en_passant);
        fen[fen_i++] = en_passant_str[0];
        fen[fen_i++] = en_passant_str[1];
    }
    else
        fen[fen_i++] = '-';
    fen[fen_i++] = ' ';

    string fen_string = string(fen, fen_i);

    if (!with_turns)
        return fen_string;

    fen_string += to_string(half_turn_rule);
    fen_string += string(" ");

    fen_string += to_string(game_turn);

    return fen_string;
}

Board *Board::clone()
{
    Board *cloned_board = new Board();

    *cloned_board = *this;

    return cloned_board;
}

void Board::_main_parsing(
    string _board,
    string _color,
    string _castling,
    string _en_passant,
    int    _half_turn_rule,
    int    _game_turn,
    bool   _chess960_rule,
    bool   _codingame_rule
)
{
#if USE_VISUAL_BOARD == 1
    this->visual_board = VisualBoard();
#endif

    if (Board::lookup_tables_initialized == false)
        Board::_initialize_lookup_tables();

    chess960_rule = _chess960_rule;
    codingame_rule = _codingame_rule;

    _initialize_bitboards();

    _parse_board(_board);
    white_turn = _color == "w";
    _parse_castling(_castling);
    en_passant = _en_passant != "-" ? algebraic_to_bitboard(_en_passant) : 0;
    next_turn_en_passant = 0UL;
    half_turn_rule = _half_turn_rule;
    game_turn = _game_turn;

    moves_computed = false;
    game_state_computed = false;
    engine_data_updated = false;

    current_sfen_history_index = 0;
    current_sfen = NULL;
    _update_serialized_fen_history();
}

void Board::_initialize_bitboards()
{
    white_pawns = 0UL;
    white_knights = 0UL;
    white_bishops = 0UL;
    white_rooks = 0UL;
    white_queens = 0UL;
    white_king = 0UL;
    black_pawns = 0UL;
    black_knights = 0UL;
    black_bishops = 0UL;
    black_rooks = 0UL;
    black_queens = 0UL;
    black_king = 0UL;

    white_castles = 0UL;
    black_castles = 0UL;
    en_passant = 0UL;

    white_pieces_mask = 0UL;
    black_pieces_mask = 0UL;
    not_white_pieces_mask = 0UL;
    not_black_pieces_mask = 0UL;
    all_pieces_mask = 0UL;
    empty_cells_mask = 0UL;
}

void Board::_parse_board(string fen_board)
{
    char pos_index = 0;

    for (size_t i = 0; i < fen_board.length(); i++)
    {
        char piece = fen_board[i];

        if (isdigit(piece))
        {
            pos_index += atoi(&fen_board[i]);
        }
        else if (piece != '/')
        {
            white_pawns |= (uint64_t)(piece == 'P') << pos_index;
            white_knights |= (uint64_t)(piece == 'N') << pos_index;
            white_bishops |= (uint64_t)(piece == 'B') << pos_index;
            white_rooks |= (uint64_t)(piece == 'R') << pos_index;
            white_queens |= (uint64_t)(piece == 'Q') << pos_index;
            white_king |= (uint64_t)(piece == 'K') << pos_index;
            black_pawns |= (uint64_t)(piece == 'p') << pos_index;
            black_knights |= (uint64_t)(piece == 'n') << pos_index;
            black_bishops |= (uint64_t)(piece == 'b') << pos_index;
            black_rooks |= (uint64_t)(piece == 'r') << pos_index;
            black_queens |= (uint64_t)(piece == 'q') << pos_index;
            black_king |= (uint64_t)(piece == 'k') << pos_index;

            pos_index++;
        }
    }

#if USE_VISUAL_BOARD == 1
    this->visual_board.resetBoard();
    this->visual_board.updateBoard('P', white_pawns);
    this->visual_board.updateBoard('N', white_knights);
    this->visual_board.updateBoard('B', white_bishops);
    this->visual_board.updateBoard('R', white_rooks);
    this->visual_board.updateBoard('Q', white_queens);
    this->visual_board.updateBoard('K', white_king);
    this->visual_board.updateBoard('p', black_pawns);
    this->visual_board.updateBoard('n', black_knights);
    this->visual_board.updateBoard('b', black_bishops);
    this->visual_board.updateBoard('r', black_rooks);
    this->visual_board.updateBoard('q', black_queens);
    this->visual_board.updateBoard('k', black_king);
#endif
}

void Board::_parse_castling(string castling_fen)
{
    this->serialized_white_castles = 0;
    this->serialized_black_castles = 0;

    if (castling_fen == "-")
        return;

    uint8_t rook_index;
    for (size_t i = 0; i < castling_fen.length(); i++)
    {
        if (isupper(castling_fen[i]))
        {
            if (this->chess960_rule)
            {
                this->white_castles |= algebraic_to_bitboard(string(1, castling_fen[i]) + '1');

                rook_index = column_name_to_index(castling_fen[i]);
            }
            else
            {
                if (castling_fen[i] == 'K')
                {
                    this->white_castles |= 0x8000000000000000UL;
                    rook_index = 7;
                }
                else
                {
                    this->white_castles |= 0x0100000000000000UL;
                    rook_index = 0;
                }
            }

            this->serialized_white_castles |= rook_index <= 3 ? rook_index << 4 : rook_index;
        }
        else
        {
            if (this->chess960_rule)
            {
                this->black_castles |= algebraic_to_bitboard(string(1, castling_fen[i]) + '8');

                rook_index = column_name_to_index(castling_fen[i]);
            }
            else
            {
                if (castling_fen[i] == 'k')
                {
                    this->black_castles |= 0b10000000UL;
                    rook_index = 7;
                }
                else
                {
                    this->black_castles |= 0b00000001UL;
                    rook_index = 0;
                }
            }

            this->serialized_black_castles |= rook_index <= 3 ? rook_index << 4 : rook_index;
        }
    }
}

char Board::_get_cell(uint64_t mask)
{
    if (white_pawns & mask)
        return 'P';
    if (white_knights & mask)
        return 'N';
    if (white_bishops & mask)
        return 'B';
    if (white_rooks & mask)
        return 'R';
    if (white_queens & mask)
        return 'Q';
    if (white_king & mask)
        return 'K';
    if (black_pawns & mask)
        return 'p';
    if (black_knights & mask)
        return 'n';
    if (black_bishops & mask)
        return 'b';
    if (black_rooks & mask)
        return 'r';
    if (black_queens & mask)
        return 'q';
    if (black_king & mask)
        return 'k';

    return EMPTY_CELL;
}

void Board::_create_fen_for_standard_castling(char *fen, int *fen_i)
{
    uint64_t white_mask = 1UL << 63;
    for (int x = 7; x >= 0; x--)
    {
        if (white_castles & white_mask)
        {
            fen[*fen_i] = x == 0 ? 'Q' : 'K';
            (*fen_i)++;
        }
        white_mask = 1UL << (55 + x);
    }

    uint64_t black_mask = 1UL << 7;
    for (int x = 7; x >= 0; x--)
    {
        if (black_castles & black_mask)
        {
            fen[*fen_i] = x == 0 ? 'q' : 'k';
            (*fen_i)++;
        }
        black_mask >>= 1;
    }
}

void Board::_create_fen_for_chess960_castling(char *fen, int *fen_i)
{
    uint64_t white_mask = 1UL << 56;
    for (int x = 0; x < 8; x++)
    {
        if (white_castles & white_mask)
        {
            fen[*fen_i] = toupper(column_index_to_name(x));
            (*fen_i)++;
        }
        white_mask <<= 1;
    }

    uint64_t black_mask = 1UL;
    for (int x = 0; x < 8; x++)
    {
        if (black_castles & black_mask)
        {
            fen[*fen_i] = column_index_to_name(x);
            (*fen_i)++;
        }
        black_mask <<= 1;
    }
}

void Board::_apply_regular_white_move(uint64_t src, uint64_t dst, uint64_t *piece_mask)
{
    _capture_black_pieces(dst);

    *piece_mask &= ~src;
    *piece_mask |= dst;
}

void Board::_apply_regular_black_move(uint64_t src, uint64_t dst, uint64_t *piece_mask)
{
    _capture_white_pieces(dst);

    *piece_mask &= ~src;
    *piece_mask |= dst;
}

void Board::_move_white_pawn(uint64_t src, uint64_t dst, char promotion)
{
    half_turn_rule = -1;

    if (dst == en_passant)
        black_pawns &= (~en_passant) << 8;

    if ((src & 0x00FF000000000000UL) && (dst & 0x000000FF00000000UL))
        next_turn_en_passant = src >> 8;

    _capture_black_pieces(dst);

    white_pawns &= ~src;

    char final_piece = promotion ? toupper(promotion) : 'P';
    if (final_piece == 'P')
        white_pawns |= dst;
    else if (final_piece == 'N')
        white_knights |= dst;
    else if (final_piece == 'B')
        white_bishops |= dst;
    else if (final_piece == 'R')
        white_rooks |= dst;
    else if (final_piece == 'Q')
        white_queens |= dst;
}

void Board::_move_black_pawn(uint64_t src, uint64_t dst, char promotion)
{
    half_turn_rule = -1;

    if (dst == en_passant)
        white_pawns &= (~en_passant) >> 8;

    if ((src & 0x000000000000FF00UL) && (dst & 0x00000000FF000000UL))
        next_turn_en_passant = src << 8;

    _capture_white_pieces(dst);

    black_pawns &= ~src;

    char final_piece = promotion ? promotion : 'p';
    if (final_piece == 'p')
        black_pawns |= dst;
    else if (final_piece == 'n')
        black_knights |= dst;
    else if (final_piece == 'b')
        black_bishops |= dst;
    else if (final_piece == 'r')
        black_rooks |= dst;
    else if (final_piece == 'q')
        black_queens |= dst;
}

void Board::_move_white_king(uint64_t src, uint64_t dst, castle_info_e castle_info)
{
    if (castle_info == NOINFO)
    {
        if ((src & white_king) && (dst & white_rooks))
        {
            castle_info = dst < src ? WHITELEFT : WHITERIGHT;
        }
        else
            castle_info = NOTCASTLE;
    }

    if (castle_info == NOTCASTLE)
    {
        _capture_black_pieces(dst);

        white_king = dst;
    }
    else if (castle_info == WHITELEFT)
    {
        white_rooks &= ~dst;
        white_king = BITMASK_CASTLE_WHITE_LEFT_KING;
        white_rooks |= BITMASK_CASTLE_WHITE_LEFT_ROOK;
    }
    else if (castle_info == WHITERIGHT)
    {
        white_rooks &= ~dst;
        white_king = BITMASK_CASTLE_WHITE_RIGHT_KING;
        white_rooks |= BITMASK_CASTLE_WHITE_RIGHT_ROOK;
    }

    white_castles = 0UL;
    serialized_white_castles = 0;
}

void Board::_move_black_king(uint64_t src, uint64_t dst, castle_info_e castle_info)
{
    if (castle_info == NOINFO)
    {
        if ((src & black_king) && (dst & black_rooks))
        {
            castle_info = dst < src ? BLACKLEFT : BLACKRIGHT;
        }
        else
            castle_info = NOTCASTLE;
    }

    if (castle_info == NOTCASTLE)
    {
        _capture_white_pieces(dst);

        black_king = dst;
    }
    else if (castle_info == BLACKLEFT)
    {
        black_rooks &= ~dst;
        black_king = BITMASK_CASTLE_BLACK_LEFT_KING;
        black_rooks |= BITMASK_CASTLE_BLACK_LEFT_ROOK;
    }
    else if (castle_info == BLACKRIGHT)
    {
        black_rooks &= ~dst;
        black_king = BITMASK_CASTLE_BLACK_RIGHT_KING;
        black_rooks |= BITMASK_CASTLE_BLACK_RIGHT_ROOK;
    }

    black_castles = 0UL;
    serialized_black_castles = 0;
}

void Board::_capture_white_pieces(uint64_t dst)
{
    if (all_pieces_mask & dst)
    {
        half_turn_rule = -1;

        uint64_t not_dst_mask = ~dst;

        if (dst & white_castles)
        {
            white_castles &= not_dst_mask;
            white_rooks &= not_dst_mask;
            return;
        }

        white_pawns &= not_dst_mask;
        white_knights &= not_dst_mask;
        white_bishops &= not_dst_mask;
        white_rooks &= not_dst_mask;
        white_queens &= not_dst_mask;
    }
}

void Board::_capture_black_pieces(uint64_t dst)
{
    if (all_pieces_mask & dst)
    {
        half_turn_rule = -1;

        uint64_t not_dst_mask = ~dst;

        if (dst & black_castles)
        {
            black_castles &= not_dst_mask;
            black_rooks &= not_dst_mask;
            return;
        }

        black_pawns &= not_dst_mask;
        black_knights &= not_dst_mask;
        black_bishops &= not_dst_mask;
        black_rooks &= not_dst_mask;
        black_queens &= not_dst_mask;
    }
}

void Board::_update_engine_at_turn_start()
{
    white_pieces_mask =
        white_pawns | white_knights | white_bishops | white_rooks | white_queens | white_king;
    black_pieces_mask =
        black_pawns | black_knights | black_bishops | black_rooks | black_queens | black_king;
    not_white_pieces_mask = ~white_pieces_mask;
    not_black_pieces_mask = ~black_pieces_mask;

    all_pieces_mask = white_pieces_mask | black_pieces_mask;
    empty_cells_mask = ~all_pieces_mask;

    if (white_turn)
    {
        ally_king = white_king;
        ally_pieces = white_pieces_mask;

        enemy_pawns = black_pawns;
        enemy_knights = black_knights;
        enemy_bishops = black_bishops;
        enemy_rooks = black_rooks;
        enemy_queens = black_queens;
    }
    else
    {
        ally_king = black_king;
        ally_pieces = black_pieces_mask;

        enemy_pawns = white_pawns;
        enemy_knights = white_knights;
        enemy_bishops = white_bishops;
        enemy_rooks = white_rooks;
        enemy_queens = white_queens;
    }
    enemy_pieces_sliding_diag = enemy_bishops | enemy_queens;
    enemy_pieces_sliding_line = enemy_rooks | enemy_queens;

    capturable_by_white_pawns_mask = black_pieces_mask | en_passant;
    capturable_by_black_pawns_mask = white_pieces_mask | en_passant;

    check_state = false;
    double_check = false;
    uncheck_mask = 0UL;
    pawn_uncheck_mask = 0UL;
    std::fill(std::begin(pin_masks), std::end(pin_masks), BITMASK_ALL_CELLS);
    attacked_by_white_mask = 0UL;
    attacked_by_black_mask = 0UL;

    _update_check_and_pins();
    _update_attacked_cells_masks();

    pawn_uncheck_mask |= uncheck_mask;

    engine_data_updated = true;
}

void Board::_update_check_and_pins()
{
    if (ally_king == 0UL)
    {
        check_state = false;
        uncheck_mask = BITMASK_ALL_CELLS;
        return;
    }

    int king_lkt_i = _count_trailing_zeros(ally_king);

    _update_pawn_check(king_lkt_i);

    uint64_t knight_attacks = knight_lookup[king_lkt_i] & enemy_knights;
    if (knight_attacks)
    {
        check_state = true;
        uncheck_mask |= knight_attacks;
    }

    _compute_sliding_piece_negative_ray_checks_and_pins(
        ally_king, NORTHEAST, enemy_pieces_sliding_diag
    );
    _compute_sliding_piece_positive_ray_checks_and_pins(
        ally_king, SOUTHEAST, enemy_pieces_sliding_diag
    );
    _compute_sliding_piece_positive_ray_checks_and_pins(
        ally_king, SOUTHWEST, enemy_pieces_sliding_diag
    );
    _compute_sliding_piece_negative_ray_checks_and_pins(
        ally_king, NORTHWEST, enemy_pieces_sliding_diag
    );

    _compute_sliding_piece_negative_ray_checks_and_pins(
        ally_king, NORTH, enemy_pieces_sliding_line
    );
    _compute_sliding_piece_positive_ray_checks_and_pins(ally_king, EAST, enemy_pieces_sliding_line);
    _compute_sliding_piece_positive_ray_checks_and_pins(
        ally_king, SOUTH, enemy_pieces_sliding_line
    );
    _compute_sliding_piece_negative_ray_checks_and_pins(ally_king, WEST, enemy_pieces_sliding_line);

    if (uncheck_mask == 0UL)
    {
        check_state = false;
        uncheck_mask = BITMASK_ALL_CELLS;
    }
}

void Board::_update_pawn_check(int king_lkt_i)
{
    int lkt_color = white_turn ? 0 : 1;

    uint64_t attacking_pawn = pawn_captures_lookup[king_lkt_i][lkt_color] & enemy_pawns;
    if (attacking_pawn)
    {
        check_state = true;
        uncheck_mask |= attacking_pawn;

        if (en_passant)
        {
            if (white_turn)
            {
                if (attacking_pawn == (en_passant << 8))
                {
                    if (!_is_sliding_piece_negative_diagonal_ray_behind(
                            attacking_pawn, ally_king == attacking_pawn - 9 ? NORTHWEST : NORTHEAST
                        ))
                        pawn_uncheck_mask = en_passant;
                }
            }
            else
            {
                if (attacking_pawn == (en_passant >> 8))
                {
                    if (!_is_sliding_piece_positive_diagonal_ray_behind(
                            attacking_pawn, ally_king == attacking_pawn + 7 ? SOUTHWEST : SOUTHEAST
                        ))
                        pawn_uncheck_mask = en_passant;
                }
            }
        }
    }
}

void Board::_update_attacked_cells_masks()
{
    _apply_function_on_all_pieces(&Board::_find_black_pawns_attacks, black_pawns);
    _apply_function_on_all_pieces(&Board::_find_black_knights_attacks, black_knights);
    _apply_function_on_all_pieces(&Board::_find_black_bishops_attacks, black_bishops);
    _apply_function_on_all_pieces(&Board::_find_black_rooks_attacks, black_rooks);
    _apply_function_on_all_pieces(&Board::_find_black_queens_attacks, black_queens);
    _find_black_king_attacks();

    _apply_function_on_all_pieces(&Board::_find_white_pawns_attacks, white_pawns);
    _apply_function_on_all_pieces(&Board::_find_white_knights_attacks, white_knights);
    _apply_function_on_all_pieces(&Board::_find_white_bishops_attacks, white_bishops);
    _apply_function_on_all_pieces(&Board::_find_white_rooks_attacks, white_rooks);
    _apply_function_on_all_pieces(&Board::_find_white_queens_attacks, white_queens);
    _find_white_king_attacks();
}

void Board::_update_engine_at_turn_end()
{
#if USE_VISUAL_BOARD == 1
    if (PRINT_DEBUG_DATA)
    {
        this->log(true);
        this->visual_board.printSpecificBoard('W', attacked_by_white_mask, "Attacked by white");
        this->visual_board.printSpecificBoard('B', attacked_by_black_mask, "Attacked by black");
    }
#endif

    moves_computed = false;
    game_state_computed = false;
    engine_data_updated = false;

    en_passant = next_turn_en_passant;
    next_turn_en_passant = 0UL;

    half_turn_rule += 1;

    if (!white_turn)
        game_turn += 1;
    white_turn = !white_turn;

    _update_serialized_fen_history();
}

uint8_t Board::_serialize_en_passant()
{
    if (en_passant == 0)
        return 0;

    uint8_t pos = _count_trailing_zeros(en_passant);

    return (pos % 8) << 3 | (pos / 8);
}

void Board::_update_serialized_fen_history()
{

    current_sfen_history_index++;

    if (current_sfen_history_index == FEN_HISTORY_SIZE)
        current_sfen_history_index = 0;

    current_sfen = &serialized_fen_history[current_sfen_history_index];

    current_sfen->serialized_pawns = white_pawns | ((__int128)black_pawns << 64);
    current_sfen->serialized_knights = white_knights | ((__int128)black_knights << 64);
    current_sfen->serialized_bishops = white_bishops | ((__int128)black_bishops << 64);
    current_sfen->serialized_rooks = white_rooks | ((__int128)black_rooks << 64);
    current_sfen->serialized_queens = white_queens | ((__int128)black_queens << 64);
    current_sfen->serialized_kings = white_king | ((__int128)black_king << 64);

    uint8_t turns_bit = (white_turn ? 0b0 : 0b11111111);
    uint8_t en_passant_bits = _serialize_en_passant();

    current_sfen->serialized_last_info = turns_bit | ((uint32_t)serialized_white_castles << 8) |
                                         ((uint32_t)serialized_black_castles << 16) |
                                         ((uint32_t)en_passant_bits << 24);
}

void Board::_find_white_pawns_attacks(uint64_t src)
{
    int src_lkt_i = _count_trailing_zeros(src);
    attacked_by_white_mask |= pawn_captures_lookup[src_lkt_i][0];
}

void Board::_find_white_knights_attacks(uint64_t src)
{
    int src_lkt_i = _count_trailing_zeros(src);
    attacked_by_white_mask |= knight_lookup[src_lkt_i];
}

void Board::_find_white_bishops_attacks(uint64_t src)
{
    attacked_by_white_mask |= _get_diagonal_rays(src, black_king);
}

void Board::_find_white_rooks_attacks(uint64_t src)
{
    attacked_by_white_mask |= _get_line_rays(src, black_king);
}

void Board::_find_white_queens_attacks(uint64_t src)
{
    attacked_by_white_mask |= _get_diagonal_rays(src, black_king) | _get_line_rays(src, black_king);
}

void Board::_find_white_king_attacks()
{
    if (white_king)
    {
        int src_lkt_i = _count_trailing_zeros(white_king);
        attacked_by_white_mask |= king_lookup[src_lkt_i];
    }
}

void Board::_find_black_pawns_attacks(uint64_t src)
{
    int src_lkt_i = _count_trailing_zeros(src);
    attacked_by_black_mask |= pawn_captures_lookup[src_lkt_i][1];
}

void Board::_find_black_knights_attacks(uint64_t src)
{
    int src_lkt_i = _count_trailing_zeros(src);
    attacked_by_black_mask |= knight_lookup[src_lkt_i];
}

void Board::_find_black_bishops_attacks(uint64_t src)
{
    attacked_by_black_mask |= _get_diagonal_rays(src, white_king);
}

void Board::_find_black_rooks_attacks(uint64_t src)
{
    attacked_by_black_mask |= _get_line_rays(src, white_king);
}

void Board::_find_black_queens_attacks(uint64_t src)
{
    attacked_by_black_mask |= _get_diagonal_rays(src, white_king) | _get_line_rays(src, white_king);
}

void Board::_find_black_king_attacks()
{
    if (black_king)
    {
        int src_lkt_i = _count_trailing_zeros(black_king);
        attacked_by_black_mask |= king_lookup[src_lkt_i];
    }
}

void Board::_find_moves()
{
    this->available_moves.clear();

    if (white_turn)
    {
        if (!double_check)
        {
            _apply_function_on_all_pieces(&Board::_find_white_pawns_moves, white_pawns);
            _apply_function_on_all_pieces(&Board::_find_white_queens_moves, white_queens);
            _apply_function_on_all_pieces(&Board::_find_white_rooks_moves, white_rooks);
            _apply_function_on_all_pieces(&Board::_find_white_bishops_moves, white_bishops);
            _apply_function_on_all_pieces(&Board::_find_white_knights_moves, white_knights);
            _apply_function_on_all_pieces(&Board::_find_white_castle_moves, white_castles);
        }
        _find_white_king_moves();
    }
    else
    {
        if (!double_check)
        {
            _apply_function_on_all_pieces(&Board::_find_black_pawns_moves, black_pawns);
            _apply_function_on_all_pieces(&Board::_find_black_queens_moves, black_queens);
            _apply_function_on_all_pieces(&Board::_find_black_rooks_moves, black_rooks);
            _apply_function_on_all_pieces(&Board::_find_black_bishops_moves, black_bishops);
            _apply_function_on_all_pieces(&Board::_find_black_knights_moves, black_knights);
            _apply_function_on_all_pieces(&Board::_find_black_castle_moves, black_castles);
        }
        _find_black_king_moves();
    }

    this->moves_computed = true;
}

void Board::_find_white_pawns_moves(uint64_t src)
{
    int src_lkt_i = _count_trailing_zeros(src);

    uint64_t capture_moves = pawn_captures_lookup[src_lkt_i][0] & capturable_by_white_pawns_mask;
    uint64_t advance_move = (src >> 8) & empty_cells_mask;
    uint64_t legal_moves =
        (capture_moves | advance_move) & pawn_uncheck_mask & pin_masks[src_lkt_i];

    if (src & BITMASK_LINE_2 && (src >> 8) & empty_cells_mask)
        legal_moves |= (src >> 16) & empty_cells_mask & pawn_uncheck_mask & pin_masks[src_lkt_i];

    uint64_t legal_advances = legal_moves & ~BITMASK_LINE_8;
    uint64_t legal_promotions = legal_moves & BITMASK_LINE_8;

    _apply_function_on_all_pieces(&Board::_create_move, legal_advances, 'P', src);
    _apply_function_on_all_pieces(
        &Board::_create_white_pawn_promotions, legal_promotions, 'P', src
    );
}

void Board::_find_white_knights_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves =
        knight_lookup[src_lkt_i] & not_white_pieces_mask & uncheck_mask & pin_masks[src_lkt_i];

    _apply_function_on_all_pieces(&Board::_create_move, legal_moves, 'N', src);
}

void Board::_find_white_bishops_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves =
        not_white_pieces_mask & _get_diagonal_rays(src) & uncheck_mask & pin_masks[src_lkt_i];

    _apply_function_on_all_pieces(&Board::_create_move, legal_moves, 'B', src);
}

void Board::_find_white_rooks_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves =
        not_white_pieces_mask & _get_line_rays(src) & uncheck_mask & pin_masks[src_lkt_i];

    _apply_function_on_all_pieces(&Board::_create_move, legal_moves, 'R', src);
}

void Board::_find_white_queens_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves = not_white_pieces_mask & (_get_diagonal_rays(src) | _get_line_rays(src)) &
                           uncheck_mask & pin_masks[src_lkt_i];

    _apply_function_on_all_pieces(&Board::_create_move, legal_moves, 'Q', src);
}

void Board::_find_white_king_moves()
{
    if (white_king)
    {
        int      src_lkt_i = _count_trailing_zeros(white_king);
        uint64_t legal_moves =
            king_lookup[src_lkt_i] & not_white_pieces_mask & ~attacked_by_black_mask;

        _apply_function_on_all_pieces(&Board::_create_move, legal_moves, 'K', white_king);
    }
}

void Board::_find_white_castle_moves(uint64_t rook)
{
    if (white_king && !check_state)
    {
        uint64_t      rook_path;
        uint64_t      king_path;
        castle_info_e castle_info;
        if (rook < white_king)
        {
            castle_info = WHITELEFT;

            if (white_king < BITMASK_CASTLE_WHITE_LEFT_KING)
                king_path =
                    _compute_castling_positive_path(white_king, BITMASK_CASTLE_WHITE_LEFT_KING);
            else
                king_path =
                    _compute_castling_negative_path(white_king, BITMASK_CASTLE_WHITE_LEFT_KING);
            if (rook < BITMASK_CASTLE_WHITE_LEFT_ROOK)
                rook_path = _compute_castling_positive_path(rook, BITMASK_CASTLE_WHITE_LEFT_ROOK);
            else
                rook_path = _compute_castling_negative_path(rook, BITMASK_CASTLE_WHITE_LEFT_ROOK);
        }
        else
        {
            castle_info = WHITERIGHT;

            if (white_king < BITMASK_CASTLE_WHITE_RIGHT_KING)
                king_path =
                    _compute_castling_positive_path(white_king, BITMASK_CASTLE_WHITE_RIGHT_KING);
            else
                king_path =
                    _compute_castling_negative_path(white_king, BITMASK_CASTLE_WHITE_RIGHT_KING);
            if (rook < BITMASK_CASTLE_WHITE_RIGHT_ROOK)
                rook_path = _compute_castling_positive_path(rook, BITMASK_CASTLE_WHITE_RIGHT_ROOK);
            else
                rook_path = _compute_castling_negative_path(rook, BITMASK_CASTLE_WHITE_RIGHT_ROOK);
        }

        if (((king_path | rook_path) & (all_pieces_mask ^ white_king ^ rook)) == 0UL &&
            (king_path & attacked_by_black_mask) == 0UL &&
            pin_masks[_count_trailing_zeros(rook)] == BITMASK_ALL_CELLS)
        {
            this->available_moves.push_back(Move('K', white_king, rook, 0, castle_info));
        }
    }
}

void Board::_find_black_pawns_moves(uint64_t src)
{
    int src_lkt_i = _count_trailing_zeros(src);

    uint64_t capture_moves = pawn_captures_lookup[src_lkt_i][1] & capturable_by_black_pawns_mask;
    uint64_t advance_move = (src << 8) & empty_cells_mask;
    uint64_t legal_moves =
        (capture_moves | advance_move) & pawn_uncheck_mask & pin_masks[src_lkt_i];

    if (src & BITMASK_LINE_7 && (src << 8) & empty_cells_mask)
        legal_moves |= (src << 16) & empty_cells_mask & pawn_uncheck_mask & pin_masks[src_lkt_i];

    uint64_t legal_advances = legal_moves & ~BITMASK_LINE_1;
    uint64_t legal_promotions = legal_moves & BITMASK_LINE_1;

    _apply_function_on_all_pieces(&Board::_create_move, legal_advances, 'p', src);
    _apply_function_on_all_pieces(
        &Board::_create_black_pawn_promotions, legal_promotions, 'p', src
    );
}

void Board::_find_black_knights_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves =
        knight_lookup[src_lkt_i] & not_black_pieces_mask & uncheck_mask & pin_masks[src_lkt_i];

    _apply_function_on_all_pieces(&Board::_create_move, legal_moves, 'n', src);
}

void Board::_find_black_bishops_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves =
        not_black_pieces_mask & _get_diagonal_rays(src) & uncheck_mask & pin_masks[src_lkt_i];

    _apply_function_on_all_pieces(&Board::_create_move, legal_moves, 'b', src);
}

void Board::_find_black_rooks_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves =
        not_black_pieces_mask & _get_line_rays(src) & uncheck_mask & pin_masks[src_lkt_i];

    _apply_function_on_all_pieces(&Board::_create_move, legal_moves, 'r', src);
}

void Board::_find_black_queens_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves = not_black_pieces_mask & (_get_diagonal_rays(src) | _get_line_rays(src)) &
                           uncheck_mask & pin_masks[src_lkt_i];

    _apply_function_on_all_pieces(&Board::_create_move, legal_moves, 'q', src);
}

void Board::_find_black_king_moves()
{
    if (black_king)
    {
        int      src_lkt_i = _count_trailing_zeros(black_king);
        uint64_t legal_moves =
            king_lookup[src_lkt_i] & not_black_pieces_mask & ~attacked_by_white_mask;

        _apply_function_on_all_pieces(&Board::_create_move, legal_moves, 'k', black_king);
    }
}

void Board::_find_black_castle_moves(uint64_t rook)
{
    if (black_king && !check_state)
    {
        uint64_t      rook_path;
        uint64_t      king_path;
        castle_info_e castle_info;
        if (rook < black_king)
        {
            castle_info = BLACKLEFT;

            if (black_king < BITMASK_CASTLE_BLACK_LEFT_KING)
                king_path =
                    _compute_castling_positive_path(black_king, BITMASK_CASTLE_BLACK_LEFT_KING);
            else
                king_path =
                    _compute_castling_negative_path(black_king, BITMASK_CASTLE_BLACK_LEFT_KING);
            if (rook < BITMASK_CASTLE_BLACK_LEFT_ROOK)
                rook_path = _compute_castling_positive_path(rook, BITMASK_CASTLE_BLACK_LEFT_ROOK);
            else
                rook_path = _compute_castling_negative_path(rook, BITMASK_CASTLE_BLACK_LEFT_ROOK);
        }
        else
        {
            castle_info = BLACKRIGHT;

            if (black_king < BITMASK_CASTLE_BLACK_RIGHT_KING)
                king_path =
                    _compute_castling_positive_path(black_king, BITMASK_CASTLE_BLACK_RIGHT_KING);
            else
                king_path =
                    _compute_castling_negative_path(black_king, BITMASK_CASTLE_BLACK_RIGHT_KING);
            if (rook < BITMASK_CASTLE_BLACK_RIGHT_ROOK)
                rook_path = _compute_castling_positive_path(rook, BITMASK_CASTLE_BLACK_RIGHT_ROOK);
            else
                rook_path = _compute_castling_negative_path(rook, BITMASK_CASTLE_BLACK_RIGHT_ROOK);
        }

        if (((king_path | rook_path) & (all_pieces_mask ^ black_king ^ rook)) == 0UL &&
            (king_path & attacked_by_white_mask) == 0UL &&
            (pin_masks[_count_trailing_zeros(rook)] == BITMASK_ALL_CELLS))
        {
            this->available_moves.push_back(Move('k', black_king, rook, 0, castle_info));
        }
    }
}

void Board::_create_white_pawn_promotions(char piece, uint64_t src, uint64_t dst)
{
    _create_promotion_move(piece, src, dst, 'N');
    _create_promotion_move(piece, src, dst, 'B');
    _create_promotion_move(piece, src, dst, 'R');
    _create_promotion_move(piece, src, dst, 'Q');
}

void Board::_create_black_pawn_promotions(char piece, uint64_t src, uint64_t dst)
{
    _create_promotion_move(piece, src, dst, 'n');
    _create_promotion_move(piece, src, dst, 'b');
    _create_promotion_move(piece, src, dst, 'r');
    _create_promotion_move(piece, src, dst, 'q');
}

void Board::_create_promotion_move(char piece, uint64_t src, uint64_t dst, char promotion)
{
    this->available_moves.push_back(Move(piece, src, dst, promotion));
}

void Board::_create_move(char piece, uint64_t src, uint64_t dst)
{
    this->available_moves.push_back(Move(piece, src, dst));
}

void Board::_apply_function_on_all_pieces(methodAddrWith1Params func, uint64_t bitboard)
{
    uint64_t piece;
    while (bitboard)
    {
        piece = _get_least_significant_bit(bitboard);
        (this->*func)(piece);

        bitboard ^= piece;
    }
}

void Board::_apply_function_on_all_pieces(
    methodAddrWith3Params func, uint64_t bitboard, char param1, uint64_t param2
)
{
    uint64_t piece;
    while (bitboard)
    {
        piece = _get_least_significant_bit(bitboard);
        (this->*func)(param1, param2, piece);

        bitboard ^= piece;
    }
}

uint64_t Board::_get_diagonal_rays(uint64_t src, uint64_t piece_to_ignore)
{
    return _compute_sliding_piece_positive_ray(src, SOUTHEAST, piece_to_ignore) |
           _compute_sliding_piece_positive_ray(src, SOUTHWEST, piece_to_ignore) |
           _compute_sliding_piece_negative_ray(src, NORTHWEST, piece_to_ignore) |
           _compute_sliding_piece_negative_ray(src, NORTHEAST, piece_to_ignore);
}

uint64_t Board::_get_line_rays(uint64_t src, uint64_t piece_to_ignore)
{
    return _compute_sliding_piece_positive_ray(src, EAST, piece_to_ignore) |
           _compute_sliding_piece_positive_ray(src, SOUTH, piece_to_ignore) |
           _compute_sliding_piece_negative_ray(src, WEST, piece_to_ignore) |
           _compute_sliding_piece_negative_ray(src, NORTH, piece_to_ignore);
}

uint64_t
Board::_compute_sliding_piece_positive_ray(uint64_t src, ray_dir_e dir, uint64_t piece_to_ignore)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t attacks = sliding_lookup[src_lkt_i][dir];

    uint64_t blockers = attacks & (all_pieces_mask ^ piece_to_ignore);
    if (blockers)
    {
        int src_lkt_i = _count_trailing_zeros(blockers);

        attacks ^= sliding_lookup[src_lkt_i][dir];
    }

    return attacks;
}

uint64_t
Board::_compute_sliding_piece_negative_ray(uint64_t src, ray_dir_e dir, uint64_t piece_to_ignore)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t attacks = sliding_lookup[src_lkt_i][dir];

    uint64_t blockers = attacks & (all_pieces_mask ^ piece_to_ignore);
    if (blockers)
    {
        uint64_t blocker = _get_most_significant_bit(blockers);
        int      src_lkt_i = _count_trailing_zeros(blocker);

        attacks ^= sliding_lookup[src_lkt_i][dir];
    }

    return attacks;
}

void Board::_compute_sliding_piece_positive_ray_checks_and_pins(
    uint64_t king_pos, ray_dir_e dir, uint64_t potential_attacker
)
{
    int      king_lkt_i = _count_trailing_zeros(king_pos);
    uint64_t attacks = sliding_lookup[king_lkt_i][dir];

    uint64_t blockers = attacks & all_pieces_mask;

    if (blockers)
    {
        uint64_t blocker = _get_least_significant_bit(blockers);
        int      blocker_lkt_i = _count_trailing_zeros(blocker);

        if (blocker & potential_attacker)
        {
            if (check_state)
                double_check = true;
            check_state = true;

            uncheck_mask |= attacks ^ sliding_lookup[blocker_lkt_i][dir];
        }

        else if (blocker & ally_pieces)
        {
            blockers ^= blocker;
            if (blockers)
            {
                blocker = _get_least_significant_bit(blockers);

                if (blocker & potential_attacker)
                {
                    int      second_blocker_lkt_i = _count_trailing_zeros(blocker);
                    uint64_t pin_ray = attacks ^ sliding_lookup[second_blocker_lkt_i][dir];

                    pin_masks[blocker_lkt_i] = pin_ray;
                }
            }
        }
    }
}

void Board::_compute_sliding_piece_negative_ray_checks_and_pins(
    uint64_t king_pos, ray_dir_e dir, uint64_t potential_attacker
)
{
    int      king_lkt_i = _count_trailing_zeros(king_pos);
    uint64_t attacks = sliding_lookup[king_lkt_i][dir];

    uint64_t blockers = attacks & all_pieces_mask;

    if (blockers)
    {
        uint64_t blocker = _get_most_significant_bit(blockers);
        int      blocker_lkt_i = _count_trailing_zeros(blocker);

        if (blocker & potential_attacker)
        {
            if (check_state)
                double_check = true;
            check_state = true;

            uncheck_mask |= attacks ^ sliding_lookup[blocker_lkt_i][dir];
        }

        else if (blocker & ally_pieces)
        {
            blockers ^= blocker;
            if (blockers)
            {
                blocker = _get_most_significant_bit(blockers);

                if (blocker & potential_attacker)
                {
                    int      second_blocker_lkt_i = _count_trailing_zeros(blocker);
                    uint64_t pin_ray = attacks ^ sliding_lookup[second_blocker_lkt_i][dir];

                    pin_masks[blocker_lkt_i] = pin_ray;
                }
            }
        }
    }
}

bool Board::_is_sliding_piece_positive_diagonal_ray_behind(uint64_t pawn_pos, ray_dir_e dir)
{
    int      pawn_lkt_i = _count_trailing_zeros(pawn_pos);
    uint64_t attacks = sliding_lookup[pawn_lkt_i][dir];

    uint64_t blockers = attacks & all_pieces_mask;
    if (blockers)
    {
        uint64_t blocker = _get_least_significant_bit(blockers);

        if (blocker & enemy_pieces_sliding_diag)
            return true;
    }

    return false;
}

bool Board::_is_sliding_piece_negative_diagonal_ray_behind(uint64_t pawn_pos, ray_dir_e dir)
{
    int      pawn_lkt_i = _count_trailing_zeros(pawn_pos);
    uint64_t attacks = sliding_lookup[pawn_lkt_i][dir];

    uint64_t blockers = attacks & all_pieces_mask;
    if (blockers)
    {
        uint64_t blocker = _get_most_significant_bit(blockers);

        if (blocker & enemy_pieces_sliding_diag)
            return true;
    }

    return false;
}

uint64_t Board::_compute_castling_positive_path(uint64_t src, uint64_t dst)
{
    int src_lkt_i = _count_trailing_zeros(src);
    int dst_lkt_i = _count_trailing_zeros(dst);

    return sliding_lookup[src_lkt_i][EAST] ^ sliding_lookup[dst_lkt_i][EAST];
}

uint64_t Board::_compute_castling_negative_path(uint64_t src, uint64_t dst)
{
    int src_lkt_i = _count_trailing_zeros(src);
    int dst_lkt_i = _count_trailing_zeros(dst);

    return sliding_lookup[src_lkt_i][WEST] ^ sliding_lookup[dst_lkt_i][WEST];
}

float Board::_compute_game_state()
{
    if (half_turn_rule >= 99 || _threefold_repetition_rule() || _insufficient_material_rule())
        return DRAW;

    if (codingame_rule)
    {
        if (game_turn > 125)
            return DRAW;
    }

    vector<Move> moves = get_available_moves();

    if (moves.size() == 0)
    {
        if (get_check_state())
            return white_turn ? BLACK_WIN : WHITE_WIN;
        return DRAW;
    }

    return GAME_CONTINUE;
}

bool Board::_threefold_repetition_rule()
{
    int max_history_size = min((game_turn + 1) * 2, FEN_HISTORY_SIZE);

    bool fen_found = false;
    int  i = -1;
    while (++i < max_history_size)
    {
        if (i != current_sfen_history_index &&
            memcmp(this->current_sfen, &serialized_fen_history[i], SIZEOF_T_SERIALIZED_FEN) == 0)
        {
            if (fen_found)
                return true;

            fen_found = true;
        }
    }

    return false;
}

bool Board::_insufficient_material_rule()
{
    if (white_pawns | black_pawns | white_rooks | black_rooks | white_queens | black_queens)
        return false;

    int white_knights_count = _count_bits(white_knights);
    int black_knights_count = _count_bits(black_knights);
    int knights_count = white_knights_count + black_knights_count;

    if (knights_count > 1)
        return false;

    int white_bishops_count = _count_bits(white_bishops);
    int black_bishops_count = _count_bits(black_bishops);
    int bishops_count = white_bishops_count + black_bishops_count;

    if (knights_count == 1 && bishops_count > 0)
        return false;

    uint64_t all_bishops = white_bishops | black_bishops;
    if ((all_bishops & BITMASK_WHITE_CELLS) && (all_bishops & BITMASK_BLACK_CELLS))
        return false;

    return true;
}

bool Board::operator==(Board *test_board_abstracted)
{
    return memcmp(
               this->current_sfen, test_board_abstracted->current_sfen, SIZEOF_T_SERIALIZED_FEN
           ) == 0;
}

void Board::_initialize_lookup_tables()
{
    memset(pawn_captures_lookup, 0, sizeof(uint64_t) * 64 * 2);
    memset(knight_lookup, 0, sizeof(uint64_t) * 64);
    memset(sliding_lookup, 0, sizeof(uint64_t) * 64 * 8);
    memset(king_lookup, 0, sizeof(uint64_t) * 64);

    int      lkt_i = 0;
    uint64_t pos_mask = 1UL;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            _create_pawn_captures_lookup_table(y, x, pos_mask, lkt_i);
            _create_knight_lookup_table(y, x, pos_mask, lkt_i);
            _create_sliding_lookup_table(y, x, pos_mask, lkt_i);
            _create_king_lookup_table(y, x, pos_mask, lkt_i);
            lkt_i++;
            pos_mask <<= 1;
        }
    }

    Board::lookup_tables_initialized = true;
}

void Board::_create_pawn_captures_lookup_table(int y, int x, uint64_t position, int lkt_i)
{

    uint64_t pawn_mask = 0UL;

    if (x > 0 && y > 0)
        pawn_mask |= (position >> 9);
    if (x < 7 && y > 0)
        pawn_mask |= (position >> 7);

    pawn_captures_lookup[lkt_i][0] = pawn_mask;

    pawn_mask = 0UL;

    if (x > 0 && y < 7)
        pawn_mask |= position << 7;
    if (x < 7 && y < 7)
        pawn_mask |= position << 9;

    pawn_captures_lookup[lkt_i][1] = pawn_mask;
}

void Board::_create_knight_lookup_table(int y, int x, uint64_t position, int lkt_i)
{
    uint64_t knight_mask = 0UL;

    if (y > 0)
    {
        if (x > 1)
            knight_mask |= position >> 10;
        if (x < 6)
            knight_mask |= position >> 6;
    }
    if (y > 1)
    {
        if (x > 0)
            knight_mask |= position >> 17;
        if (x < 7)
            knight_mask |= position >> 15;
    }
    if (y < 6)
    {
        if (x > 0)
            knight_mask |= position << 15;
        if (x < 7)
            knight_mask |= position << 17;
    }
    if (y < 7)
    {
        if (x > 1)
            knight_mask |= position << 6;
        if (x < 6)
            knight_mask |= position << 10;
    }

    knight_lookup[lkt_i] = knight_mask;
}

void Board::_create_sliding_lookup_table(int y, int x, uint64_t position, int lkt_i)
{
    uint64_t sliding_pos = position;
    for (int j = y - 1; j >= 0; j--)
    {
        sliding_pos >>= 8;
        sliding_lookup[lkt_i][NORTH] |= sliding_pos;
    }

    sliding_pos = position;
    for (int j = y - 1, i = x + 1; j >= 0 && i < 8; j--, i++)
    {
        sliding_pos >>= 7;
        sliding_lookup[lkt_i][NORTHEAST] |= sliding_pos;
    }

    sliding_pos = position;
    for (int i = x + 1; i < 8; i++)
    {
        sliding_pos <<= 1;
        sliding_lookup[lkt_i][EAST] |= sliding_pos;
    }

    sliding_pos = position;
    for (int j = y + 1, i = x + 1; j < 8 && i < 8; j++, i++)
    {
        sliding_pos <<= 9;
        sliding_lookup[lkt_i][SOUTHEAST] |= sliding_pos;
    }

    sliding_pos = position;
    for (int j = y + 1; j < 8; j++)
    {
        sliding_pos <<= 8;
        sliding_lookup[lkt_i][SOUTH] |= sliding_pos;
    }

    sliding_pos = position;
    for (int j = y + 1, i = x - 1; j < 8 && i >= 0; j++, i--)
    {
        sliding_pos <<= 7;
        sliding_lookup[lkt_i][SOUTHWEST] |= sliding_pos;
    }

    sliding_pos = position;
    for (int i = x - 1; i >= 0; i--)
    {
        sliding_pos >>= 1;
        sliding_lookup[lkt_i][WEST] |= sliding_pos;
    }

    sliding_pos = position;
    for (int j = y - 1, i = x - 1; j >= 0 && i >= 0; j--, i--)
    {
        sliding_pos >>= 9;
        sliding_lookup[lkt_i][NORTHWEST] |= sliding_pos;
    }
}

void Board::_create_king_lookup_table(int y, int x, uint64_t position, int lkt_i)
{
    uint64_t king_mask = 0UL;
    if (x > 0)
    {
        if (y > 0)
            king_mask |= position >> 9;
        king_mask |= position >> 1;
        if (y < 7)
            king_mask |= position << 7;
    }
    if (x < 7)
    {
        if (y > 0)
            king_mask |= position >> 7;
        king_mask |= position << 1;
        if (y < 7)
            king_mask |= position << 9;
    }
    if (y > 0)
        king_mask |= position >> 8;
    if (y < 7)
        king_mask |= position << 8;

    king_lookup[lkt_i] = king_mask;
}

/*
        Content of 'srcs/chessengine/Move.cpp'
*/

Move::Move(string _uci)
{
    this->uci = _uci;

    this->src = algebraic_to_bitboard(_uci.substr(0, 2));
    this->dst = algebraic_to_bitboard(_uci.substr(2, 2));

    this->promotion = _uci.length() > 4 ? _uci[4] : 0;

    this->piece = EMPTY_CELL;
    this->castle_info = NOINFO;
}

Move::Move(char _piece, uint64_t _src, uint64_t _dst, char _promotion, castle_info_e _castle_info)
{
    this->piece = _piece;
    this->src = _src;
    this->dst = _dst;
    this->promotion = _promotion;

    this->uci = "";
    this->castle_info = _castle_info;
}

void Move::log()
{
    cerr << "Move: Piece = " << this->piece << endl;
    cerr << "Move: src = " << bitboard_to_algebraic(this->src) << endl;
    cerr << "Move: dst = " << bitboard_to_algebraic(this->dst) << endl;
    cerr << "Move: Promote to " << (char)(this->promotion ? this->promotion : EMPTY_CELL) << endl;
    cerr << "Move: Castle info = " << this->castle_info << endl;
    cerr << "Move UCI: " << this->to_uci() << endl;
}

string Move::to_uci()
{
    return this->to_uci(CHESS960_RULES, this->castle_info > 1);
}

string Move::to_uci(bool chess960_rules, bool castling)
{

    if (this->uci != "")
        return this->uci;

    /*
        In my implementation, castling moves are always
       represented by a king moving to its own rook, as Chess960
       rules. With standard rules, the UCI representation of a
       castle is the king moving of 2 cells. So when the rules
       are standard, the destination position must be hardcoded
       so the UCI representation is correct.
    */
    uint64_t tmp_dst = this->dst;
    if (castling && !chess960_rules)
    {
        if (this->dst & 0xFFUL)
            tmp_dst = this->dst < this->src ? 1UL << 2 : 1UL << 6;
        else
            tmp_dst = this->dst < this->src ? 1UL << 58 : 1UL << 62;
    }

    string uci = bitboard_to_algebraic(this->src) + bitboard_to_algebraic(tmp_dst);

    if (this->promotion)
        uci += string(1, tolower(this->promotion));

    this->uci = uci;
    return uci;
}

bool Move::operator==(Move *other)
{
    return this->to_uci() == other->to_uci() &&
           (this->piece == other->piece || this->piece == EMPTY_CELL || other->piece == EMPTY_CELL
           ) &&
           this->src == other->src && this->dst == other->dst &&
           this->promotion == other->promotion && this->castle_info == other->castle_info;
}

bool Move::compare_move_vector(vector<Move> movelst1, vector<Move> movelst2)
{
    bool success = true;

    for (Move move_f : movelst1)
    {
        if (!_is_move_in_movelst(&move_f, movelst2))
        {
            cerr << "- Move "
                    "from "
                    "vector 1 "
                    "isn't in "
                    "vector 2 : "
                 << move_f.to_uci() << endl;
            move_f.log();
            success = false;
        }
    }

    for (Move move_f : movelst2)
    {
        if (!_is_move_in_movelst(&move_f, movelst1))
        {
            cerr << "- Move "
                    "from "
                    "vector 2 "
                    "isn't in "
                    "vector 1 : "
                 << move_f.to_uci() << endl;
            move_f.log();
            success = false;
        }
    }

    return success;
}

bool Move::_is_move_in_movelst(Move *move, vector<Move> movelst)
{
    for (Move move_f : movelst)
    {
        if (*move == &move_f)
            return true;
    }

    return false;
};

/*
        Content of 'srcs/agents/MinMaxAlphaBetaAgent.cpp'
*/

MinMaxAlphaBetaAgent::MinMaxAlphaBetaAgent(AbstractHeuristic *heuristic, int ms_constraint)
{
    this->_heuristic = heuristic;
    this->_ms_constraint = ms_constraint;
    this->_ms_turn_stop = ms_constraint * 0.95;
    this->_turn_start_clock = 0;
    this->_depth_reached = 0;
    this->_nodes_explored = 0;
}

string MinMaxAlphaBetaAgent::get_name()
{
    return Board::get_name() + ".MinMaxAlphaBetaAgent[" + to_string(this->_ms_constraint) + "ms]." +
           this->_heuristic->get_name();
}

vector<string> MinMaxAlphaBetaAgent::get_stats()
{
    vector<string> stats;

    stats.push_back("version=BbMmabPv-7.1.8");
    stats.push_back("depth=" + to_string(this->_depth_reached));
    stats.push_back("states=" + to_string(this->_nodes_explored));
    cerr << "BbMmabPv-7.1.8\t: stats=" << stats[0] << " " << stats[1] << " " << stats[2] << endl;
    return stats;
}

Move MinMaxAlphaBetaAgent::choose_from(Board *board, clock_t turn_start_clock)
{
    this->_turn_start_clock = turn_start_clock;

    vector<Move>   moves = board->get_available_moves();
    vector<float> *qualities = new vector<float>(moves.size());

    get_qualities(board, moves, qualities);

    auto comparator = board->is_white_turn()
                          ? std::max_element(qualities->begin(), qualities->end())
                          : std::min_element(qualities->begin(), qualities->end());

    size_t selectedIndex = std::distance(qualities->begin(), comparator);

    Move move = moves[selectedIndex];

    float dtime = elapsed_time(this->_turn_start_clock);
    if (dtime >= _ms_constraint)
        cerr << "MinMaxAlphaBetaAgent: TIMEOUT: dtime=" << dtime << "/" << this->_ms_constraint
             << "ms" << endl;

    return move;
}

void MinMaxAlphaBetaAgent::get_qualities(Board *board, vector<Move> moves, vector<float> *qualities)
{
    int max_depth = 2;
    this->_nodes_explored = 0;
    while (!this->is_time_up())
    {
        for (size_t i = 0; i < moves.size(); i++)
        {
            Board new_board = *board;
            new_board.apply_move(moves[i]);

            float move_quality = this->minmax(&new_board, max_depth, 1, -1, 1);

            if (this->is_time_up())
                break;

            qualities->at(i) = move_quality;
        }

        max_depth++;
    }

    this->_depth_reached = max_depth;
}

float MinMaxAlphaBetaAgent::minmax(Board *board, int max_depth, int depth, float alpha, float beta)
{
    this->_nodes_explored++;

    if (depth == max_depth || this->is_time_up() || board->get_game_state() != GAME_CONTINUE)
        return this->_heuristic->evaluate(board);

    vector<Move> moves = board->get_available_moves();

    float best_quality;
    if (board->is_white_turn())
    {
        best_quality = this->max_node(board, &moves, max_depth, depth, alpha, beta);
    }
    else
    {
        best_quality = this->min_node(board, &moves, max_depth, depth, alpha, beta);
    }

    return best_quality;
}

float MinMaxAlphaBetaAgent::max_node(
    Board *board, vector<Move> *moves, int max_depth, int depth, float alpha, float beta
)
{
    float best_quality = -1;
    for (Move move : *moves)
    {
        Board new_board = *board;
        new_board.apply_move(move);

        float child_quality = this->minmax(&new_board, max_depth, depth + 1, alpha, beta);

        if (this->is_time_up())
            break;

        best_quality = max(best_quality, child_quality);

        if (beta <= best_quality)
            return best_quality;

        alpha = max(alpha, best_quality);
    }

    return best_quality;
}

float MinMaxAlphaBetaAgent::min_node(
    Board *board, vector<Move> *moves, int max_depth, int depth, float alpha, float beta
)
{
    float best_quality = 1;
    for (Move move : *moves)
    {
        Board new_board = *board;
        new_board.apply_move(move);

        float child_quality = this->minmax(&new_board, max_depth, depth + 1, alpha, beta);

        if (this->is_time_up())
            break;

        best_quality = min(best_quality, child_quality);

        if (alpha >= best_quality)
            return best_quality;

        beta = min(beta, best_quality);
    }

    return best_quality;
}

bool MinMaxAlphaBetaAgent::is_time_up()
{
    return this->elapsed_time(this->_turn_start_clock) >= this->_ms_turn_stop;
}

float MinMaxAlphaBetaAgent::elapsed_time(clock_t clock_start)
{
    return (float)(clock() - clock_start) / CLOCKS_PER_SEC * 1000;
}

/*
        Content of 'srcs/heuristics/PiecesHeuristic.cpp'
*/

#include <algorithm>

PiecesHeuristic::PiecesHeuristic()
{
    for (int i = 0, eval = -EVALUATION_WINRATE_MAP_SIZE / 2; i < EVALUATION_WINRATE_MAP_SIZE;
         i++, eval++)
    {

        _evaluation_winrate_map[i] = 1.0 / (1.0 + std::exp(-0.003 * eval));
    }
}

float PiecesHeuristic::evaluate(Board *board)
{
    float state = board->get_game_state();
    if (state != GAME_CONTINUE)
    {
        if (state == BLACK_WIN)
            return 0;
        else if (state == DRAW)
            return 0.5;
        else
            return 1;
    }

    int white_material;
    int black_material;
    int material_evaluation = _material_evaluation(board, &white_material, &black_material);

    int white_material_in_bound = min(max(white_material, material_end_game), material_start_game);
    int black_material_in_bound = min(max(black_material, material_end_game), material_start_game);

    float white_eg_coefficient =
        (float)(material_start_game - white_material_in_bound) / material_start_end_game_diff;
    float black_eg_coefficient =
        (float)(material_start_game - black_material_in_bound) / material_start_end_game_diff;

    int pp_evaluation =
        _piece_positions_evaluation(board, white_eg_coefficient, black_eg_coefficient);

    int white_control_on_empty_cell_count =
        _count_bits(board->attacked_by_white_mask & board->empty_cells_mask);
    int white_control_on_enemy_cell_count =
        _count_bits(board->attacked_by_white_mask & board->black_pieces_mask);
    int white_control_on_ally_cell_count =
        _count_bits(board->attacked_by_white_mask & board->white_pieces_mask);

    int black_control_on_empty_cell_count =
        _count_bits(board->attacked_by_black_mask & board->empty_cells_mask);
    int black_control_on_enemy_cell_count =
        _count_bits(board->attacked_by_black_mask & board->white_pieces_mask);
    int black_control_on_ally_cell_count =
        _count_bits(board->attacked_by_black_mask & board->black_pieces_mask);

    int control_evaluation = (white_control_on_empty_cell_count - black_control_on_empty_cell_count
                             ) * control_value_for_empty_cell +
                             (white_control_on_enemy_cell_count - black_control_on_enemy_cell_count
                             ) * control_value_for_enemy_cell +
                             (white_control_on_ally_cell_count - black_control_on_ally_cell_count) *
                                 control_value_for_ally_cell;

    int evaluation = material_evaluation + pp_evaluation + control_evaluation;

    return _evaluation_winrate_map[evaluation + EVALUATION_WINRATE_MAP_SIZE / 2];
}

string PiecesHeuristic::get_name()
{
    return "PiecesHeuristic";
}

int PiecesHeuristic::_material_evaluation(Board *board, int *white_material, int *black_material)
{
    int white_pawn_count = _count_bits(board->white_pawns);
    int white_knight_count = _count_bits(board->white_knights);
    int white_bishop_count = _count_bits(board->white_bishops);
    int white_rook_count = _count_bits(board->white_rooks);
    int white_queen_count = _count_bits(board->white_queens);
    int black_pawn_count = _count_bits(board->black_pawns);
    int black_knight_count = _count_bits(board->black_knights);
    int black_bishop_count = _count_bits(board->black_bishops);
    int black_rook_count = _count_bits(board->black_rooks);
    int black_queen_count = _count_bits(board->black_queens);

    *white_material = white_pawn_count * PAWN_VALUE + white_knight_count * KNIGHT_VALUE +
                      white_bishop_count * BISHOP_VALUE + white_rook_count * ROOK_VALUE +
                      white_queen_count * QUEEN_VALUE;
    *black_material = black_pawn_count * PAWN_VALUE + black_knight_count * KNIGHT_VALUE +
                      black_bishop_count * BISHOP_VALUE + black_rook_count * ROOK_VALUE +
                      black_queen_count * QUEEN_VALUE;

    return *white_material - *black_material;
}

int PiecesHeuristic::_piece_positions_evaluation(
    Board *board, float white_eg_coefficient, float black_eg_coefficient
)
{
    int pp_eval = 0;

    pp_eval += _lookup_bonuses_for_all_pieces(
        white_pawn_sg_bonus_table, white_pawn_eg_bonus_table, white_eg_coefficient,
        board->white_pawns
    );
    pp_eval += _lookup_bonuses_for_all_pieces(white_knight_bonus_table, board->white_knights);
    pp_eval += _lookup_bonuses_for_all_pieces(white_bishop_bonus_table, board->white_bishops);
    pp_eval += _lookup_bonuses_for_all_pieces(white_rook_bonus_table, board->white_rooks);
    pp_eval += _lookup_bonuses_for_all_pieces(white_queen_bonus_table, board->white_queens);
    pp_eval += _lookup_bonuses_for_all_pieces(
        white_king_sg_bonus_table, white_king_eg_bonus_table, white_eg_coefficient,
        board->white_king
    );

    pp_eval -= _lookup_bonuses_for_all_pieces(
        black_pawn_sg_bonus_table, black_pawn_eg_bonus_table, black_eg_coefficient,
        board->black_pawns
    );
    pp_eval -= _lookup_bonuses_for_all_pieces(black_knight_bonus_table, board->black_knights);
    pp_eval -= _lookup_bonuses_for_all_pieces(black_bishop_bonus_table, board->black_bishops);
    pp_eval -= _lookup_bonuses_for_all_pieces(black_rook_bonus_table, board->black_rooks);
    pp_eval -= _lookup_bonuses_for_all_pieces(black_queen_bonus_table, board->black_queens);
    pp_eval -= _lookup_bonuses_for_all_pieces(
        black_king_sg_bonus_table, black_king_eg_bonus_table, black_eg_coefficient,
        board->black_king
    );

    return pp_eval;
}

int PiecesHeuristic::_lookup_bonuses_for_all_pieces(int *bonus_table, uint64_t bitboard)
{
    int bonuses = 0;

    while (bitboard)
    {
        int first_piece_i = _count_trailing_zeros(bitboard);

        bonuses += bonus_table[first_piece_i];

        bitboard ^= 1UL << first_piece_i;
    }

    return bonuses;
}

int PiecesHeuristic::_lookup_bonuses_for_all_pieces(
    int *sg_bonus_table, int *eg_bonus_table, float eg_coef, uint64_t bitboard
)
{
    int   bonuses = 0;
    float sg_coef = 1 - eg_coef;

    while (bitboard)
    {
        int first_piece_i = _count_trailing_zeros(bitboard);

        bonuses +=
            sg_bonus_table[first_piece_i] * sg_coef + eg_bonus_table[first_piece_i] * eg_coef;

        bitboard ^= 1UL << first_piece_i;
    }

    return bonuses;
}

/*
        Content of 'mains/main.cpp'
*/

using namespace std;

int main()
{
    GameEngine *game_engine = new GameEngine(new MinMaxAlphaBetaAgent(new PiecesHeuristic(), 50));
    game_engine->infinite_game_loop();
}
