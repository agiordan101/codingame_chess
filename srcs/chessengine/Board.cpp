#include "Board.hpp"

# if BITBOARD_IMPLEMENTATION == 1

// --- PUBLIC METHODS ---

Board::Board(string _fen, bool _chess960_rule) {
    
    stringstream ss(_fen);
    string board;
    string color;
    string castling;
    string en_passant;
    string half_turn_rule;
    string game_turn;

    getline(ss, board, ' ');
    getline(ss, color, ' ');
    getline(ss, castling, ' ');
    getline(ss, en_passant, ' ');
    getline(ss, half_turn_rule, ' ');
    getline(ss, game_turn, ' ');

    _main_parsing(board, color, castling, en_passant, stoi(half_turn_rule), stoi(game_turn), _chess960_rule);
}

Board::Board(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _game_turn, bool _chess960_rule) {
    _main_parsing(_board, _color, _castling, _en_passant, _half_turn_rule, _game_turn, _chess960_rule);
}

void Board::log() {

    uint64_t rook;

    // Find all individual rooks in white_castles
    string white_castles_pos[2] = { "N/A", "N/A" };
    int white_castles_pos_i = 0;
    uint64_t castle_tmp = white_castles;
    while (castle_tmp)
    {
        rook = _get_least_significant_bit(castle_tmp);
        white_castles_pos[white_castles_pos_i++] = bitboard_to_algebraic(rook);

        // Remove the actual rook from castle_tmp, so we can find the next one
        castle_tmp ^= rook;
    }
    
    // Find all individual rooks in black_castles
    string black_castles_pos[2] = { "N/A", "N/A" };
    int black_castles_pos_i = 0;
    castle_tmp = black_castles;
    while (castle_tmp)
    {
        rook = _get_least_significant_bit(castle_tmp);
        black_castles_pos[black_castles_pos_i++] = bitboard_to_algebraic(rook);

        // Remove the actual rook from castle_tmp, so we can find the next one
        castle_tmp ^= rook;
    }

    cerr << "Board: FEN: " << create_fen() << endl;
    cerr << "Board: Turn: " << (white_turn ? "White" : "Black") << endl;
    cerr << "Board: White castling: " << white_castles_pos[0] << " " << white_castles_pos[1] << endl;
    cerr << "Board: Black castling: " << black_castles_pos[0] << " " << black_castles_pos[1] << endl;
    cerr << "Board: En passant: " << (en_passant ? bitboard_to_algebraic(en_passant) : "N/A") << endl;
    cerr << "Board: half_turn_rule: " << to_string(half_turn_rule) << endl;
    cerr << "Board: game_turn: " << to_string(game_turn) << endl;

    this->visual_board.printBoard();
}

void Board::log_history(int turns) {
}

void Board::apply_move(Move move)
{
    if (!this->engine_data_updated)
        _update_engine_at_turn_start();

    // TODO: Sort them by probability to optimize the if-else chain
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

        // Disable castling for this rook
        white_castles &= ~move.src;
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

        // Disable castling for this rook
        black_castles &= ~move.src;
    }
    else if (piece == 'q')
        _apply_regular_black_move(move.src, move.dst, &black_queens);
    else if (piece == 'k')
        _move_black_king(move.src, move.dst, move.castle_info);

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

    _update_engine_at_turn_end();
}

float Board::get_game_state()
{
    if (!this->game_state_computed)
    {
        if (!this->engine_data_updated)
            _update_engine_at_turn_start();

        this->game_state = GAME_CONTINUE;
    }

    return this->game_state;
}

bool Board::get_check_state()
{
    if (!this->engine_data_updated)
        _update_engine_at_turn_start();

    return this->check_state;
}

char Board::get_cell(int x, int y) {

    uint64_t pos_mask = 1UL << (y * 8 + x);
    
    return _get_cell(pos_mask);
}

