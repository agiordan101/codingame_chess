#include "Board.hpp"

bool     Board::lookup_tables_initialized = false;
uint64_t Board::pawn_captures_lookup[64][2];
uint64_t Board::knight_lookup[64];
uint64_t Board::sliding_lookup[64][8];
uint64_t Board::king_lookup[64];

// --- PUBLIC METHODS ---

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

    // Find all individual rooks in white_castles
    string   white_castles_pos[2] = {"N/A", "N/A"};
    int      white_castles_pos_i = 0;
    uint64_t castle_tmp = white_castles;
    while (castle_tmp)
    {
        rook = _get_least_significant_bit(castle_tmp);
        white_castles_pos[white_castles_pos_i++] = bitboard_to_algebraic(rook);

        // Remove the actual rook from castle_tmp, so we can find the next one
        castle_tmp ^= rook;
    }

    // Find all individual rooks in black_castles
    string black_castles_pos[2] = {"N/A", "N/A"};
    int    black_castles_pos_i = 0;
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

int Board::get_castling_rights()
{
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

string Board::get_name()
{
    return "BitBoard";
}

string Board::create_fen(bool with_turns)
{
    char fen[85];
    int  fen_i = 0;

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
                continue;
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
    // TODO: Create a new constructor, taking an instance in param, which then copy all the needed
    // data instead of creating and parsing FEN
    Board *cloned_board = new Board(create_fen(), chess960_rule, codingame_rule);

    // Copy history
    for (int i = 0; i < FEN_HISTORY_SIZE; i++)
    {
        if (this->fen_history[i].empty())
            break;
        cloned_board->fen_history[i] = this->fen_history[i];
    }

    return cloned_board;
}

// --- PRIVATE METHODS ---

// - Parsing -

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

    // Initialize private variables
    chess960_rule = _chess960_rule;
    codingame_rule = _codingame_rule;

    _initialize_bitboards();

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

    check_state = false;
    double_check = false;
    uncheck_mask = BITMASK_ALL_CELLS;
    attacked_cells_mask = BITMASK_ALL_CELLS;
}

void Board::_parse_board(string fen_board)
{
    char pos_index = 0;

    // Parse _board string into boards
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
    // '-' means that no castling are available
    if (castling_fen == "-")
        return;

    // Parse castling fen 'AHah' into 0707 for example
    for (size_t i = 0; i < castling_fen.length(); i++)
    {
        if (isupper(castling_fen[i]))
        {
            if (chess960_rule)
                white_castles |= algebraic_to_bitboard(string(1, castling_fen[i]) + '1');
            else
                white_castles |=
                    castling_fen[i] == 'K' ? 0x8000000000000000UL : 0x0100000000000000UL; // Rooks
        }
        else
        {
            if (chess960_rule)
                black_castles |= algebraic_to_bitboard(string(1, castling_fen[i]) + '8');
            else
                black_castles |= castling_fen[i] == 'k' ? 0b10000000UL : 0b00000001UL; // Rooks
        }
    }
}

// - Accessibility / Getters -

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

void Board::_create_fen_for_chess960_castling(char *fen, int *fen_i)
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

void Board::_apply_regular_white_move(uint64_t src, uint64_t dst, uint64_t *piece_mask)
{
    _capture_black_pieces(dst);

    // Remove the piece from its actual cell, and add the piece to the destination cell
    *piece_mask &= ~src;
    *piece_mask |= dst;
}

void Board::_apply_regular_black_move(uint64_t src, uint64_t dst, uint64_t *piece_mask)
{
    _capture_white_pieces(dst);

    // Remove the piece from its actual cell, and add the piece to the destination cell
    *piece_mask &= ~src;
    *piece_mask |= dst;
}

