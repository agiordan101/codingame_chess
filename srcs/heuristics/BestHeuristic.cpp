#include "BestHeuristic.hpp"
#include <algorithm>

BestHeuristic::BestHeuristic()
{
    for (int i = 0, eval = -EVALUATION_WINRATE_MAP_SIZE / 2; i < EVALUATION_WINRATE_MAP_SIZE;
         i++, eval++)
    {
        // if (eval > 0)
        //     _evaluation_winrate_map[i] = 1 - 1.0 / (1 + eval);
        // else
        //     _evaluation_winrate_map[i] = -1 - 1.0 / (-1 + eval);

        // Sigmoid function (Between 0 and 1)
        _evaluation_winrate_map[i] = 1.0 / (1.0 + std::exp(-0.003 * eval));
    }
}

float BestHeuristic::evaluate(Board *board)
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

    t_unpinned_pieces _unpinned_bitboards;
    _find_unpinned_pieces(board, &_unpinned_bitboards);

    // --- Material evaluation ---
    int white_material;
    int black_material;
    int material_evaluation =
        _material_evaluation(&_unpinned_bitboards, &white_material, &black_material);

    // --- Piece position evaluation ---
    int white_material_in_bound = min(max(white_material, material_end_game), material_start_game);
    int black_material_in_bound = min(max(black_material, material_end_game), material_start_game);

    // Interpolate current material in start and end game interval, to a coefficient between 0 and 1
    // 0 = start game, 1 = end game
    float white_eg_coefficient =
        (float)(material_start_game - white_material_in_bound) / material_start_end_game_diff;
    float black_eg_coefficient =
        (float)(material_start_game - black_material_in_bound) / material_start_end_game_diff;

    int pp_evaluation = _piece_positions_evaluation(
        &_unpinned_bitboards, white_eg_coefficient, black_eg_coefficient
    );

    // --- Cells control evaluation ---
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

    // Return an evaluation between 0 and 1, where 0 is a win for black, 0.5 is a draw and 1 is a
    // win for white
    return _evaluation_winrate_map[evaluation + EVALUATION_WINRATE_MAP_SIZE / 2];
}

string BestHeuristic::get_name()
{
    return "BestHeuristic";
}

void BestHeuristic::_find_unpinned_pieces(Board *board, t_unpinned_pieces *_unpinned_bitboards)
{
    _unpinned_bitboards->white_pawns = _create_unpinned_bitboard(board, board->white_pawns);
    _unpinned_bitboards->white_knights = _create_unpinned_bitboard(board, board->white_knights);
    _unpinned_bitboards->white_bishops = _create_unpinned_bitboard(board, board->white_bishops);
    _unpinned_bitboards->white_rooks = _create_unpinned_bitboard(board, board->white_rooks);
    _unpinned_bitboards->white_queens = _create_unpinned_bitboard(board, board->white_queens);
    _unpinned_bitboards->white_king = board->white_king;
    _unpinned_bitboards->black_pawns = _create_unpinned_bitboard(board, board->black_pawns);
    _unpinned_bitboards->black_knights = _create_unpinned_bitboard(board, board->black_knights);
    _unpinned_bitboards->black_bishops = _create_unpinned_bitboard(board, board->black_bishops);
    _unpinned_bitboards->black_rooks = _create_unpinned_bitboard(board, board->black_rooks);
    _unpinned_bitboards->black_queens = _create_unpinned_bitboard(board, board->black_queens);
    _unpinned_bitboards->black_king = board->black_king;
}

uint64_t BestHeuristic::_create_unpinned_bitboard(Board *board, uint64_t bitboard)
{
    uint64_t unpinned_bitboard = bitboard;

    // Find all individual bits in bitboard
    while (bitboard)
    {
        int      first_piece_i = _count_trailing_zeros(bitboard);
        uint64_t lsb = 1UL << first_piece_i;

        // If a piece is pinned, remove it from the unpinned bitboard
        if (board->pin_masks[first_piece_i] != BITMASK_ALL_CELLS)
            unpinned_bitboard ^= lsb;

        // Remove the actual bit from bitboard, so we can find the next one
        bitboard ^= lsb;
    }

    return unpinned_bitboard;
}