int Board::get_castling_rights() {
    return 0;
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

string Board::create_fen(bool with_turns) {
    char fen[85];
    int fen_i = 0;

    bzero(fen, 85);

    // Write pieces
    int empty_cells_count = 0;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            // TODO: Don't use get_cell() but a mask we shift at each for loop beginning
            //          And create get_cell(uint64_t mask) method, used inside get_cell(int, int)
            if (get_cell(x, y) == EMPTY_CELL)
            {
                empty_cells_count++;
                continue ;
            }

            if (empty_cells_count > 0)
            {
                fen[fen_i++] = '0' + empty_cells_count;
                empty_cells_count = 0;
            }

            fen[fen_i++] = get_cell(x, y);
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

    // Write turn
    fen[fen_i++] = white_turn ? 'w' : 'b';
    fen[fen_i++] = ' ';

    // Write castling
    if (white_castles || black_castles)
    {
        if (chess960_rule)
            _create_fen_for_chess960_castling(fen, &fen_i);
        else
            _create_fen_for_standard_castling(fen, &fen_i);
    }
    else
        fen[fen_i++] = '-';
    fen[fen_i++] = ' ';

    // Write en passant
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

    // Write half turn rule
    fen_string += to_string(half_turn_rule);
    fen_string += string(" ");

    // Write game turn
    fen_string += to_string(game_turn);

    return fen_string;
}

Board *Board::clone()
{
    // TODO: Create a new constructor, taking an instance in param, which then copy all the needed data
    Board *cloned_board = new Board(create_fen(), chess960_rule);

    // Copy history
    for (int i = 0; i < FEN_HISTORY_SIZE; i++)
    {
        if (this->fen_history[i].empty())
            break ;
        cloned_board->fen_history[i] = this->fen_history[i];
    }

    return cloned_board;
}

// --- PRIVATE METHODS ---

// - Parsing -

void Board::_main_parsing(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _game_turn, bool _chess960_rule)
{
    this->visual_board = VisualBoard();

    // Initialize private variables
    chess960_rule = _chess960_rule;

    _initialize_bitboards();
    _create_lookup_tables();

    // Parse FEN data
    _parse_board(_board);
    white_turn = _color == "w";
    _parse_castling(_castling);
    en_passant = _en_passant != "-" ? algebraic_to_bitboard(_en_passant) : 0;
    next_turn_en_passant = 0UL;
    half_turn_rule = _half_turn_rule;
    game_turn = _game_turn;

    // Initialize internal data
    moves_computed = false;
    game_state_computed = false;
    engine_data_updated = false;

    fen_history_index = 0;
    for (int i = 0; i < FEN_HISTORY_SIZE; i++)
        fen_history[i] = string();
    _update_fen_history();
}

void Board::_initialize_bitboards() {
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
    
    check_state = false;
    double_check = false;
    uncheck_mask = 0xFFFFFFFFFFFFFFFF;
    attacked_cells_mask = 0xFFFFFFFFFFFFFFFF;
}

void Board::_parse_board(string fen_board) {

    char pos_index = 0;
    
    // Parse _board string into boards
    for (int i = 0; i < fen_board.length(); i++) {

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
}

void Board::_parse_castling(string castling_fen)
{
    // '-' means that no castling are available
    if (castling_fen == "-")
        return ;

    // Parse castling fen 'AHah' into 0707 for example
    for (int i = 0; i < castling_fen.length(); i++)
    {
        if (isupper(castling_fen[i]))
        {
            if (chess960_rule)
                white_castles |= algebraic_to_bitboard(string(1, castling_fen[i]) + '1');
            else
                white_castles |= castling_fen[i] == 'K' ? 0x8000000000000000UL : 0x0100000000000000UL;
        }
        else
        {
            if (chess960_rule)
                black_castles |= algebraic_to_bitboard(string(1, castling_fen[i]) + '8');
            else
                black_castles |= castling_fen[i] == 'k' ? 0b10000000UL : 0b00000001UL;
        }
    }
}

// - Accessibility / Getters -

char    Board::_get_cell(uint64_t mask)
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

void    Board::_create_fen_for_standard_castling(char *fen, int *fen_i)
{
    // Loop over the castling boards from the right to the left
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

void    Board::_create_fen_for_chess960_castling(char *fen, int *fen_i)
{
    // Loop over the castling boards from the left to the right
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

// - Move application -

void    Board::_apply_regular_white_move(uint64_t src, uint64_t dst, uint64_t *piece_mask)
{
    _capture_black_pieces(dst);

    // Remove the piece from its actual cell, and add the piece to the destination cell
    *piece_mask &= ~src;
    *piece_mask |= dst;
}

void    Board::_apply_regular_black_move(uint64_t src, uint64_t dst, uint64_t *piece_mask)
{
    _capture_white_pieces(dst);

    // Remove the piece from its actual cell, and add the piece to the destination cell
    *piece_mask &= ~src;
    *piece_mask |= dst;
}

void    Board::_move_white_pawn(uint64_t src, uint64_t dst, char promotion)
{
    // Fifty-Move rule: Reset half turn counter if a pawn is moved (-1 because it will be incremented at the end of the turn)
    half_turn_rule = -1;

    // If the pawn move is an en passant, capture the opponent pawn
    if (dst == en_passant)
        black_pawns &= (~en_passant) << 8;

    // If the pawn moves 2 squares, we generate an en-passant
    if ((src & 0x00FF000000000000UL) && (dst & 0x000000FF00000000UL))
        next_turn_en_passant = src >> 8;

    _capture_black_pieces(dst);

    // Remove the piece from its actual cell
    white_pawns &= ~src;
    
    // TODO: Sort them by probability to optimize the if-else chain
    // Add the piece to the destination cell
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

void    Board::_move_black_pawn(uint64_t src, uint64_t dst, char promotion)
{
    // Fifty-Move rule: Reset half turn counter if a pawn is moved (-1 because it will be incremented at the end of the turn)
    half_turn_rule = -1;

    // If the pawn move is an en passant, capture the opponent pawn
    if (dst == en_passant)
       white_pawns &= (~en_passant) >> 8;
    
    // If the pawn moves 2 squares, we generate an en-passant
    if ((src & 0x000000000000FF00UL) && (dst & 0x00000000FF000000UL))
        next_turn_en_passant = src << 8;

    _capture_white_pieces(dst);

    // Remove the piece from its actual cell
    black_pawns &= ~src;

    // TODO: Sort them by probability to optimize the if-else chain
    // Add the piece to the destination cell
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

void    Board::_move_white_king(uint64_t src, uint64_t dst, castle_info_e castle_info)
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

        // Remove the piece from its actual cell, and add the piece to the destination cell
        white_king = dst;
    }
    else if (castle_info == WHITELEFT)
    {
        white_rooks &= ~dst;
        white_king = 0x0400000000000000;
        white_rooks |= 0x0800000000000000;
    }
    else if (castle_info == WHITERIGHT)
    {
        white_rooks &= ~dst;
        white_king = 0x4000000000000000;
        white_rooks |= 0x2000000000000000;
    }

    white_castles = 0UL;
}

void    Board::_move_black_king(uint64_t src, uint64_t dst, castle_info_e castle_info)
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

        // Remove the piece from its actual cell, and add the piece to the destination cell
        black_king = dst;
    }
    else if (castle_info == BLACKLEFT)
    {
        black_rooks &= ~dst;
        black_king = 0x04;
        black_rooks |= 0x08;
    }
    else if (castle_info == BLACKRIGHT)
    {
        black_rooks &= ~dst;
        black_king = 0x40;
        black_rooks |= 0x20;
    }

    black_castles = 0UL;
}

void    Board::_capture_white_pieces(uint64_t dst)
{
    // Detect if a piece is captured
    if (all_pieces_mask & dst)
    {
        // Fifty-Move rule: Reset half turn counter if a piece is captured (-1 because it will be incremented at the end of the turn)
        half_turn_rule = -1;

        // Remove the captured piece
        uint64_t not_dst_mask = ~dst;
        white_pawns &= not_dst_mask;
        white_knights &= not_dst_mask;
        white_bishops &= not_dst_mask;
        white_rooks &= not_dst_mask;
        white_queens &= not_dst_mask;
    }
}

void    Board::_capture_black_pieces(uint64_t dst)
{
    // Detect if a piece is captured
    if (all_pieces_mask & dst)
    {
        // Fifty-Move rule: Reset half turn counter if a piece is captured (-1 because it will be incremented at the end of the turn)
        half_turn_rule = -1;

        // Remove the captured piece
        uint64_t not_dst_mask = ~dst;
        black_pawns &= not_dst_mask;
        black_knights &= not_dst_mask;
        black_bishops &= not_dst_mask;
        black_rooks &= not_dst_mask;
        black_queens &= not_dst_mask;
    }
}

// - Engine specific -

void Board::_update_engine_at_turn_start()
{
    white_pieces_mask = white_pawns | white_knights | white_bishops | white_rooks | white_queens | white_king;
    black_pieces_mask = black_pawns | black_knights | black_bishops | black_rooks | black_queens | black_king;
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

    _update_check_and_pins();
    _update_attacked_cells_mask();

    engine_data_updated = true;
}

void Board::_update_check_and_pins()
{
    // https://www.codeproject.com/Articles/5313417/Worlds-Fastest-board-Chess-Movegenerator
    //  - We Define a checkmask
    //  - We Define a pinmask
    // Sliding pieces attacks: https://www.chessprogramming.org/Classical_Approach
    if (ally_king == 0UL)
        return;

    uncheck_mask = 0UL;

    int king_lkt_i = _count_trailing_zeros(ally_king);
    int lkt_color = white_turn ? 0 : 1;

    // Compute pawn attacks
    uint64_t pawn_attacks = pawn_captures_lookup[king_lkt_i][lkt_color] & enemy_pawns;
    if (pawn_attacks)
    {
        check_state = true;
        uncheck_mask |= pawn_attacks;
    }

    // Compute knight attacks
    uint64_t knight_attacks = knight_lookup[king_lkt_i] & enemy_knights;
    if (knight_attacks)
    {
        check_state = true;
        uncheck_mask |= knight_attacks;
    }

    // Compute diagonale attacks
    _compute_sliding_piece_negative_ray_checks_and_pins(ally_king, NORTHEAST, enemy_pieces_sliding_diag);
    _compute_sliding_piece_positive_ray_checks_and_pins(ally_king, SOUTHEAST, enemy_pieces_sliding_diag);
    _compute_sliding_piece_positive_ray_checks_and_pins(ally_king, SOUTHWEST, enemy_pieces_sliding_diag);
    _compute_sliding_piece_negative_ray_checks_and_pins(ally_king, NORTHWEST, enemy_pieces_sliding_diag);

    // Compute line attacks
    _compute_sliding_piece_negative_ray_checks_and_pins(ally_king, NORTH, enemy_pieces_sliding_line);
    _compute_sliding_piece_positive_ray_checks_and_pins(ally_king, EAST, enemy_pieces_sliding_line);
    _compute_sliding_piece_positive_ray_checks_and_pins(ally_king, SOUTH, enemy_pieces_sliding_line);
    _compute_sliding_piece_negative_ray_checks_and_pins(ally_king, WEST, enemy_pieces_sliding_line);

    // If no checks found, then there are no move restrictions
    if (uncheck_mask == 0UL)
    {
        check_state = false;
        uncheck_mask = 0xFFFFFFFFFFFFFFFF;
    }

    this->visual_board.printSpecificBoard('C', uncheck_mask, "Uncheck mask");
}

void Board::_update_attacked_cells_mask()
{
    if (enemy_king == 0UL)
        return;

    attacked_cells_mask = 0UL;

    if (white_turn)
    {
        _apply_function_on_all_pieces(black_pawns, [this](uint64_t param) {
            _find_black_pawns_attacks(param);
        });
        _apply_function_on_all_pieces(black_knights, [this](uint64_t param) {
            _find_black_knights_attacks(param);
        });
        _apply_function_on_all_pieces(black_bishops, [this](uint64_t param) {
            _find_black_bishops_attacks(param);
        });
        _apply_function_on_all_pieces(black_rooks, [this](uint64_t param) {
            _find_black_rooks_attacks(param);
        });
        _apply_function_on_all_pieces(black_queens, [this](uint64_t param) {
            _find_black_queens_attacks(param);
        });
        _find_black_king_attacks();
    }
    else
    {
        _apply_function_on_all_pieces(white_pawns, [this](uint64_t param) {
            _find_white_pawns_attacks(param);
        });
        _apply_function_on_all_pieces(white_knights, [this](uint64_t param) {
            _find_white_knights_attacks(param);
        });
        _apply_function_on_all_pieces(white_bishops, [this](uint64_t param) {
            _find_white_bishops_attacks(param);
        });
        _apply_function_on_all_pieces(white_rooks, [this](uint64_t param) {
            _find_white_rooks_attacks(param);
        });
        _apply_function_on_all_pieces(white_queens, [this](uint64_t param) {
            _find_white_queens_attacks(param);
        });
        _find_white_king_attacks();
    }

    if (attacked_cells_mask == 0UL)
        attacked_cells_mask = 0xFFFFFFFFFFFFFFFF;

    this->visual_board.printSpecificBoard('A', attacked_cells_mask, "Attacked cells mask");
}

void Board::_update_engine_at_turn_end()
{
    check_state = false;
    double_check = false;

    moves_computed = false;
    game_state_computed = false;
    engine_data_updated = false;
    
    en_passant = next_turn_en_passant;
    next_turn_en_passant = 0UL;

    half_turn_rule += 1;

    // Only increment game turn after black turn
    if (!white_turn)
        game_turn += 1;
    white_turn = !white_turn;

    _update_fen_history();
}

void Board::_update_fen_history()
{
    // Removing half turn rule and game turn (For future Threefold rule comparisons)
    string fen = create_fen(false);

    // Loop the history
    if (fen_history_index == FEN_HISTORY_SIZE)
        fen_history_index = 0;

    fen_history[fen_history_index++] = fen;
}

// - Piece attacks -

void Board::_find_white_pawns_attacks(uint64_t src)
{
    int src_lkt_i = _count_trailing_zeros(src);
    attacked_cells_mask |= pawn_captures_lookup[src_lkt_i][0];
}

void Board::_find_white_knights_attacks(uint64_t src) {
    int src_lkt_i = _count_trailing_zeros(src);
    attacked_cells_mask |= knight_lookup[src_lkt_i];
}

void Board::_find_white_bishops_attacks(uint64_t src) {
    attacked_cells_mask |= _get_diagonal_rays(src, ally_king);
}

void Board::_find_white_rooks_attacks(uint64_t src) {
    attacked_cells_mask |= _get_line_rays(src, ally_king);
}

void Board::_find_white_queens_attacks(uint64_t src) {
    attacked_cells_mask |= _get_diagonal_rays(src, ally_king) | _get_line_rays(src, ally_king);
}

void Board::_find_white_king_attacks() {
    // TODO: This condition is only needed for testing purposes, when no king is on the board
    if (white_king)
    {
        int src_lkt_i = _count_trailing_zeros(white_king);
        attacked_cells_mask |= king_lookup[src_lkt_i];
    }
}

void Board::_find_black_pawns_attacks(uint64_t src) {
    int src_lkt_i = _count_trailing_zeros(src);
    attacked_cells_mask |= pawn_captures_lookup[src_lkt_i][1];
}

void Board::_find_black_knights_attacks(uint64_t src) {
    int src_lkt_i = _count_trailing_zeros(src);
    attacked_cells_mask |= knight_lookup[src_lkt_i];
}

void Board::_find_black_bishops_attacks(uint64_t src) {
    attacked_cells_mask |= _get_diagonal_rays(src, ally_king);
}

void Board::_find_black_rooks_attacks(uint64_t src) {
    attacked_cells_mask |= _get_line_rays(src, ally_king);
}

void Board::_find_black_queens_attacks(uint64_t src) {
    attacked_cells_mask |= _get_diagonal_rays(src, ally_king) | _get_line_rays(src, ally_king);
}

void Board::_find_black_king_attacks() {
    // TODO: This condition is only needed for testing purposes, when no king is on the board
    if (black_king)
    {
        int src_lkt_i = _count_trailing_zeros(black_king);
        attacked_cells_mask |= king_lookup[src_lkt_i];
    }
}

// - Move creation -

void Board::_find_moves()
{
    if (white_turn)
    {
        if (!double_check)
        {
            _apply_function_on_all_pieces(white_pawns, [this](uint64_t param) {
                _find_white_pawns_moves(param);
            });
            _apply_function_on_all_pieces(white_knights, [this](uint64_t param) {
                _find_white_knights_moves(param);
            });
            _apply_function_on_all_pieces(white_bishops, [this](uint64_t param) {
                _find_white_bishops_moves(param);
            });
            _apply_function_on_all_pieces(white_rooks, [this](uint64_t param) {
                _find_white_rooks_moves(param);
            });
            _apply_function_on_all_pieces(white_queens, [this](uint64_t param) {
                _find_white_queens_moves(param);
            });
            _apply_function_on_all_pieces(white_castles, [this](uint64_t param) {
                _find_white_castle_moves(param);
            });
        }
        _find_white_king_moves();
    }
    else
    {
        if (!double_check)
        {
            _apply_function_on_all_pieces(black_pawns, [this](uint64_t param) {
                _find_black_pawns_moves(param);
            });
            _apply_function_on_all_pieces(black_knights, [this](uint64_t param) {
                _find_black_knights_moves(param);
            });
            _apply_function_on_all_pieces(black_bishops, [this](uint64_t param) {
                _find_black_bishops_moves(param);
            });
            _apply_function_on_all_pieces(black_rooks, [this](uint64_t param) {
                _find_black_rooks_moves(param);
            });
            _apply_function_on_all_pieces(black_queens, [this](uint64_t param) {
                _find_black_queens_moves(param);
            });
            _apply_function_on_all_pieces(black_castles, [this](uint64_t param) {
                _find_black_castle_moves(param);
            });
        }
        _find_black_king_moves();
    }

    this->visual_board.printBoard();
    this->moves_computed = true;
}

void Board::_find_white_pawns_moves(uint64_t src)
{
    // Generate pawn moves: (pawn position shifted) & ~ColumnA & enemy_pieces & check_mask & pin_mask
    // TODO: Take care of checks and pins
    int src_lkt_i = _count_trailing_zeros(src);
    
    uint64_t capture_moves = pawn_captures_lookup[src_lkt_i][0] & capturable_by_white_pawns_mask;
    uint64_t advance_move = (src >> 8) & empty_cells_mask;
    uint64_t legal_moves = (capture_moves | advance_move) & uncheck_mask;

    // When pawn is on the 7th rank, it can move two squares forward
    // We just need to check if the squares in front of it are empty
    if (src & BITMASK_LINE_2 && (src >> 8) & empty_cells_mask)
        legal_moves |= (src >> 16) & empty_cells_mask;

    // Find all individual bits in legal_moves
    uint64_t dst;
    while (legal_moves)
    {
        dst = _get_least_significant_bit(legal_moves);
        _add_regular_move_or_promotion('P', src, dst);

        // Remove the actual bit from legal_moves, so we can find the next one
        legal_moves ^= dst;
    }
}

void Board::_find_white_knights_moves(uint64_t src)
{
    // Generate knight moves: (knight position shifted) & ~ally_pieces & check_mask & pin_mask
    // TODO: Take care of checks and pins
    int src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves = knight_lookup[src_lkt_i] & not_white_pieces_mask & uncheck_mask;

    _create_piece_moves('N', src, legal_moves);
}

void Board::_find_white_bishops_moves(uint64_t src) {
    // Generate bishop moves: (bishop position shifted) & ~ally_pieces & check_mask & pin_mask
    // TODO: Take care of checks and pins
    uint64_t legal_moves = not_white_pieces_mask & _get_diagonal_rays(src) & uncheck_mask;

    _create_piece_moves('B', src, legal_moves);
}

void Board::_find_white_rooks_moves(uint64_t src) {
    // Generate rook moves: (rook position shifted) & ~ally_pieces & check_mask & pin_mask
    // TODO: Take care of checks and pins
    uint64_t legal_moves = not_white_pieces_mask & _get_line_rays(src) & uncheck_mask;

    _create_piece_moves('R', src, legal_moves);
}

void Board::_find_white_queens_moves(uint64_t src) {
    // Generate queen moves: (queen position shifted) & ~ally_pieces & check_mask & pin_mask
    // TODO: Take care of checks and pins
    uint64_t legal_moves = not_white_pieces_mask & (_get_diagonal_rays(src) | _get_line_rays(src)) & uncheck_mask;

    _create_piece_moves('Q', src, legal_moves);
}

void Board::_find_white_king_moves() {
    // Generate king moves: (king position shifted) & ~ally_pieces & check_mask & pin_mask
    // TODO: Take care of checks and pins
    if (white_king)
    {
        int src_lkt_i = _count_trailing_zeros(white_king);
        uint64_t legal_moves = king_lookup[src_lkt_i] & not_white_pieces_mask & ~attacked_cells_mask;
        
        _create_piece_moves('K', white_king, legal_moves);
    }
}

void Board::_find_white_castle_moves(uint64_t dst) {
    // TODO: Take care of checks and pins
    // For castling. we need to know if the squares the king is passing through are attacked :
    // Pawns -> Just slide them
    // Knights -> Just slide them
    // King -> Just slide it
    // Sliders -> Loop over them, resolve x-rays using lookup tables
    _create_piece_moves('K', white_king, dst);
}

void Board::_find_black_pawns_moves(uint64_t src)
{
    // Generate pawn moves: (pawn position shifted) & ~ColumnA & enemy_pieces & check_mask & pin_mask
    // TODO: Take care of checks and pins
    int src_lkt_i = _count_trailing_zeros(src);

    uint64_t capture_moves = pawn_captures_lookup[src_lkt_i][1] & capturable_by_black_pawns_mask;
    uint64_t advance_move = (src << 8) & empty_cells_mask;
    uint64_t legal_moves = (capture_moves | advance_move) & uncheck_mask;

    // When pawn is on the 7th rank, it can move two squares forward
    // We just need to check if the squares in front of it are empty
    if (src & BITMASK_LINE_7 && (src << 8) & empty_cells_mask)
        legal_moves |= (src << 16) & empty_cells_mask;

    // Find all individual bits in legal_moves
    uint64_t dst;
    while (legal_moves)
    {
        dst = _get_least_significant_bit(legal_moves);
        _add_regular_move_or_promotion('p', src, dst);

        // Remove the actual bit from legal_moves, so we can find the next one
        legal_moves ^= dst;
    }
}

void Board::_find_black_knights_moves(uint64_t src)
{
    // Generate knight moves: (knight position shifted) & ~ally_pieces & check_mask & pin_mask
    // TODO: Take care of checks and pins
    int src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves = knight_lookup[src_lkt_i] & not_black_pieces_mask & uncheck_mask;

    _create_piece_moves('n', src, legal_moves);
}

void Board::_find_black_bishops_moves(uint64_t src) {
    // Generate bishop moves: (bishop position shifted) & ~ally_pieces & check_mask & pin_mask
    // TODO: Take care of checks and pins
    uint64_t legal_moves = not_black_pieces_mask & _get_diagonal_rays(src) & uncheck_mask;

    _create_piece_moves('b', src, legal_moves);
}

void Board::_find_black_rooks_moves(uint64_t src) {
    // Generate rook moves: (rook position shifted) & ~ally_pieces & check_mask & pin_mask
    // TODO: Take care of checks and pins
    uint64_t legal_moves = not_black_pieces_mask & _get_line_rays(src) & uncheck_mask;

    _create_piece_moves('r', src, legal_moves);
}

void Board::_find_black_queens_moves(uint64_t src) {
    // Generate queen moves: (queen position shifted) & ~ally_pieces & check_mask & pin_mask
    // TODO: Take care of checks and pins
    uint64_t legal_moves = not_black_pieces_mask & (_get_diagonal_rays(src) | _get_line_rays(src)) & uncheck_mask;

    _create_piece_moves('q', src, legal_moves);
}

void Board::_find_black_king_moves() {
    // Generate king moves: (king position shifted) & ~ally_pieces & check_mask & pin_mask
    // TODO: Take care of checks and pins
    if (black_king)
    {
        int src_lkt_i = _count_trailing_zeros(black_king);
        uint64_t legal_moves = king_lookup[src_lkt_i] & not_black_pieces_mask & ~attacked_cells_mask;
        
        _create_piece_moves('k', black_king, legal_moves);
    }
}

void Board::_find_black_castle_moves(uint64_t dst) {
    // TODO: Take care of checks and pins
    // For castling. we need to know if the squares the king is passing through are attacked :
    // Pawns -> Just slide them
    // Knights -> Just slide them
    // King -> Just slide it
    // Sliders -> Loop over them, resolve x-rays using lookup tables
    _create_piece_moves('k', black_king, dst);
}

void Board::_add_regular_move_or_promotion(char piece, uint64_t src, uint64_t dst)
{
    this->visual_board.updateBoard('o', dst);

    if (dst & BITMASK_LINE_81)
    {
        // Promotions (As UCI representation is always lowercase, finale piece case doesn't matter)
        this->available_moves.push_back(Move(piece, src, dst, 'n'));
        this->available_moves.push_back(Move(piece, src, dst, 'b'));
        this->available_moves.push_back(Move(piece, src, dst, 'r'));
        this->available_moves.push_back(Move(piece, src, dst, 'q'));
    }
    else
        this->available_moves.push_back(Move(piece, src, dst));
}

void Board::_create_piece_moves(char piece, uint64_t src, uint64_t legal_moves)
{
    this->visual_board.updateBoard('o', legal_moves);

    uint64_t dst;
    // Find all individual bits in legal_moves
    while (legal_moves)
    {
        dst = _get_least_significant_bit(legal_moves);
        this->available_moves.push_back(Move(piece, src, dst));

        // Remove the actual bit from legal_moves, so we can find the next one
        legal_moves ^= dst;
    }
}

// - Bit operations -

void        Board::_apply_function_on_all_pieces(uint64_t bitboard, std::function<void(uint64_t)> func)
{
    uint64_t piece;
    while (bitboard)
    {
        piece = _get_least_significant_bit(bitboard);
        func(piece);

        // Remove the actual bit from bitboard, so we can find the next one
        bitboard ^= piece;
    }
}

uint64_t    Board::_get_diagonal_rays(uint64_t src, uint64_t piece_to_ignore) {
    return\
        _compute_sliding_piece_positive_ray(src, SOUTHEAST, piece_to_ignore) |\
        _compute_sliding_piece_positive_ray(src, SOUTHWEST, piece_to_ignore) |\
        _compute_sliding_piece_negative_ray(src, NORTHWEST, piece_to_ignore) |\
        _compute_sliding_piece_negative_ray(src, NORTHEAST, piece_to_ignore);
}

uint64_t    Board::_get_line_rays(uint64_t src, uint64_t piece_to_ignore) {
    return\
       _compute_sliding_piece_positive_ray(src, EAST, piece_to_ignore) |\
       _compute_sliding_piece_positive_ray(src, SOUTH, piece_to_ignore) |\
       _compute_sliding_piece_negative_ray(src, WEST, piece_to_ignore) |\
       _compute_sliding_piece_negative_ray(src, NORTH, piece_to_ignore);
}

uint64_t    Board::_compute_sliding_piece_positive_ray(uint64_t src, ray_dir_e dir, uint64_t piece_to_ignore)
{
    int src_lkt_i = _count_trailing_zeros(src);
    uint64_t attacks = sliding_lookup[src_lkt_i][dir];

    // VisualBoard vb = this->visual_board.clone();
    // vb.updateBoard('*', sliding_lookup[src_lkt_i][dir]);
    // vb.printBoard();

    // Find all pieces in the ray (Ignoring ally king when searching attacked cells)
    uint64_t blockers = attacks & (all_pieces_mask ^ piece_to_ignore);
    if (blockers) {
        // Find the first blocker in the ray (the one with the smallest index)
        int src_lkt_i = _count_trailing_zeros(blockers);

        // vb = this->visual_board.clone();
        // vb.updateBoard('#', blockers);
        // vb.updateBoard('X', sliding_lookup[src_lkt_i][dir]);
        // vb.printBoard();

        // Remove all squares behind the blocker from the attacks
        attacks ^= sliding_lookup[src_lkt_i][dir];
    }

    // vb = this->visual_board.clone();
    // vb.updateBoard('X', attacks);
    // vb.printBoard();

    // Blocker should be in the ray, so ~color_pieces_mask removes it if it's an ally
    return attacks;
}

uint64_t    Board::_compute_sliding_piece_negative_ray(uint64_t src, ray_dir_e dir, uint64_t piece_to_ignore)
{
    int src_lkt_i = _count_trailing_zeros(src);
    uint64_t attacks = sliding_lookup[src_lkt_i][dir];

    // VisualBoard vb = this->visual_board.clone();
    // vb.updateBoard('*', sliding_lookup[src_lkt_i][dir]);
    // vb.printBoard();

    // Find all pieces in the ray (Ignoring ally king when searching attacked cells)
    uint64_t blockers = attacks & (all_pieces_mask ^ piece_to_ignore);
    if (blockers) {
        // Find the first blocker in the ray (the one with the biggest index)
        uint64_t blocker = _get_most_significant_bit(blockers);
        int src_lkt_i = _count_trailing_zeros(blocker);

        // vb = this->visual_board.clone();
        // vb.updateBoard('#', blockers);
        // vb.updateBoard('X', sliding_lookup[src_lkt_i][dir]);
        // vb.printBoard();

        // Remove all squares behind the blocker from the attacks
        attacks ^= sliding_lookup[src_lkt_i][dir];
    }

    // vb = this->visual_board.clone();
    // vb.updateBoard('X', attacks);
    // vb.printBoard();

    // Blocker should be in the ray, so ~color_pieces_mask removes it if it's an ally
    return attacks;
}

void        Board::_compute_sliding_piece_positive_ray_checks_and_pins(uint64_t king_pos, ray_dir_e dir, uint64_t potential_attacker)
{
    int king_lkt_i = _count_trailing_zeros(king_pos);
    uint64_t attacks = sliding_lookup[king_lkt_i][dir];

    // cerr << "King pos: " << std::bitset<64>(king_pos) << endl;
    // cerr << "King lkt i: " << king_lkt_i << endl;
    // this->visual_board.printSpecificBoard('K', king_pos);

    // VisualBoard vb = this->visual_board.clone();
    // vb.updateBoard('*', sliding_lookup[king_lkt_i][dir]);
    // vb.printBoard();

    // Find all pieces in the ray
    uint64_t blockers = attacks & all_pieces_mask;

    // if (dir == EAST)
    // {
    //     this->visual_board.printSpecificBoard('A', attacks, "_compute_sliding_piece_negative_ray_checks_and_pins: attacks");
    //     this->visual_board.printSpecificBoard('P', all_pieces_mask, "_compute_sliding_piece_negative_ray_checks_and_pins: all_pieces_mask");
    //     this->visual_board.printSpecificBoard('B', blockers, "_compute_sliding_piece_negative_ray_checks_and_pins: blockers");
    // }

    if (blockers)
    {
        // Find the first blocker in the ray (the one with the smallest index)
        uint64_t blocker = _get_least_significant_bit(blockers);
        int blocker_lkt_i = _count_trailing_zeros(blocker);

        // if (dir == EAST)
        // {
        //     this->visual_board.printSpecificBoard('b', blocker, "_compute_sliding_piece_negative_ray_checks_and_pins: blockers");
        //     this->visual_board.printSpecificBoard('p', potential_attacker, "_compute_sliding_piece_negative_ray_checks_and_pins: potential_attacker");
        //     this->visual_board.printSpecificBoard('A', blocker & potential_attacker, "_compute_sliding_piece_negative_ray_checks_and_pins: blocker & potential_attacker");
        // }

        // If the blocker is a potential attacker, there is check
        if (blocker & potential_attacker)
        {
            if (check_state)
               double_check = true;
            check_state = true;

            // if (dir == EAST)
            // {
            //     this->visual_board.printSpecificBoard('b', attacks, "_compute_sliding_piece_negative_ray_checks_and_pins: attackss");
            //     this->visual_board.printSpecificBoard('p', sliding_lookup[blocker_lkt_i][dir], "_compute_sliding_piece_negative_ray_checks_and_pins: sliding_lookup[blocker_lkt_i][dir]");
            //     this->visual_board.printSpecificBoard('A', attacks ^ sliding_lookup[blocker_lkt_i][dir], "_compute_sliding_piece_negative_ray_checks_and_pins: attacks ^ sliding_lookup[blocker_lkt_i][dir]");
            // }

            // Remove all squares behind the blocker
            uncheck_mask |= attacks ^ sliding_lookup[blocker_lkt_i][dir];
        }

        // If the blocker is an ally, it might be pinned
        if (blocker & ally_pieces)
        {
            // Remove this blocker from the mask, and look for further pieces in the ray
            blockers ^= blocker;
            if (blockers)
            {
                blocker = _get_least_significant_bit(blockers);

                // If the new blocker is a potential attacker, there is a pin
                if (blocker & potential_attacker)
                {
                    // Remove all squares behind the blocker
                    int blocker_lkt_i = _count_trailing_zeros(blocker);
                    uint64_t pin_ray = attacks ^ sliding_lookup[blocker_lkt_i][dir];

                    // TODO: Save the pinned piece and the total path of the pin
                    // How ?
                    // Buffer of tuples (piece_mask, pin_ray)
                }
            }
        }
    }
}

void        Board::_compute_sliding_piece_negative_ray_checks_and_pins(uint64_t king_pos, ray_dir_e dir, uint64_t potential_attacker)
{
    int king_lkt_i = _count_trailing_zeros(king_pos);
    uint64_t attacks = sliding_lookup[king_lkt_i][dir];

    // VisualBoard vb = this->visual_board.clone();
    // vb.updateBoard('*', sliding_lookup[king_lkt_i][dir]);
    // vb.printBoard();

    // Find all pieces in the ray
    uint64_t blockers = attacks & all_pieces_mask;

    if (blockers)
    {
        // Find the first blocker in the ray (the one with the smallest index)
        uint64_t blocker = _get_most_significant_bit(blockers);
        int blocker_lkt_i = _count_trailing_zeros(blocker);

        // If the blocker is a potential attacker, there is check
        if (blocker & potential_attacker)
        {
            if (check_state)
               double_check = true;
            check_state = true;

            // Remove all squares behind the blocker
            uncheck_mask |= attacks ^ sliding_lookup[blocker_lkt_i][dir];
        }

        // If the blocker is an ally, it might be pinned
        if (blocker & ally_pieces)
        {
            // Remove this blocker from the mask, and look for further pieces in the ray
            blockers ^= blocker;
            if (blockers)
            {
                blocker = _get_most_significant_bit(blockers);

                // If the new blocker is a potential attacker, there is a pin
                if (blocker & potential_attacker)
                {
                    // Remove all squares behind the blocker
                    int blocker_lkt_i = _count_trailing_zeros(blocker);
                    uint64_t pin_ray = attacks ^ sliding_lookup[blocker_lkt_i][dir];

                    // TODO: Save the pinned piece and the total path of the pin
                    // How ?
                    // Buffer of tuples (piece_mask, pin_ray)
                }
            }
        }
    }
}

// --- OPERATORS ---

bool Board::operator==(Board *test_board_abstracted) {
    // TODO: move this in the Board class, as inlined method
    return this->create_fen() == test_board_abstracted->create_fen();
}

// --- LOOKUP METHODS ---

void Board::_create_lookup_tables()
{
    memset(pawn_captures_lookup, 0, sizeof(uint64_t) * 64 * 2);
    memset(knight_lookup, 0, sizeof(uint64_t) * 64);
    memset(sliding_lookup, 0, sizeof(uint64_t) * 64 * 8);
    memset(king_lookup, 0, sizeof(uint64_t) * 64);

    int lkt_i = 0;
    uint64_t pos_mask = 1UL;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            // cerr << "_create_lookup_tables pos: " << std::bitset<64>(pos_mask) << endl;

            // Create the lookup table for the current position
            _create_pawn_captures_lookup_table(y, x, pos_mask, lkt_i);
            _create_knight_lookup_table(y, x, pos_mask, lkt_i);
            _create_sliding_lookup_table(y, x, pos_mask, lkt_i);
            _create_king_lookup_table(y, x, pos_mask, lkt_i);
            lkt_i++;
            pos_mask <<= 1;
        }
    }
}