void Board::_move_white_pawn(uint64_t src, uint64_t dst, char promotion)
{
    // Fifty-Move rule: Reset half turn counter if a pawn is moved (-1 because it will be
    // incremented at the end of the turn)
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

void Board::_move_black_pawn(uint64_t src, uint64_t dst, char promotion)
{
    // Fifty-Move rule: Reset half turn counter if a pawn is moved (-1 because it will be
    // incremented at the end of the turn)
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

        // Remove the piece from its actual cell, and add the piece to the destination cell
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
}

void Board::_move_black_king(uint64_t src, uint64_t dst, castle_info_e castle_info)
{
    // When move is created from UCI, there is no castle information, so we need to compute it
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
}

void Board::_capture_white_pieces(uint64_t dst)
{
    // Detect if a piece is captured
    if (all_pieces_mask & dst)
    {
        // Fifty-Move rule: Reset half turn counter if a piece is captured (-1 because it will be
        // incremented at the end of the turn)
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

void Board::_capture_black_pieces(uint64_t dst)
{
    // Detect if a piece is captured
    if (all_pieces_mask & dst)
    {
        // Fifty-Move rule: Reset half turn counter if a piece is captured (-1 because it will be
        // incremented at the end of the turn)
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

// - Engine updates -

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
    attacked_cells_mask = 0UL;

    _update_check_and_pins();
    _update_attacked_cells_mask();

    // pawn_uncheck_mask is only set for a the specific case, so we must add generic uncheck_mask to
    // it
    pawn_uncheck_mask |= uncheck_mask;

#if USE_VISUAL_BOARD == 1
    if (PRINT_DEBUG_DATA)
    {
        this->visual_board.printSpecificBoard('A', attacked_cells_mask, "Attacked cells mask");
        this->visual_board.printSpecificBoard('C', uncheck_mask, "Uncheck mask");
    }
#endif

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

    // If our king can take an opponent piece using its movements, then the opponent piece is
    // checking our king.
    int king_lkt_i = _count_trailing_zeros(ally_king);

    // Compute pawn attacks
    _update_pawn_check(king_lkt_i);

    // Compute knight attacks
    uint64_t knight_attacks = knight_lookup[king_lkt_i] & enemy_knights;
    if (knight_attacks)
    {
        check_state = true;
        uncheck_mask |= knight_attacks;
    }

    // Compute diagonale attacks
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

    // Compute line attacks
    _compute_sliding_piece_negative_ray_checks_and_pins(
        ally_king, NORTH, enemy_pieces_sliding_line
    );
    _compute_sliding_piece_positive_ray_checks_and_pins(ally_king, EAST, enemy_pieces_sliding_line);
    _compute_sliding_piece_positive_ray_checks_and_pins(
        ally_king, SOUTH, enemy_pieces_sliding_line
    );
    _compute_sliding_piece_negative_ray_checks_and_pins(ally_king, WEST, enemy_pieces_sliding_line);

    // If no checks found, then there are no move restrictions
    if (uncheck_mask == 0UL)
    {
        check_state = false;
        uncheck_mask = BITMASK_ALL_CELLS;
    }
}

void Board::_update_pawn_check(int king_lkt_i)
{
    int lkt_color = white_turn ? 0 : 1;

    // Compute pawn attacks
    uint64_t attacking_pawn = pawn_captures_lookup[king_lkt_i][lkt_color] & enemy_pawns;
    if (attacking_pawn)
    {
        check_state = true;
        uncheck_mask |= attacking_pawn;

        // Attacking pawn could be captured by taking the en-passant
        if (en_passant)
        {
            if (white_turn)
            {
                if (attacking_pawn == (en_passant << 8))
                {
                    // But only if no opponent diagonal sliding piece is hidden behind it
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
                    // But only if no opponent diagonal sliding piece is hidden behind it
                    if (!_is_sliding_piece_positive_diagonal_ray_behind(
                            attacking_pawn, ally_king == attacking_pawn + 7 ? SOUTHWEST : SOUTHEAST
                        ))
                        pawn_uncheck_mask = en_passant;
                }
            }
        }
    }
}

void Board::_update_attacked_cells_mask()
{
    if (white_turn)
    {
        _apply_function_on_all_pieces(
            black_pawns, [this](uint64_t param) { _find_black_pawns_attacks(param); }
        );
        _apply_function_on_all_pieces(
            black_knights, [this](uint64_t param) { _find_black_knights_attacks(param); }
        );
        _apply_function_on_all_pieces(
            black_bishops, [this](uint64_t param) { _find_black_bishops_attacks(param); }
        );
        _apply_function_on_all_pieces(
            black_rooks, [this](uint64_t param) { _find_black_rooks_attacks(param); }
        );
        _apply_function_on_all_pieces(
            black_queens, [this](uint64_t param) { _find_black_queens_attacks(param); }
        );
        _find_black_king_attacks();
    }
    else
    {
        _apply_function_on_all_pieces(
            white_pawns, [this](uint64_t param) { _find_white_pawns_attacks(param); }
        );
        _apply_function_on_all_pieces(
            white_knights, [this](uint64_t param) { _find_white_knights_attacks(param); }
        );
        _apply_function_on_all_pieces(
            white_bishops, [this](uint64_t param) { _find_white_bishops_attacks(param); }
        );
        _apply_function_on_all_pieces(
            white_rooks, [this](uint64_t param) { _find_white_rooks_attacks(param); }
        );
        _apply_function_on_all_pieces(
            white_queens, [this](uint64_t param) { _find_white_queens_attacks(param); }
        );
        _find_white_king_attacks();
    }
}

void Board::_update_engine_at_turn_end()
{
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

void Board::_find_white_knights_attacks(uint64_t src)
{
    int src_lkt_i = _count_trailing_zeros(src);
    attacked_cells_mask |= knight_lookup[src_lkt_i];
}

void Board::_find_white_bishops_attacks(uint64_t src)
{
    attacked_cells_mask |= _get_diagonal_rays(src, ally_king);
}

void Board::_find_white_rooks_attacks(uint64_t src)
{
    attacked_cells_mask |= _get_line_rays(src, ally_king);
}

void Board::_find_white_queens_attacks(uint64_t src)
{
    attacked_cells_mask |= _get_diagonal_rays(src, ally_king) | _get_line_rays(src, ally_king);
}

void Board::_find_white_king_attacks()
{
    // TODO: This condition is only needed for testing purposes, when no king is on the board
    if (white_king)
    {
        int src_lkt_i = _count_trailing_zeros(white_king);
        attacked_cells_mask |= king_lookup[src_lkt_i];
    }
}

void Board::_find_black_pawns_attacks(uint64_t src)
{
    int src_lkt_i = _count_trailing_zeros(src);
    attacked_cells_mask |= pawn_captures_lookup[src_lkt_i][1];
}

void Board::_find_black_knights_attacks(uint64_t src)
{
    int src_lkt_i = _count_trailing_zeros(src);
    attacked_cells_mask |= knight_lookup[src_lkt_i];
}

void Board::_find_black_bishops_attacks(uint64_t src)
{
    attacked_cells_mask |= _get_diagonal_rays(src, ally_king);
}

void Board::_find_black_rooks_attacks(uint64_t src)
{
    attacked_cells_mask |= _get_line_rays(src, ally_king);
}

void Board::_find_black_queens_attacks(uint64_t src)
{
    attacked_cells_mask |= _get_diagonal_rays(src, ally_king) | _get_line_rays(src, ally_king);
}

void Board::_find_black_king_attacks()
{
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
    // Reset move list
    this->available_moves.clear();

    if (white_turn)
    {
        if (!double_check)
        {
            _apply_function_on_all_pieces(
                white_pawns, [this](uint64_t param) { _find_white_pawns_moves(param); }
            );
            _apply_function_on_all_pieces(
                white_knights, [this](uint64_t param) { _find_white_knights_moves(param); }
            );
            _apply_function_on_all_pieces(
                white_bishops, [this](uint64_t param) { _find_white_bishops_moves(param); }
            );
            _apply_function_on_all_pieces(
                white_rooks, [this](uint64_t param) { _find_white_rooks_moves(param); }
            );
            _apply_function_on_all_pieces(
                white_queens, [this](uint64_t param) { _find_white_queens_moves(param); }
            );
            _apply_function_on_all_pieces(
                white_castles, [this](uint64_t param) { _find_white_castle_moves(param); }
            );
        }
        _find_white_king_moves();
    }
    else
    {
        if (!double_check)
        {
            _apply_function_on_all_pieces(
                black_pawns, [this](uint64_t param) { _find_black_pawns_moves(param); }
            );
            _apply_function_on_all_pieces(
                black_knights, [this](uint64_t param) { _find_black_knights_moves(param); }
            );
            _apply_function_on_all_pieces(
                black_bishops, [this](uint64_t param) { _find_black_bishops_moves(param); }
            );
            _apply_function_on_all_pieces(
                black_rooks, [this](uint64_t param) { _find_black_rooks_moves(param); }
            );
            _apply_function_on_all_pieces(
                black_queens, [this](uint64_t param) { _find_black_queens_moves(param); }
            );
            _apply_function_on_all_pieces(
                black_castles, [this](uint64_t param) { _find_black_castle_moves(param); }
            );
        }
        _find_black_king_moves();
    }

    this->moves_computed = true;
}

void Board::_find_white_pawns_moves(uint64_t src)
{
    // Generate pawn moves: (pawn position shifted) & ~ColumnA & enemy_pieces & check_mask &
    int src_lkt_i = _count_trailing_zeros(src);

    uint64_t capture_moves = pawn_captures_lookup[src_lkt_i][0] & capturable_by_white_pawns_mask;
    uint64_t advance_move = (src >> 8) & empty_cells_mask;
    uint64_t legal_moves =
        (capture_moves | advance_move) & pawn_uncheck_mask & pin_masks[src_lkt_i];

    // When pawn is on the 7th rank, it can move two squares forward
    // We just need to check if the squares in front of it are empty
    if (src & BITMASK_LINE_2 && (src >> 8) & empty_cells_mask)
        legal_moves |= (src >> 16) & empty_cells_mask & pawn_uncheck_mask & pin_masks[src_lkt_i];

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
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves =
        knight_lookup[src_lkt_i] & not_white_pieces_mask & uncheck_mask & pin_masks[src_lkt_i];

    _create_piece_moves('N', src, legal_moves);
}

void Board::_find_white_bishops_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves =
        not_white_pieces_mask & _get_diagonal_rays(src) & uncheck_mask & pin_masks[src_lkt_i];

    _create_piece_moves('B', src, legal_moves);
}

void Board::_find_white_rooks_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves =
        not_white_pieces_mask & _get_line_rays(src) & uncheck_mask & pin_masks[src_lkt_i];

    _create_piece_moves('R', src, legal_moves);
}

void Board::_find_white_queens_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves = not_white_pieces_mask & (_get_diagonal_rays(src) | _get_line_rays(src)) &
                           uncheck_mask & pin_masks[src_lkt_i];

    _create_piece_moves('Q', src, legal_moves);
}

void Board::_find_white_king_moves()
{
    if (white_king)
    {
        int      src_lkt_i = _count_trailing_zeros(white_king);
        uint64_t legal_moves =
            king_lookup[src_lkt_i] & not_white_pieces_mask & ~attacked_cells_mask;

        _create_piece_moves('K', white_king, legal_moves);
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

        // Castle is legal if :
        // - No pieces are blocking both king and rook paths
        // - No cells are attacked in the king path
        // - Rook isn't pinned
        if (((king_path | rook_path) & (all_pieces_mask ^ white_king ^ rook)) == 0UL &&
            (king_path & attacked_cells_mask) == 0UL &&
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

    // When pawn is on the 7th rank, it can move two squares forward
    // We just need to check if the squares in front of it are empty
    if (src & BITMASK_LINE_7 && (src << 8) & empty_cells_mask)
        legal_moves |= (src << 16) & empty_cells_mask & pawn_uncheck_mask & pin_masks[src_lkt_i];

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
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves =
        knight_lookup[src_lkt_i] & not_black_pieces_mask & uncheck_mask & pin_masks[src_lkt_i];

    _create_piece_moves('n', src, legal_moves);
}

void Board::_find_black_bishops_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves =
        not_black_pieces_mask & _get_diagonal_rays(src) & uncheck_mask & pin_masks[src_lkt_i];

    _create_piece_moves('b', src, legal_moves);
}

void Board::_find_black_rooks_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves =
        not_black_pieces_mask & _get_line_rays(src) & uncheck_mask & pin_masks[src_lkt_i];

    _create_piece_moves('r', src, legal_moves);
}

void Board::_find_black_queens_moves(uint64_t src)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t legal_moves = not_black_pieces_mask & (_get_diagonal_rays(src) | _get_line_rays(src)) &
                           uncheck_mask & pin_masks[src_lkt_i];

    _create_piece_moves('q', src, legal_moves);
}

void Board::_find_black_king_moves()
{
    if (black_king)
    {
        int      src_lkt_i = _count_trailing_zeros(black_king);
        uint64_t legal_moves =
            king_lookup[src_lkt_i] & not_black_pieces_mask & ~attacked_cells_mask;

        _create_piece_moves('k', black_king, legal_moves);
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

        // Castle is legal if :
        // - No pieces are blocking both king and rook paths
        // - No cells are attacked in the king path
        // - Rook isn't pinned
        if (((king_path | rook_path) & (all_pieces_mask ^ black_king ^ rook)) == 0UL &&
            (king_path & attacked_cells_mask) == 0UL &&
            (pin_masks[_count_trailing_zeros(rook)] == BITMASK_ALL_CELLS))
        {
            this->available_moves.push_back(Move('k', black_king, rook, 0, castle_info));
        }
    }
}

void Board::_add_regular_move_or_promotion(char piece, uint64_t src, uint64_t dst)
{
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
    // Find all individual bits in legal_moves
    uint64_t dst;
    while (legal_moves)
    {
        dst = _get_least_significant_bit(legal_moves);
        this->available_moves.push_back(Move(piece, src, dst));

        // Remove the actual bit from legal_moves, so we can find the next one
        legal_moves ^= dst;
    }
}

// - Bit operations -

void Board::_apply_function_on_all_pieces(uint64_t bitboard, std::function<void(uint64_t)> func)
{
    // Find all individual bits in bitboard
    uint64_t piece;
    while (bitboard)
    {
        piece = _get_least_significant_bit(bitboard);
        func(piece);

        // Remove the actual bit from bitboard, so we can find the next one
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

    // Find all pieces in the ray (Ignoring ally king when searching attacked cells)
    uint64_t blockers = attacks & (all_pieces_mask ^ piece_to_ignore);
    if (blockers)
    {
        // Find the first blocker in the ray (the one with the smallest index)
        int src_lkt_i = _count_trailing_zeros(blockers);

        // Remove all squares behind the blocker from the attacks
        attacks ^= sliding_lookup[src_lkt_i][dir];
    }

    // Blocker should be in the ray, so ~color_pieces_mask removes it if it's an ally
    return attacks;
}

uint64_t
Board::_compute_sliding_piece_negative_ray(uint64_t src, ray_dir_e dir, uint64_t piece_to_ignore)
{
    int      src_lkt_i = _count_trailing_zeros(src);
    uint64_t attacks = sliding_lookup[src_lkt_i][dir];

    // Find all pieces in the ray (Ignoring ally king when searching attacked cells)
    uint64_t blockers = attacks & (all_pieces_mask ^ piece_to_ignore);
    if (blockers)
    {
        // Find the first blocker in the ray (the one with the biggest index)
        uint64_t blocker = _get_most_significant_bit(blockers);
        int      src_lkt_i = _count_trailing_zeros(blocker);

        // Remove all squares behind the blocker from the attacks
        attacks ^= sliding_lookup[src_lkt_i][dir];
    }

    // Blocker should be in the ray, so ~color_pieces_mask removes it if it's an ally
    return attacks;
}

void Board::_compute_sliding_piece_positive_ray_checks_and_pins(
    uint64_t king_pos, ray_dir_e dir, uint64_t potential_attacker
)
{
    int      king_lkt_i = _count_trailing_zeros(king_pos);
    uint64_t attacks = sliding_lookup[king_lkt_i][dir];

    // Find all pieces in the ray
    uint64_t blockers = attacks & all_pieces_mask;

    if (blockers)
    {
        // Find the first blocker in the ray (the one with the smallest index)
        uint64_t blocker = _get_least_significant_bit(blockers);
        int      blocker_lkt_i = _count_trailing_zeros(blocker);

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
        else if (blocker & ally_pieces)
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
                    int      second_blocker_lkt_i = _count_trailing_zeros(blocker);
                    uint64_t pin_ray = attacks ^ sliding_lookup[second_blocker_lkt_i][dir];

                    // Save this pin ray on the ally blocker cell
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

    // Find all pieces in the ray
    uint64_t blockers = attacks & all_pieces_mask;

    if (blockers)
    {
        // Find the first blocker in the ray (the one with the greatest index)
        uint64_t blocker = _get_most_significant_bit(blockers);
        int      blocker_lkt_i = _count_trailing_zeros(blocker);

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
        else if (blocker & ally_pieces)
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
                    int      second_blocker_lkt_i = _count_trailing_zeros(blocker);
                    uint64_t pin_ray = attacks ^ sliding_lookup[second_blocker_lkt_i][dir];

                    // Save this pin ray on the ally blocker cell
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

    // Find all pieces in the ray
    uint64_t blockers = attacks & all_pieces_mask;
    if (blockers)
    {
        // Find the first blocker in the ray (the one with the smallest index)
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

    // Find all pieces in the ray
    uint64_t blockers = attacks & all_pieces_mask;
    if (blockers)
    {
        // Find the first blocker in the ray (the one with the greatest index)
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

// - End game -

float Board::_compute_game_state()
{
    // Fifty-Move rule + Game turn limit + 2 other rules to detect a draw
    if (half_turn_rule >= 99 || _threefold_repetition_rule() || _insufficient_material_rule())
        return DRAW;

    // Convert this to PRE PROCESSING if ?
    if (codingame_rule)
    {
        if (game_turn > 125)
            return DRAW;
    }

    vector<Move> moves = get_available_moves();

    // If no moves are available, it's either a Checkmate or a Stalemate
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
    int    actual_fen_index = fen_history_index - 1;
    string actual_fen = fen_history[actual_fen_index];

    // Check if the actual FEN is already 2 times in the history
    // Loop over all FEN_HISTORY_SIZE last moves, skipping the actual one
    bool fen_found = false;
    int  history_index = -1;
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
    if (white_pawns | black_pawns | white_rooks | black_rooks | white_queens | black_queens)
        return false;

    int white_knights_count = _count_bits(white_knights);
    int black_knights_count = _count_bits(black_knights);
    int knights_count = white_knights_count + black_knights_count;

    // If there is multiple knights, it's not a material insufficient
    if (knights_count > 1)
        return false;

    int white_bishops_count = _count_bits(white_bishops);
    int black_bishops_count = _count_bits(black_bishops);
    int bishops_count = white_bishops_count + black_bishops_count;

    // If there is a knight and another piece, it's not a material insufficient
    if (knights_count == 1 && bishops_count > 0)
        return false;

    // If there is more than one bishop, on opposite colors, it's not a material insufficient
    uint64_t all_bishops = white_bishops | black_bishops;
    if ((all_bishops & BITMASK_WHITE_CELLS) && (all_bishops & BITMASK_BLACK_CELLS))
        return false;

    return true;
}

// --- OPERATORS ---

bool Board::operator==(Board *test_board_abstracted)
{
    // TODO: move this in the Board class, as inlined method
    return this->create_fen() == test_board_abstracted->create_fen();
}

// --- STATIC LOOKUP METHODS ---

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
            // Create the lookup table for the current position
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
    // We need pawn attacks on lines 0 and 7, because we are using them to find checks (when kings
    // are on those lines)

    // - White pawns -
    uint64_t pawn_mask = 0UL;

    // Left capture
    if (x > 0 && y > 0)
        pawn_mask |= (position >> 9);
    // Right capture
    if (x < 7 && y > 0)
        pawn_mask |= (position >> 7);

    pawn_captures_lookup[lkt_i][0] = pawn_mask;

    // - Black pawns -
    pawn_mask = 0UL;

    // Left capture
    if (x > 0 && y < 7)
        pawn_mask |= position << 7;
    // Right capture
    if (x < 7 && y < 7)
        pawn_mask |= position << 9;

    pawn_captures_lookup[lkt_i][1] = pawn_mask;
}

void Board::_create_knight_lookup_table(int y, int x, uint64_t position, int lkt_i)
{
    uint64_t knight_mask = 0UL;

    if (y > 0)
    {
        // 2 left 1 down
        if (x > 1)
            knight_mask |= position >> 10;
        // 2 right 1 down
        if (x < 6)
            knight_mask |= position >> 6;
    }
    if (y > 1)
    {
        // 2 up 1 left
        if (x > 0)
            knight_mask |= position >> 17;
        // 2 up 1 right
        if (x < 7)
            knight_mask |= position >> 15;
    }
    if (y < 6)
    {
        // 2 down 1 left
        if (x > 0)
            knight_mask |= position << 15;
        // 2 down 1 right
        if (x < 7)
            knight_mask |= position << 17;
    }
    if (y < 7)
    {
        // 2 left 1 up
        if (x > 1)
            knight_mask |= position << 6;
        // 2 right 1 up
        if (x < 6)
            knight_mask |= position << 10;
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
            king_mask |= position >> 9;
        // 1 left
        king_mask |= position >> 1;
        // 1 down left
        if (y < 7)
            king_mask |= position << 7;
    }
    if (x < 7)
    {
        // 1 up right
        if (y > 0)
            king_mask |= position >> 7;
        // 1 right
        king_mask |= position << 1;
        // 1 down right
        if (y < 7)
            king_mask |= position << 9;
    }
    // 1 up
    if (y > 0)
        king_mask |= position >> 8;
    // 1 down
    if (y < 7)
        king_mask |= position << 8;

    king_lookup[lkt_i] = king_mask;
}
