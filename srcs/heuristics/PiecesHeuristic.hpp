#ifndef PIECESHEURISTIC_HPP
#define PIECESHEURISTIC_HPP

#include "AbstractHeuristic.hpp"

# define EVALUATION_WINRATE_MAP_SIZE 40000

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

        // From AlphaZero paper
        // https://en.wikipedia.org/wiki/Chess_piece_relative_value
        typedef enum t_piece_values
        {
            PAWN_VALUE = 100,
            KNIGHT_VALUE = 305,
            BISHOP_VALUE = 333,
            ROOK_VALUE = 563,
            QUEEN_VALUE = 950
        } e_piece_values;

        // Cells control
        const int control_value_for_empty_cell = 5;
        const int control_value_for_enemy_cell = 10;
        const int control_value_for_ally_cell = 15;

        // Piece positions - Completely arbitrary estimation (but fast)
        const int material_start_game =
            10 * PAWN_VALUE + 2 * KNIGHT_VALUE + 2 * BISHOP_VALUE + 2 * ROOK_VALUE + QUEEN_VALUE;
        const int material_end_game = QUEEN_VALUE + ROOK_VALUE + 3 * PAWN_VALUE;
        const int material_start_end_game_diff = material_start_game - material_end_game;

        // clang-format off
        // Pieces bonuses depending on their position
        // https://www.chessprogramming.org/Simplified_Evaluation_Function
        // https://github.com/amir650/BlackWidow-Chess/blob/master/src/com/chess/engine/classic/Alliance.java
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