void Board::_create_pawn_captures_lookup_table(int y, int x, uint64_t position, int lkt_i)
{
    if (0 < y && y < 7)
    {
        // - White pawns -
        uint64_t pawn_mask = 0UL;

        // Left capture
        if (x > 0)
            pawn_mask |= 1UL << ((y - 1) * 8 + (x - 1));
        // Right capture
        if (x < 7)
            pawn_mask |= 1UL << ((y - 1) * 8 + (x + 1));

        pawn_captures_lookup[lkt_i][0] = pawn_mask;

        // - Black pawns -
        pawn_mask = 0UL;

        // Left capture
        if (x > 0)
            pawn_mask |= 1UL << ((y + 1) * 8 + (x - 1));
        // Right capture
        if (x < 7)
            pawn_mask |= 1UL << ((y + 1) * 8 + (x + 1));

        pawn_captures_lookup[lkt_i][1] = pawn_mask;
    }
}

void Board::_create_knight_lookup_table(int y, int x, uint64_t position, int lkt_i)
{
    uint64_t knight_mask = 0UL;
    if (x > 0)
    {
        // 2 up 1 left
        if (y > 1)
            knight_mask |= 1UL << ((y - 2) * 8 + (x - 1));
        // 2 down 1 left
        if (y < 6)
            knight_mask |= 1UL << ((y + 2) * 8 + (x - 1));
    }
    if (x > 1)
    {
        // 2 left 1 up
        if (y > 0)
        knight_mask |= 1UL << ((y - 1) * 8 + (x - 2));
        // 2 left 1 down
        if (y < 7)
            knight_mask |= 1UL << ((y + 1) * 8 + (x - 2));
    }
    if (x < 6)
    {
        // 2 right 1 down
        if (y > 0)
        knight_mask |= 1UL << ((y - 1) * 8 + (x + 2));
        // 2 right 1 up
        if (y < 7)
            knight_mask |= 1UL << ((y + 1) * 8 + (x + 2));
    }
    if (x < 7)
    {
        // 2 up 1 right
        if (y > 1)
            knight_mask |= 1UL << ((y - 2) * 8 + (x + 1));
        // 2 down 1 right
        if (y < 6)
            knight_mask |= 1UL << ((y + 2) * 8 + (x + 1));
    }

    knight_lookup[lkt_i] = knight_mask;
}