int BestHeuristic::_material_evaluation(
    t_unpinned_pieces *_unpinned_bitboards, int *white_material, int *black_material
)
{
    int white_pawn_count = _count_bits(_unpinned_bitboards->white_pawns);
    int white_knight_count = _count_bits(_unpinned_bitboards->white_knights);
    int white_bishop_count = _count_bits(_unpinned_bitboards->white_bishops);
    int white_rook_count = _count_bits(_unpinned_bitboards->white_rooks);
    int white_queen_count = _count_bits(_unpinned_bitboards->white_queens);
    int black_pawn_count = _count_bits(_unpinned_bitboards->black_pawns);
    int black_knight_count = _count_bits(_unpinned_bitboards->black_knights);
    int black_bishop_count = _count_bits(_unpinned_bitboards->black_bishops);
    int black_rook_count = _count_bits(_unpinned_bitboards->black_rooks);
    int black_queen_count = _count_bits(_unpinned_bitboards->black_queens);

    // Material evaluation
    *white_material = white_pawn_count * PAWN_VALUE + white_knight_count * KNIGHT_VALUE +
                      white_bishop_count * BISHOP_VALUE + white_rook_count * ROOK_VALUE +
                      white_queen_count * QUEEN_VALUE;
    *black_material = black_pawn_count * PAWN_VALUE + black_knight_count * KNIGHT_VALUE +
                      black_bishop_count * BISHOP_VALUE + black_rook_count * ROOK_VALUE +
                      black_queen_count * QUEEN_VALUE;

    return *white_material - *black_material;
}

int BestHeuristic::_piece_positions_evaluation(
    t_unpinned_pieces *_unpinned_bitboards, float white_eg_coefficient, float black_eg_coefficient
)
{
    int pp_eval = 0;

    pp_eval += _lookup_bonuses_for_all_pieces(
        white_pawn_sg_bonus_table, white_pawn_eg_bonus_table, white_eg_coefficient,
        _unpinned_bitboards->white_pawns
    );
    pp_eval += _lookup_bonuses_for_all_pieces(
        white_knight_bonus_table, _unpinned_bitboards->white_knights
    );
    pp_eval += _lookup_bonuses_for_all_pieces(
        white_bishop_bonus_table, _unpinned_bitboards->white_bishops
    );
    pp_eval +=
        _lookup_bonuses_for_all_pieces(white_rook_bonus_table, _unpinned_bitboards->white_rooks);
    pp_eval +=
        _lookup_bonuses_for_all_pieces(white_queen_bonus_table, _unpinned_bitboards->white_queens);
    pp_eval += _lookup_bonuses_for_all_pieces(
        white_king_sg_bonus_table, white_king_eg_bonus_table, white_eg_coefficient,
        _unpinned_bitboards->white_king
    );

    pp_eval -= _lookup_bonuses_for_all_pieces(
        black_pawn_sg_bonus_table, black_pawn_eg_bonus_table, black_eg_coefficient,
        _unpinned_bitboards->black_pawns
    );
    pp_eval -= _lookup_bonuses_for_all_pieces(
        black_knight_bonus_table, _unpinned_bitboards->black_knights
    );
    pp_eval -= _lookup_bonuses_for_all_pieces(
        black_bishop_bonus_table, _unpinned_bitboards->black_bishops
    );
    pp_eval -=
        _lookup_bonuses_for_all_pieces(black_rook_bonus_table, _unpinned_bitboards->black_rooks);
    pp_eval -=
        _lookup_bonuses_for_all_pieces(black_queen_bonus_table, _unpinned_bitboards->black_queens);
    pp_eval -= _lookup_bonuses_for_all_pieces(
        black_king_sg_bonus_table, black_king_eg_bonus_table, black_eg_coefficient,
        _unpinned_bitboards->black_king
    );

    return pp_eval;
}

int BestHeuristic::_lookup_bonuses_for_all_pieces(int *bonus_table, uint64_t bitboard)
{
    int bonuses = 0;

    // Find all individual bits in bitboard
    while (bitboard)
    {
        int first_piece_i = _count_trailing_zeros(bitboard);

        // Lookup table for this piece
        bonuses += bonus_table[first_piece_i];

        // Remove the actual bit from bitboard, so we can find the next one
        bitboard ^= 1UL << first_piece_i;
    }

    return bonuses;
}

int BestHeuristic::_lookup_bonuses_for_all_pieces(
    int *sg_bonus_table, int *eg_bonus_table, float eg_coef, uint64_t bitboard
)
{
    int   bonuses = 0;
    float sg_coef = 1 - eg_coef;

    // Find all individual bits in bitboard
    while (bitboard)
    {
        int first_piece_i = _count_trailing_zeros(bitboard);

        // Lookup both start game and end game bonus tables, and mix them
        bonuses +=
            sg_bonus_table[first_piece_i] * sg_coef + eg_bonus_table[first_piece_i] * eg_coef;

        // Remove the actual bit from bitboard, so we can find the next one
        bitboard ^= 1UL << first_piece_i;
    }

    return bonuses;
}
