#include "Board.hpp"

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
    cerr << "Board: Turn: " << (white_turn ? "White" : "Black") << endl;
    cerr << "Board: Castling: w " << castles[0] << " | w " << castles[1] << " | b " << castles[2] << " | b " << castles[3] << endl;
    cerr << "Board: Kings initial columns: w " << kings_initial_columns[0] << " - b " << kings_initial_columns[1] << endl;
    cerr << "Board: En passant: Available=" << en_passant_available << " Coordonates= " << en_passant_x << " " << en_passant_y << endl;
    cerr << "Board: half_turn_rule: " << to_string(half_turn_rule) << endl;
    cerr << "Board: game_turn: " << to_string(game_turn) << endl;

    cerr << "----+----------------" << endl;
    cerr << " i  | 0 1 2 3 4 5 6 7" << endl;
    cerr << "  n | A B C D E F G H" << endl;
    cerr << "----+----------------" << endl;
    for (int y = 0; y < 8; y++)
    {
        cerr << y << " " << line_index_to_number(y) << " |";
        for (int x = 0; x < 8; x++)
            cerr << " " << board[y][x];
        cerr << endl;
    }
    cerr << "----+----------------" << endl;
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
    _update_castling_rights();
    _update_fen_history();
}

float Board::game_state()
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

bool Board::get_check_state()
{
    if (!this->check_computed)
    {
        this->check = _is_check();
        this->check_computed = true;
    }

    return this->check;
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
    // Parse FEN data
    _parse_board(_board);
    white_turn = _color == "w";
    _parse_castling(_castling);
    _parse_en_passant(_en_passant);
    half_turn_rule = _half_turn_rule;
    game_turn = _game_turn;

    // Set the right castling function pointer
    chess960_rule = _chess960_rule;
    _handle_castle = _chess960_rule ? &Board::_handle_chess960_castle : &Board::_handle_standard_castle;

    // Initialize private variables
    check_computed = false;
    moves_computed = false;

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
            castles[white_castles_i++] = column_name_to_index(castling_fen[i]);
        if (islower(castling_fen[i]))
            castles[2 + black_castles_i++] = column_name_to_index(castling_fen[i]);
    }
    // for (int i = 0; i < 4; i++)
    //     cerr << "Castle parsing " << castles[i] << endl;

    // Find kings initial column indexes
    for (int i = 0; i < 8; i++)
    {
        if (board[7][i] == 'K')
            kings_initial_columns[0] = i;
        if (board[0][i] == 'k')
            kings_initial_columns[1] = i;
    }
    // for (int i = 0; i < 2; i++)
    //     cerr << "King initial pos " << kings_initial_columns[i] << endl;
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