void Board::_create_sliding_lookup_table(int y, int x, uint64_t position, int lkt_i)
{
    // North
    uint64_t sliding_pos = position;
    for (int j = y - 1; j >= 0; j--)
    {
        sliding_pos >>= 8;
        sliding_lookup[lkt_i][NORTH] |= sliding_pos;
    }

    // North-East
    sliding_pos = position;
    for (int j = y - 1, i = x + 1; j >= 0 && i < 8; j--, i++)
    {
        sliding_pos >>= 7;
        sliding_lookup[lkt_i][NORTHEAST] |= sliding_pos;
    }

    // East
    sliding_pos = position;
    for (int i = x + 1; i < 8; i++)
    {
        sliding_pos <<= 1;
        sliding_lookup[lkt_i][EAST] |= sliding_pos;
    }

    // South-East
    sliding_pos = position;
    for (int j = y + 1, i = x + 1; j < 8 && i < 8; j++, i++)
    {
        sliding_pos <<= 9;
        sliding_lookup[lkt_i][SOUTHEAST] |= sliding_pos;
    }

    // South
    sliding_pos = position;
    for (int j = y + 1; j < 8; j++)
    {
        sliding_pos <<= 8;
        sliding_lookup[lkt_i][SOUTH] |= sliding_pos;
    }

    // South-West
    sliding_pos = position;
    for (int j = y + 1, i = x - 1; j < 8 && i >= 0; j++, i--)
    {
        sliding_pos <<= 7;
        sliding_lookup[lkt_i][SOUTHWEST] |= sliding_pos;
    }

    // West
    sliding_pos = position;
    for (int i = x - 1; i >= 0; i--)
    {
        sliding_pos >>= 1;
        sliding_lookup[lkt_i][WEST] |= sliding_pos;
    }

    // North-West
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
        // 1 up left
        if (y > 0)
            king_mask |= 1UL << ((y - 1) * 8 + (x - 1));
        // 1 left
        king_mask |= 1UL << (y * 8 + (x - 1));
        // 1 down left
        if (y < 7)
            king_mask |= 1UL << ((y + 1) * 8 + (x - 1));
    }
    if (x < 7)
    {
        // 1 up right
        if (y > 0)
            king_mask |= 1UL << ((y - 1) * 8 + (x + 1));
        // 1 right
        king_mask |= 1UL << (y * 8 + (x + 1));
        // 1 down right
        if (y < 7)
            king_mask |= 1UL << ((y + 1) * 8 + (x + 1));
    }
    // 1 up
    if (y > 0)
        king_mask |= 1UL << ((y - 1) * 8 + x);
    // 1 down
    if (y < 7)
        king_mask |= 1UL << ((y + 1) * 8 + x);

    king_lookup[lkt_i] = king_mask;    
}

