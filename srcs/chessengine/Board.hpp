
#ifndef BOARD_HPP
#define BOARD_HPP

// --- BASIC BOARD IMPLEMENTATION---

#pragma region Board

#include "ChessEngine.hpp"
#include "Move.hpp"
#include "VisualBoard.hpp"

// Because of the Fifty-Move rule, a game cannot exceed 50 moves without a capture
// So we can assume that a position cannot be repeated at more than 50 moves away
#define FEN_HISTORY_SIZE 50

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

        // FEN data: Player turn
        bool white_turn;

        // FEN data: Castling info
        uint64_t white_castles; // Positions where the castle is available
        uint64_t black_castles;

        // FEN data: en passant and turns
        uint64_t en_passant; // En passant position is created after a pawn move of 2 squares. 0
                             // means no en passant available
        uint64_t next_turn_en_passant; // En passant position is created after a pawn move of 2
                                       // squares. 0 means no en passant available
        int half_turn_rule; // Number of half-turn since the last capture or pawn move (Fifty-Move
                            // rule)

    public:
        int game_turn; // Game turn, incremented after each black move

        // FEN data: Pieces
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

        Board(
            string _fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1",
            bool   chess960_rule = true,
            bool   codingame_rule = true
        );
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
        bool moves_computed;

    private:
#if USE_VISUAL_BOARD == 1
        VisualBoard visual_board;
#endif

        // Getters data
        bool         check_state;
        bool         double_check;
        bool         engine_data_updated;
        vector<Move> available_moves;
        float        game_state;
        bool         game_state_computed;

        // Engine variables
        uint64_t white_pieces_mask;     // All white pieces on the board
        uint64_t black_pieces_mask;     // All black pieces on the board
        uint64_t not_white_pieces_mask; // All cells that are not a white piece
        uint64_t not_black_pieces_mask; // All cells that are not a black piece

        uint64_t all_pieces_mask;  // All pieces on the board
        uint64_t empty_cells_mask; // All empty cells on the board

        uint64_t ally_king;
        uint64_t ally_pieces;

        uint64_t enemy_pawns;
        uint64_t enemy_knights;
        uint64_t enemy_bishops;
        uint64_t enemy_rooks;
        uint64_t enemy_queens;
        uint64_t enemy_king;
        uint64_t enemy_pieces;
        uint64_t enemy_pieces_sliding_diag; // Queens and Bishops
        uint64_t enemy_pieces_sliding_line; // Queens and Rooks

        uint64_t capturable_by_white_pawns_mask;
        uint64_t capturable_by_black_pawns_mask;

        uint64_t uncheck_mask;        // Full set of bits to 1 means there is no check
        uint64_t pawn_uncheck_mask;   // Uncheck mask only available for pawns
        uint64_t attacked_cells_mask; // Squares attacked by the opponent
        uint64_t pin_masks[64];       // Each cell can have a pinned mask

        // FEN history is used to check the Threefold Repetition rule
        // Each FEN is saved in the history after each move
        string fen_history[FEN_HISTORY_SIZE];
        int    fen_history_index;

        // // Function pointer to apply castle depending on the chess960 rule
        // bool    (Board::*_handle_castle)(int, int, int, int);

        // - Parsing -
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

        // - Accessibility / Getters -
        char _get_cell(uint64_t mask);
        void _create_fen_for_standard_castling(char *fen, int *fen_i);
        void _create_fen_for_chess960_castling(char *fen, int *fen_i);

        // - Move application -
        void _apply_regular_white_move(uint64_t src, uint64_t dst, uint64_t *piece_mask);
        void _apply_regular_black_move(uint64_t src, uint64_t dst, uint64_t *piece_mask);
        void _move_white_pawn(uint64_t src, uint64_t dst, char promotion);
        void _move_black_pawn(uint64_t src, uint64_t dst, char promotion);
        void _move_white_king(uint64_t src, uint64_t dst, castle_info_e castle_info);
        void _move_black_king(uint64_t src, uint64_t dst, castle_info_e castle_info);
        void _capture_white_pieces(uint64_t dst);
        void _capture_black_pieces(uint64_t dst);

        // - Engine updates -
        void _update_engine_at_turn_end();
        void _update_engine_at_turn_start();
        void _update_check_and_pins();
        void _update_pawn_check(int king_lkt_i);
        void _update_attacked_cells_mask();
        void _update_fen_history();

        // - Piece attacks -
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

        // - Move creation -
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

        void _add_regular_move_or_promotion(char piece, uint64_t src, uint64_t dst);
        void _create_piece_moves(char piece, uint64_t src, uint64_t legal_moves);

        // - Bit operations -

        void _apply_function_on_all_pieces(uint64_t bitboard, std::function<void(uint64_t)> func);
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

        // - End game -
        float _compute_game_state();
        bool  _threefold_repetition_rule();
        bool  _insufficient_material_rule();

        // STATIC LOOKUP TABLES

        static bool     lookup_tables_initialized;
        static uint64_t pawn_captures_lookup[64][2]; // 0: white, 1: black
        static uint64_t knight_lookup[64];
        static uint64_t sliding_lookup[64][8]; // 0: N, 1: NE, 2: E, 3: SE, 4: S, 5: SW, 6: W, 7: NW
        static uint64_t king_lookup[64];

        static void _initialize_lookup_tables();
        static void _create_pawn_captures_lookup_table(int y, int x, uint64_t position, int lkt_i);
        static void _create_knight_lookup_table(int y, int x, uint64_t position, int lkt_i);
        static void _create_sliding_lookup_table(int y, int x, uint64_t position, int lkt_i);
        static void _create_king_lookup_table(int y, int x, uint64_t position, int lkt_i);
};

#endif