# else

// --- PUBLIC METHODS ---

Board::Board(string _fen, bool _chess960_rule) {
    
    stringstream ss(_fen);
    string board;
    string color;
    string castling;
    string en_passant;
    string half_turn_rule;
    string game_turn;

    getline(ss, board, ' ');
    getline(ss, color, ' ');
    getline(ss, castling, ' ');
    getline(ss, en_passant, ' ');
    getline(ss, half_turn_rule, ' ');
    getline(ss, game_turn, ' ');

    _main_parsing(board, color, castling, en_passant, stoi(half_turn_rule), stoi(game_turn), _chess960_rule);
}

Board::Board(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _game_turn, bool _chess960_rule) {
    _main_parsing(_board, _color, _castling, _en_passant, _half_turn_rule, _game_turn, _chess960_rule);
}

void Board::log() {
    std::locale::global(std::locale("C.UTF-8"));
    std::wcout.imbue(std::locale("C.UTF-8"));

    cerr << "Board: FEN: " << create_fen() << endl;
    cerr << "Board: Turn: " << (white_turn ? "White" : "Black") << endl;
    cerr << "Board: Castling: w " << castles[0] << " | w " << castles[1] << " | b " << castles[2] << " | b " << castles[3] << endl;
    cerr << "Board: Kings initial columns: w " << kings_initial_columns[0] << " - b " << kings_initial_columns[1] << endl;
    cerr << "Board: En passant: Available=" << en_passant_available << " Coordonates= " << en_passant_x << " " << en_passant_y << endl;
    cerr << "Board: half_turn_rule: " << to_string(half_turn_rule) << endl;
    cerr << "Board: game_turn: " << to_string(game_turn) << endl;

    cerr << "----+----------------" << endl;
    cerr << "yx  | 0 1 2 3 4 5 6 7" << endl;
    cerr << " uci| A B C D E F G H" << endl;
    cerr << "----+----------------" << endl;
    for (int y = 0; y < 8; y++)
    {
        wcout << y << " " << line_index_to_number(y) << " |";
        for (int x = 0; x < 8; x++)
            wcout << " " << convert_piece_to_unicode(board[y][x]);
        wcout << endl;
    }
    wcout << "----+----------------" << endl;
}

void Board::log_history(int turns)
{
    int min_turn = turns == -1 ? 0 : max(0, fen_history_index - turns);

    cerr << "Board: Current fen: " << this->create_fen() << endl;
    for (int i = fen_history_index - 1; i >= min_turn; i--)
        cerr << "Board: log_history: " << i << ": " << fen_history[i] << endl;
}

void Board::apply_move(Move move)
{
    // Disable en passant if it was available on this turn
    _update_en_passant();

    _apply_move(move.src_x, move.src_y, move.dst_x, move.dst_y, move.promotion);

    // Enable en passant if it was set in this turn
    _update_en_passant();

    // Only increment game turn after black turn
    if (!white_turn)
        game_turn += 1;

    white_turn = !white_turn;
    half_turn_rule += 1;

    // Update the engine
    check_computed = false;
    moves_computed = false;
    game_state_computed = false;

    _update_castling_rights();
    _update_fen_history();
}

float Board::get_game_state()
{
    if (!this->game_state_computed)
    {
        this->game_state = _find_game_state();
        this->game_state_computed = true;
    }

    return this->game_state;
}

bool Board::get_check_state()
{
    if (!this->check_computed)
    {
        this->check = _is_check();
        this->check_computed = true;
    }

    return this->check;
}

char Board::get_cell(int x, int y) {
    return this->board[y][x];
}

int Board::get_castling_rights() {
    return
        (this->castles[0] ? 1 : 0) +\
        (this->castles[1] ? 1 : 0) << 1 +\
        (this->castles[2] ? 1 : 0) << 2 +\
        (this->castles[3] ? 1 : 0) << 3;
}

bool Board::is_white_turn() {
    return this->white_turn;
}

vector<Move> Board::get_available_moves()
{
    if (!this->moves_computed)
    {
        _find_moves();
        this->moves_computed = true;
    }

    return this->available_moves;
}

string Board::create_fen(bool with_turns)
{
    char fen[85];
    int fen_i = 0;

    bzero(fen, 85);

    // Write pieces
    int empty_cells_count = 0;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (board[y][x] == EMPTY_CELL)
            {
                empty_cells_count++;
                continue ;
            }

            if (empty_cells_count > 0)
            {
                fen[fen_i++] = '0' + empty_cells_count;
                empty_cells_count = 0;
            }

            fen[fen_i++] = board[y][x];
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

    // Write turn
    fen[fen_i++] = white_turn ? 'w' : 'b';
    fen[fen_i++] = ' ';

    // Write castling
    if (castles[0] != -1 || castles[1] != -1 || castles[2] != -1 || castles[3] != -1)
    {
        if (chess960_rule)
        {
            if (castles[0] != -1)
                fen[fen_i++] = toupper(column_index_to_name(castles[0]));
            if (castles[1] != -1)
                fen[fen_i++] = toupper(column_index_to_name(castles[1]));
            if (castles[2] != -1)
                fen[fen_i++] = column_index_to_name(castles[2]);
            if (castles[3] != -1)
                fen[fen_i++] = column_index_to_name(castles[3]);
        }
        else
        {
            if (castles[0] != -1)
                fen[fen_i++] = castles[0] == 7 ? 'K' : 'Q';
            if (castles[1] != -1)
                fen[fen_i++] = castles[1] == 7 ? 'K' : 'Q';
            if (castles[2] != -1)
                fen[fen_i++] = castles[2] == 7 ? 'k' : 'q';
            if (castles[3] != -1)
                fen[fen_i++] = castles[3] == 7 ? 'k' : 'q';
        }
    }
    else
        fen[fen_i++] = '-';
    fen[fen_i++] = ' ';

    // Write en passant
    if (en_passant_available)
    {
        string en_passant = coord_to_algebraic(en_passant_x, en_passant_y);
        fen[fen_i++] = en_passant[0];
        fen[fen_i++] = en_passant[1];
    }
    else
        fen[fen_i++] = '-';
    fen[fen_i++] = ' ';

    string fen_string = string(fen, fen_i);

    if (!with_turns)
        return fen_string;

    // Write half turn rule
    fen_string += to_string(half_turn_rule);
    fen_string += string(" ");

    // Write game turn
    fen_string += to_string(game_turn);

    return fen_string;
}

Board *Board::clone()
{
    Board *cloned_board = new Board(create_fen(), chess960_rule);

    // Copy history
    for (int i = 0; i < FEN_HISTORY_SIZE; i++)
    {
        if (this->fen_history[i].empty())
            break ;
        cloned_board->fen_history[i] = this->fen_history[i];
    }

    return cloned_board;
}

// --- PRIVATE METHODS ---

void Board::_main_parsing(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _game_turn, bool _chess960_rule)
{
    // Set the right castling function pointer
    chess960_rule = _chess960_rule;
    _handle_castle = _chess960_rule ? &Board::_handle_chess960_castle : &Board::_handle_standard_castle;

    // Parse FEN data
    _parse_board(_board);
    white_turn = _color == "w";
    _parse_castling(_castling);
    _parse_en_passant(_en_passant);
    half_turn_rule = _half_turn_rule;
    game_turn = _game_turn;

    // Initialize private variables
    check_computed = false;
    moves_computed = false;
    game_state_computed = false;

    fen_history_index = 0;
    for (int i = 0; i < FEN_HISTORY_SIZE; i++)
        fen_history[i] = string();
    _update_fen_history();
}

void Board::_parse_board(string fen_board) {

    int cell_i = 0;

    memset(board, EMPTY_CELL, sizeof(char) * 64);
    for (int i = 0; i < fen_board.length(); i++) {

        char piece = fen_board[i];
        
        if (isdigit(piece))
            cell_i += atoi(&fen_board[i]);
        else if (piece != '/')
        {
            board[cell_i / 8][cell_i % 8] = piece;
            // board[cell_i / 8][cell_i % 8] = PIECE_letter_to_number(piece);
            // cerr << cell_i / 8 << " | " << cell_i % 8 << " -> " << (char)(piece) << " = " << board[cell_i / 8][cell_i % 8] << endl;
            cell_i++;
        }
    }
}

void Board::_parse_castling(string castling_fen)
{
    memset(castles, -1, sizeof(int) * 4);
    memset(kings_initial_columns, -1, sizeof(int) * 2);

    // '-' means that no castling are available
    if (castling_fen == "-")
        return ;

    // Parse castling fen 'AHah' into 0707 for example
    int white_castles_i = 0;
    int black_castles_i = 0;
    for (int i = 0; i < castling_fen.length(); i++)
    {
        if (isupper(castling_fen[i]))
        {
            if (chess960_rule)
                castles[white_castles_i++] = column_name_to_index(castling_fen[i]);
            else
                castles[white_castles_i++] = castling_fen[i] == 'K' ? 7 : 0;
        }
        else
        {
            if (chess960_rule)
                castles[2 + black_castles_i++] = column_name_to_index(castling_fen[i]);
            else
                castles[2 + black_castles_i++] = castling_fen[i] == 'k' ? 7 : 0;
        }
    }

    // Find kings initial column indexes
    for (int i = 0; i < 8; i++)
    {
        if (board[7][i] == 'K')
            kings_initial_columns[0] = i;
        if (board[0][i] == 'k')
            kings_initial_columns[1] = i;
    }
}

void Board::_parse_en_passant(string _en_passant)
{
    en_passant_available = _en_passant != "-";

    if (en_passant_available)
    {
        algebraic_to_coord(_en_passant, &en_passant_x, &en_passant_y);
    }
    else
    {
        en_passant_x = -1;
        en_passant_y = -1;
    }
}

void Board::_apply_move(int src_x, int src_y, int dst_x, int dst_y, char promotion) {

    // Castling
    if ((this->*_handle_castle)(src_x, src_y, dst_x, dst_y))
    {
        _apply_castle(src_x, src_y, dst_x, dst_y);
        return ;
    }
    
    if (promotion)
    {
        // Fifty-Move rule: Reset half turn counter if a pawn is moved (-1 because it will be incremented at the end of the turn)
        half_turn_rule = -1;

        // Promote the pawn : A valid piece must have been created in find_move
        board[src_y][src_x] = EMPTY_CELL;
        board[dst_y][dst_x] = white_turn ? toupper(promotion) : tolower(promotion);
        return ;
    }

    // Pawn moves
    if (tolower(board[src_y][src_x]) == 'p')
    {
        // Fifty-Move rule: Reset half turn counter if a pawn is moved (-1 because it will be incremented at the end of the turn)
        half_turn_rule = -1;

        // When a pawn go to an empty destination on another column, an opposant pawn has done an en passant
        if (abs(dst_x - src_x) == 1 && board[dst_y][dst_x] == EMPTY_CELL)
            board[src_y][dst_x] = EMPTY_CELL;

        // When a pawn jump two cells: Save coordinates where the opponent pawn could take it
        if (abs(dst_y - src_y) == 2)
        {
            en_passant_x = dst_x;
            en_passant_y = dst_y > src_y ? dst_y - 1 : dst_y + 1;
        }

        // Move the pawn
        board[dst_y][dst_x] = board[src_y][src_x];
        board[src_y][src_x] = EMPTY_CELL;
        return;
    }

    // Fifty-Move rule: Reset half turn counter if a piece is captured (-1 because it will be incremented at the end of the turn)
    if (board[dst_y][dst_x] != EMPTY_CELL)
        half_turn_rule = -1;

    // Regular move
    board[dst_y][dst_x] = board[src_y][src_x];
    board[src_y][src_x] = EMPTY_CELL;
}

bool Board::_handle_standard_castle(int src_x, int src_y, int dst_x, int dst_y)
{
    // In my implementation, castling moves are always represented by a king moving to its own rook, as Chess960 rules.
    // To detect a castle with standard rules, we just need to check if the king is moving by 2 cells on the x axis
    return (tolower(board[src_y][src_x]) == 'k' && abs(dst_x - src_x) >= 2);
}

bool Board::_handle_chess960_castle(int src_x, int src_y, int dst_x, int dst_y)
{
    // A castle with Chess960 rule is represented by moving the king to its own rook
    return 
        tolower(board[src_y][src_x]) == 'k' &&
        tolower(board[dst_y][dst_x]) == 'r' &&
        islower(board[src_y][src_x]) == islower(board[dst_y][dst_x])
    ;
}

void Board::_apply_castle(int src_x, int src_y, int dst_x, int dst_y)
{
    char king;
    char rook;
    if (white_turn)
    {
        king = 'K';
        rook = 'R';
        castles[0] = -1;
        castles[1] = -1;
    }
    else
    {
        king = 'k';
        rook = 'r';
        castles[2] = -1;
        castles[3] = -1;
    }

    // First, we remove both pieces
    board[src_y][src_x] = EMPTY_CELL;
    board[dst_y][dst_x] = EMPTY_CELL;

    // Then, we force THE ONLY valid castle final position
    if (dst_x < src_x)
    {
        // Left castle
        board[dst_y][2] = king;
        board[dst_y][3] = rook;
    }
    else
    {
        // Right castle
        board[dst_y][5] = rook;
        board[dst_y][6] = king;
    }
}

void Board::_update_en_passant()
{
    if (en_passant_x != -1)
    {
        if (en_passant_available)
        {
            // Disable en passant if it was available on this turn
            en_passant_available = false;
            en_passant_x = -1;
            en_passant_y = -1;
        }
        else
        {
            // Set the en passant available if coordonates were just set in this turn
            en_passant_available = true;
        }
    }
}

void Board::_update_castling_rights() {

    // Disable white castles if they are still available
    if (castles[0] != -1 || castles[1] != -1)
    {
        // Disable player castle if its king moves
        if (board[7][kings_initial_columns[0]] != 'K')
        {
            castles[0] = -1;
            castles[1] = -1;
        }

        // Disable side castles if the rook moves
        if (castles[0] != -1 && board[7][castles[0]] != 'R')
            castles[0] = -1;
        if (castles[1] != -1 && board[7][castles[1]] != 'R')
            castles[1] = -1;
    }

    // Disable black castles if they are still available
    if (castles[2] != -1 || castles[3] != -1)
    {
        // Disable player castle if its king moves
        if (board[0][kings_initial_columns[1]] != 'k')
        {
            castles[2] = -1;
            castles[3] = -1;
        }

        // Disable side castles if the rook moves
        if (castles[2] != -1 && board[0][castles[2]] != 'r')
            castles[2] = -1;
        if (castles[3] != -1 && board[0][castles[3]] != 'r')
            castles[3] = -1;
    }
}

void Board::_update_fen_history()
{
    // Remove half turn rule and game turn (For future Threefold rule comparisons)
    string fen = create_fen(false);

    // Loop the history
    if (fen_history_index == FEN_HISTORY_SIZE)
        fen_history_index = 0;

    fen_history[fen_history_index++] = fen;
}

float Board::_find_game_state()
{
    // Fifty-Move rule + Game turn limit + 2 other rules to detect a draw
    if (
        half_turn_rule >= 100 ||
        game_turn >= game_turn_max + 1 ||
        _threefold_repetition_rule() ||
        _insufficient_material_rule()
    )
        return DRAW;

    available_moves = get_available_moves();

    // If no moves are available, it's either a Checkmate or a Stalemate
    if (available_moves.size() == 0)
    {
        if (get_check_state())
            return white_turn ? BLACK_WIN : WHITE_WIN;
        return DRAW;
    }

    return GAME_CONTINUE;
}

bool Board::_threefold_repetition_rule()
{
    int actual_fen_index = fen_history_index - 1;
    string actual_fen = fen_history[actual_fen_index];

    // Check if the actual FEN is already 2 times in the history
    bool fen_found = false;
    int history_index = -1;
    while (++history_index < FEN_HISTORY_SIZE)
        if (history_index != actual_fen_index && fen_history[history_index] == actual_fen)
        {
            if (fen_found)
                return true;
            fen_found = true;
        }

    return false;
}

bool Board::_insufficient_material_rule()
{
    bool knight_found = false;
    bool bishop_found = false;
    int bishop_odd = false;

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            char piece = tolower(board[y][x]);

            // Skip empty cells
            if (piece == EMPTY_CELL)
                continue ;

            // Skip pieces
            if (piece == 'q' || piece == 'r' || piece == 'p')
                return false;
            
            if (piece == 'n')
            {
                // If there is more than one knight, it's not a material insufficient
                if (knight_found || bishop_found)
                    return false;

                knight_found = true;
            }
            else if (piece == 'b')
            {
                // If there is more than one bishop (Not on the same color), it's not a material insufficient
                if (knight_found ||
                    bishop_found && bishop_odd != (x + y) % 2)
                    return false;

                bishop_found = true;
                bishop_odd = (x + y) % 2; // Save the color of the first bishop. Even=0, Odd=1
            }
        }
    }

    return true;
}

vector<Move> Board::_find_moves() {

    char piece_letter;

    this->available_moves = vector<Move>();
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            piece_letter = board[y][x];

            // Skip empty cells
            if (piece_letter == EMPTY_CELL)
                continue ;

            // Only consider our own pieces
            if (white_turn)
            {
                if (islower(piece_letter))
                    continue;
            }
            else if (isupper(piece_letter))
                continue;

            switch (tolower(piece_letter))
            {
                case 'p':
                    _find_moves_pawns(x, y);
                    break;
                case 'n':
                    _find_moves_knights(x, y);
                    break;
                case 'b':
                    _find_moves_bishops(x, y);
                    break;
                case 'r':
                    _find_moves_rooks(x, y);
                    break;
                case 'k':
                    _find_moves_king(x, y);
                    break;
                case 'q':
                    _find_moves_queens(x, y);
                    break;
            }
        }
    }

    _filter_non_legal_moves();

    return this->available_moves;
}

void Board::_find_moves_pawns(int x, int y) {

    int dy;
    int dy2;
    int (*opp_case_test)(int);
    if (white_turn)
    {
        dy = y - 1;
        dy2 = y - 2;
        opp_case_test = static_cast<int(*)(int)>(islower);
    }
    else
    {
        dy = y + 1;
        dy2 = y + 2;
        opp_case_test = static_cast<int(*)(int)>(isupper);
    }

    // Move 1 cell
    if (board[dy][x] == EMPTY_CELL)
    {
        _add_regular_move_or_promotion(x, y, x, dy);

        // Move 2 cells
        if ((y == 1 || y == 6) && board[dy2][x] == EMPTY_CELL)
            this->available_moves.push_back(Move(x, y, x, dy2, 0));
    }

    // Capture left
    if (x > 0 && opp_case_test(board[dy][x - 1]) ||
            (en_passant_available && dy == en_passant_y && x - 1 == en_passant_x))
        _add_regular_move_or_promotion(x, y, x - 1, dy);
    
    // Capture right
    if (x < 7 && opp_case_test(board[dy][x + 1]) ||
            (en_passant_available && dy == en_passant_y && x + 1 == en_passant_x))
        _add_regular_move_or_promotion(x, y, x + 1, dy);
}

void Board::_add_regular_move_or_promotion(int x, int y, int dx, int dy)
{
    if (dy == 0 || dy == 7)
    {
        // Promotions
        this->available_moves.push_back(Move(x, y, dx, dy, 'N'));
        this->available_moves.push_back(Move(x, y, dx, dy, 'B'));
        this->available_moves.push_back(Move(x, y, dx, dy, 'R'));
        this->available_moves.push_back(Move(x, y, dx, dy, 'Q'));
    }
    else
        this->available_moves.push_back(Move(x, y, dx, dy, 0));
}

void Board::_find_moves_knights(int x, int y) {

    bool not_top_edge = y != 0;
    bool not_left_edge = x != 0;
    bool not_right_edge = x != 7;
    bool not_bottom_edge = y != 7;

    int (*opp_case_func)(int) = white_turn ? static_cast<int(*)(int)>(islower) : static_cast<int(*)(int)>(isupper);

    // 2 left 1 up
    if (not_top_edge && x > 1 && (board[y - 1][x - 2] == EMPTY_CELL || opp_case_func(board[y - 1][x - 2])))
        this->available_moves.push_back(Move(x, y, x - 2, y - 1, 0));
    // 2 left 1 down
    if (not_bottom_edge && x > 1 && (board[y + 1][x - 2] == EMPTY_CELL || opp_case_func(board[y + 1][x - 2])))
        this->available_moves.push_back(Move(x, y, x - 2, y + 1, 0));

    // 2 right 1 up
    if (not_top_edge && x < 6 && (board[y - 1][x + 2] == EMPTY_CELL || opp_case_func(board[y - 1][x + 2])))
        this->available_moves.push_back(Move(x, y, x + 2, y - 1, 0));
    // 2 right 1 down
    if (not_bottom_edge && x < 6 && (board[y + 1][x + 2] == EMPTY_CELL || opp_case_func(board[y + 1][x + 2])))
        this->available_moves.push_back(Move(x, y, x + 2, y + 1, 0));
    
    // 2 up 1 left
    if (not_left_edge && y > 1 && (board[y - 2][x - 1] == EMPTY_CELL || opp_case_func(board[y - 2][x - 1])))
        this->available_moves.push_back(Move(x, y, x - 1, y - 2, 0));
    // 2 up 1 right
    if (not_right_edge && y > 1 && (board[y - 2][x + 1] == EMPTY_CELL || opp_case_func(board[y - 2][x + 1])))
        this->available_moves.push_back(Move(x, y, x + 1, y - 2, 0));
    
    // 2 down 1 left
    if (not_left_edge && y < 6 && (board[y + 2][x - 1] == EMPTY_CELL || opp_case_func(board[y + 2][x - 1])))
        this->available_moves.push_back(Move(x, y, x - 1, y + 2, 0));
    // 2 down 1 right
    if (not_right_edge && y < 6 && (board[y + 2][x + 1] == EMPTY_CELL || opp_case_func(board[y + 2][x + 1])))
        this->available_moves.push_back(Move(x, y, x + 1, y + 2, 0));
}

void Board::_find_moves_bishops(int x, int y) {
    
    int (*opp_case_func)(int) = white_turn ? static_cast<int(*)(int)>(islower) : static_cast<int(*)(int)>(isupper);

    // Search diagonally up left
    for (int bx = x - 1, by = y - 1; bx >= 0 && by >= 0; bx--, by--)
    {
        if (board[by][bx] == EMPTY_CELL)
            this->available_moves.push_back(Move(x, y, bx, by, 0));
        else
        {
            if (opp_case_func(board[by][bx]))
                this->available_moves.push_back(Move(x, y, bx, by, 0));
            break;
        }
    }

    // Search diagonally down right
    for (int bx = x + 1, by = y + 1; bx < 8 && by < 8; bx++, by++)
    {
        if (board[by][bx] == EMPTY_CELL)
            this->available_moves.push_back(Move(x, y, bx, by, 0));
        else
        {
            if (opp_case_func(board[by][bx]))
                this->available_moves.push_back(Move(x, y, bx, by, 0));
            break;
        }
    }

    // Search diagonally down left
    for (int bx = x - 1, by = y + 1; bx >= 0 && by < 8; bx--, by++)
    {
        if (board[by][bx] == EMPTY_CELL)
            this->available_moves.push_back(Move(x, y, bx, by, 0));
        else
        {
            if (opp_case_func(board[by][bx]))
                this->available_moves.push_back(Move(x, y, bx, by, 0));
            break;
        }
    }

    // Search diagonally up right
    for (int bx = x + 1, by = y - 1; bx < 8 && by >= 0; bx++, by--)
    {
        if (board[by][bx] == EMPTY_CELL)
            this->available_moves.push_back(Move(x, y, bx, by, 0));
        else
        {
            if (opp_case_func(board[by][bx]))
                this->available_moves.push_back(Move(x, y, bx, by, 0));
            break;
        }
    }
}

void Board::_find_moves_rooks(int x, int y) {
    
    int (*opp_case_func)(int) = white_turn ? static_cast<int(*)(int)>(islower) : static_cast<int(*)(int)>(isupper);

    // Search vertically up
    for (int by = y - 1; by >= 0; by--)
    {
        if (board[by][x] == EMPTY_CELL)
            this->available_moves.push_back(Move(x, y, x, by, 0));
        else
        {
            if (opp_case_func(board[by][x]))
                this->available_moves.push_back(Move(x, y, x, by, 0));
            break;
        }
    }

    // Search vertically down
    for (int by = y + 1; by < 8; by++)
    {
        if (board[by][x] == EMPTY_CELL)
            this->available_moves.push_back(Move(x, y, x, by, 0));
        else
        {
            if (opp_case_func(board[by][x]))
                this->available_moves.push_back(Move(x, y, x, by, 0));
            break;
        }
    }

    // Search horizontally left
    for (int bx = x - 1; bx >= 0; bx--)
    {
        if (board[y][bx] == EMPTY_CELL)
            this->available_moves.push_back(Move(x, y, bx, y, 0));
        else
        {
            if (opp_case_func(board[y][bx]))
                this->available_moves.push_back(Move(x, y, bx, y, 0));
            break;
        }
    }

    // Search horizontally right
    for (int bx = x + 1; bx < 8; bx++)
    {
        if (board[y][bx] == EMPTY_CELL)
            this->available_moves.push_back(Move(x, y, bx, y, 0));
        else
        {
            if (opp_case_func(board[y][bx]))
                this->available_moves.push_back(Move(x, y, bx, y, 0));
            break;
        }
    }
}

void Board::_find_moves_queens(int x, int y) {
    _find_moves_rooks(x, y);
    _find_moves_bishops(x, y);
}

void Board::_find_moves_king(int x, int y) {
    
    int (*opp_case_func)(int) = white_turn ? static_cast<int(*)(int)>(islower) : static_cast<int(*)(int)>(isupper);

    bool king_left_edge = x == 0;
    bool king_right_edge = x == 7;
    bool king_top_edge = y == 0;
    bool king_bottom_edge = y == 7;
    
    // Search diagonally up left
    if (!king_top_edge && !king_left_edge && (board[y - 1][x - 1] == EMPTY_CELL || opp_case_func(board[y - 1][x - 1])))
        this->available_moves.push_back(Move(x, y, x - 1, y - 1, 0));
    // Search vertically up
    if (!king_top_edge && (board[y - 1][x] == EMPTY_CELL || opp_case_func(board[y - 1][x])))
        this->available_moves.push_back(Move(x, y, x, y - 1, 0));    
    // Search diagonally up right
    if (!king_top_edge && !king_right_edge && (board[y - 1][x + 1] == EMPTY_CELL || opp_case_func(board[y - 1][x + 1])))
        this->available_moves.push_back(Move(x, y, x + 1, y - 1, 0));
    
    // Search horizontally left
    if (!king_left_edge && (board[y][x - 1] == EMPTY_CELL || opp_case_func(board[y][x - 1])))
        this->available_moves.push_back(Move(x, y, x - 1, y, 0));
    // Search horizontally right
    if (!king_right_edge && (board[y][x + 1] == EMPTY_CELL || opp_case_func(board[y][x + 1])))
        this->available_moves.push_back(Move(x, y, x + 1, y, 0));
    
    // Search diagonally down left
    if (!king_bottom_edge && !king_left_edge && (board[y + 1][x - 1] == EMPTY_CELL || opp_case_func(board[y + 1][x - 1])))
        this->available_moves.push_back(Move(x, y, x - 1, y + 1, 0));
    // Search vertically down
    if (!king_bottom_edge && (board[y + 1][x] == EMPTY_CELL || opp_case_func(board[y + 1][x])))
        this->available_moves.push_back(Move(x, y, x, y + 1, 0));
    // Search diagonally down right
    if (!king_bottom_edge && !king_right_edge && (board[y + 1][x + 1] == EMPTY_CELL || opp_case_func(board[y + 1][x + 1])))
        this->available_moves.push_back(Move(x, y, x + 1, y + 1, 0));

    // Castling
    for (int i = 0; i < 2; i++)
    {
        int castle_index = white_turn ? castles[i] : castles[2 + i];
        if (castle_index != -1)
            _find_moves_castle(x, y, castle_index);
    }
}

void Board::_find_moves_castle(int king_x, int king_y, int rook_x)
{
    int trajectory_dx;
    int trajectory_min_x;
    int trajectory_max_x;

    // Find trajectories boundaries
    if (rook_x < king_x)
    {
        trajectory_dx = -1;
        trajectory_min_x = min(rook_x, 2);
        trajectory_max_x = max(king_x, 3);
    }
    else
    {
        trajectory_dx = 1;
        trajectory_min_x = min(king_x, 5);
        trajectory_max_x = max(rook_x, 6);
    }

    // Assert that only castling pieces are on the trajectories
    int trajectory_x = trajectory_min_x;
    while (trajectory_x <= trajectory_max_x)
    {
        if (board[king_y][trajectory_x] != EMPTY_CELL &&
            board[king_y][trajectory_x] != (white_turn ? 'K' : 'k') &&
            trajectory_x != rook_x)
            return ;
        trajectory_x++;
    }

    // The king trajectory must not being in check
    if (_is_castle_legal(king_x, king_y, trajectory_dx < 0 ? 2 : 6, trajectory_dx))
        this->available_moves.push_back(Move(king_x, king_y, rook_x, king_y, 0));
}

bool Board::_is_castle_legal(int src_x, int src_y, int dst_x, int trajectory_dx)
{
    char king_piece = white_turn ? 'K' : 'k';
    
    // Put kings all over the trajectory to check if they are in check
    int x = src_x;
    while (x != dst_x + trajectory_dx)
    {
        // Save the current piece
        char piece = board[src_y][x];

        // Put a king to detect a check
        board[src_y][x] = king_piece;
        bool is_check = _is_check(x, src_y);

        board[src_y][x] = piece;

        if (is_check)
            return false;
        x += trajectory_dx;
    }

    return true;
}

void Board::_filter_non_legal_moves()
{
    Board *test_board;

    vector<Move>::iterator it = this->available_moves.begin();
    while (it < this->available_moves.end())
    {
        test_board = this->clone();
        test_board->_apply_move(it->src_x, it->src_y, it->dst_x, it->dst_y, it->promotion);

        // If the king is in check, remove the move from the available moves
        if (test_board->_is_check())
        {
            // The iterator will be incremented by the erase method
            this->available_moves.erase(it);
        }
        else
            it++;

        delete test_board;
    }
}

bool Board::_is_check()
{
    char king_piece = white_turn ? 'K' : 'k';

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (board[y][x] == king_piece)
                return _is_check(x, y);
    return false;
}

bool Board::_is_check(int king_x, int king_y)
{
    char opp_king = white_turn ? 'k' : 'K';
    char opp_queen = white_turn ? 'q' : 'Q';
    char opp_rook = white_turn ? 'r' : 'R';
    char opp_bishop = white_turn ? 'b' : 'B';
    char opp_knight = white_turn ? 'n' : 'N';
    char opp_pawn = white_turn ? 'p' : 'P';

    bool king_left_edge = king_x == 0;
    bool king_right_edge = king_x == 7;
    bool king_top_edge = king_y == 0;
    bool king_bottom_edge = king_y == 7;


    // Search vertically down
    for (int y = king_y + 1; y < 8; y++)
        if (board[y][king_x] != EMPTY_CELL)
        {
            if (board[y][king_x] == opp_queen || board[y][king_x] == opp_rook)
                return true;
            break;
        }
    // Search vertically up
    for (int y = king_y - 1; y >= 0; y--)
        if (board[y][king_x] != EMPTY_CELL)
        {
            if (board[y][king_x] == opp_queen || board[y][king_x] == opp_rook)
                return true;
            break;
        }

    // Search horizontally right
    for (int x = king_x + 1; x < 8; x++)
        if (board[king_y][x] != EMPTY_CELL)
        {
            if (board[king_y][x] == opp_queen || board[king_y][x] == opp_rook)
                return true;
            break;
        }
    // Search horizontally left
    for (int x = king_x - 1; x >= 0; x--)
        if (board[king_y][x] != EMPTY_CELL)
        {
            if (board[king_y][x] == opp_queen || board[king_y][x] == opp_rook)
                return true;
            break;
        }

    // Search diagonally up left
    for (int x = king_x - 1, y = king_y - 1; x >= 0 && y >= 0; x--, y--)
        if (board[y][x] != EMPTY_CELL)
        {
            if (board[y][x] == opp_queen || board[y][x] == opp_bishop)
                return true;
            break;
        }
    // Search diagonally down right
    for (int x = king_x + 1, y = king_y + 1; x < 8 && y < 8; x++, y++)
        if (board[y][x] != EMPTY_CELL)
        {
            if (board[y][x] == opp_queen || board[y][x] == opp_bishop)
                return true;
            break;
        }

    // Search diagonally down left
    for (int x = king_x - 1, y = king_y + 1; x >= 0 && y < 8; x--, y++)
        if (board[y][x] != EMPTY_CELL)
        {
            if (board[y][x] == opp_queen || board[y][x] == opp_bishop)
                return true;
            break;
        }
    // Search diagonally up right
    for (int x = king_x + 1, y = king_y - 1; x < 8 && y >= 0; x++, y--)
        if (board[y][x] != EMPTY_CELL)
        {
            if (board[y][x] == opp_queen || board[y][x] == opp_bishop)
                return true;
            break;
        }
    
    // Search knight - 2 left 1 up
    if (!king_top_edge && king_x > 1 && board[king_y - 1][king_x - 2] == opp_knight)
        return true;
    // Search knight - 2 left 1 down
    if (!king_bottom_edge && king_x > 1 && board[king_y + 1][king_x - 2] == opp_knight)
        return true;

    // Search knight - 2 right 1 up
    if (!king_top_edge && king_x < 6 && board[king_y - 1][king_x + 2] == opp_knight)
        return true;
    // Search knight - 2 right 1 down
    if (!king_bottom_edge && king_x < 6 && board[king_y + 1][king_x + 2] == opp_knight)
        return true;
    
    // Search knight - 2 up 1 left
    if (king_y > 1 && !king_left_edge && board[king_y - 2][king_x - 1] == opp_knight)
        return true;
    // Search knight - 2 up 1 right
    if (king_y > 1 && !king_right_edge && board[king_y - 2][king_x + 1] == opp_knight)
        return true;

    // Search knight - 2 down 1 left
    if (king_y < 6 && !king_left_edge && board[king_y + 2][king_x - 1] == opp_knight)
        return true;
    // Search knight - 2 down 1 right
    if (king_y < 6 && !king_right_edge && board[king_y + 2][king_x + 1] == opp_knight)
        return true;

    if (white_turn)
    {
        // Search pawn - 1 up left
        if (!king_top_edge && !king_left_edge && board[king_y - 1][king_x - 1] == opp_pawn)
            return true;
        // Search pawn - 1 up right
        if (!king_top_edge && !king_right_edge && board[king_y - 1][king_x + 1] == opp_pawn)
            return true;
    }
    else
    {
        // Search pawn - 1 down left
        if (!king_bottom_edge && !king_left_edge && board[king_y + 1][king_x - 1] == opp_pawn)
            return true;
        // Search pawn - 1 down right
        if (!king_bottom_edge && !king_right_edge && board[king_y + 1][king_x + 1] == opp_pawn)
            return true;
    }

    if (!king_left_edge)
    {
        // Search king - left up
        if (!king_top_edge && board[king_y - 1][king_x - 1] == opp_king)
            return true;
        // Search king - left middle
        if (board[king_y][king_x - 1] == opp_king)
            return true;
        // Search king - left down
        if (!king_bottom_edge && board[king_y + 1][king_x - 1] == opp_king)
            return true;
    }

    // Search king - middle up
    if (!king_top_edge && board[king_y - 1][king_x] == opp_king)
        return true;
    // Search king - middle down
    if (!king_bottom_edge && board[king_y + 1][king_x] == opp_king)
        return true;
    
    if (!king_right_edge)
    {
        // Search king - right up
        if (!king_top_edge && board[king_y - 1][king_x + 1] == opp_king)
            return true;
        // Search king - right middle
        if (board[king_y][king_x + 1] == opp_king)
            return true;
        // Search king - right down
        if (!king_bottom_edge && board[king_y + 1][king_x + 1] == opp_king)
            return true;
    }

    return false;
}

// --- OPERATORS ---

bool    Board::operator ==(Board *test_board) {

    // Assert that the tested board is a Board instance
    if (!test_board) {
        cerr << "Board: operator==: The tested board is not a Board instance" << endl;
        return false;
    }

    // Test equalities on all FEN data

    // Board pieces equality
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (this->board[y][x] != test_board->board[y][x])
            {
                cerr << "Board: operator==: Board pieces (x y = " << x << " " << y << "): " << this->board[y][x] << " != " << test_board->board[y][x] << endl;
                return false;
            }

    // Turn equality
    if (this->white_turn != test_board->white_turn)
    {
        cerr << "Board: operator==: White turn: " << this->white_turn << " != " << test_board->white_turn << endl;
        return false;
    }
    
    // Catling rights equality
    for (int i = 0; i < 4; i += 2)
    {
        // Could be at the same place
        // or the other one
        if (this->castles[i] >= 0 &&
            this->castles[i] != test_board->castles[i] &&
            this->castles[i] != test_board->castles[i + 1])
        {
            cerr << "Board: operator==: Castling rights: " << this->castles[i] << " != " << test_board->castles[i] << " && " << this->castles[i] << " != " << test_board->castles[i + 1] << endl;
            return false;
        }

        if (this->castles[i + 1] >= 0 &&
            this->castles[i + 1] != test_board->castles[i] &&
            this->castles[i + 1] != test_board->castles[i + 1])
        {
            cerr << "Board: operator==: Castling rights: " << this->castles[i + 1] << " != " << test_board->castles[i] << " && " << this->castles[i + 1] << " != " << test_board->castles[i + 1] << endl;
            return false;
        }
    }

    // En passant equality
    if (this->en_passant_available != test_board->en_passant_available ||
        this->en_passant_x != test_board->en_passant_x ||
        this->en_passant_y != test_board->en_passant_y)
    {
        cerr << "Board: operator==: En passant: " << this->en_passant_available << " != " << test_board->en_passant_available << " || " << this->en_passant_x << " != " << test_board->en_passant_x << " || " << this->en_passant_y << " != " << test_board->en_passant_y << endl;
        return false;
    }

    // Half turn since last capture
    if (this->half_turn_rule != test_board->half_turn_rule)
    {
        cerr << "Board: operator==: Half turn rule: " << this->half_turn_rule << " != " << test_board->half_turn_rule << endl;
        return false;
    }

    // Game turn
    if (this->game_turn != test_board->game_turn)
    {
        cerr << "Board: operator==: Game turn: " << this->game_turn << " != " << test_board->game_turn << endl;
        return false;
    }

    // Game rules
    if (this->chess960_rule != test_board->chess960_rule)
    {
        cerr << "Board: operator==: Chess960 rule: " << this->chess960_rule << " != " << test_board->chess960_rule << endl;
        return false;
    }

    return true;
}

# endif
