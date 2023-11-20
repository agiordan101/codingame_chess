#include "Board.hpp"

// --- PUBLIC METHODS ---

Board::Board(string _fen) {
    
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

    _main_parsing(board, color, castling, en_passant, stoi(half_turn_rule), stoi(game_turn));
}

Board::Board(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _game_turn) {
    _main_parsing(_board, _color, _castling, _en_passant, half_turn_rule, game_turn);
}

void Board::log() {
    cerr << "Board: Turn: " << (white_turn ? "White" : "Black") << endl;
    cerr << "Board: Castling: w " << castles[0] << " | w " << castles[1] << " | b " << castles[2] << " | b " << castles[3] << endl;
    cerr << "Board: Kings initial columns: w " << kings_initial_columns[0] << " - b " << kings_initial_columns[1] << endl;
    cerr << "Board: En passant: Available=" << en_passant_available << " Coordonates= " << en_passant_x << " " << en_passant_y << endl;
    cerr << "Board: half_turn_rule: " << half_turn_rule << endl;
    cerr << "Board: game_turn: " << game_turn << endl;

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

vector<Move> Board::find_moves() {

    vector<Move> moves;
    vector<Move> piece_moves;
    char piece_letter;

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
                    piece_moves = _find_moves_pawns(x, y);
                    break;
                case 'n':
                    piece_moves = _find_moves_knights(x, y);
                    break;
                case 'b':
                    piece_moves = _find_moves_bishops(x, y);
                    break;
                case 'r':
                    piece_moves = _find_moves_rooks(x, y);
                    break;
                case 'k':
                    piece_moves = _find_moves_king(x, y);
                    break;
                case 'q':
                    piece_moves = _find_moves_queens(x, y);
                    break;
            }

            moves.insert(moves.end(), piece_moves.begin(), piece_moves.end());
        }
    }

    //Don't forget to create a move if en_passant=True

    // moves.push_back(Move());

    this->available_moves = moves;
    return moves;
}

void Board::apply_move(Move move)
{
    _apply_move(move.src_x, move.src_y, move.dst_x, move.dst_y, move.castle, move.promotion, move.en_passant);
    
    // Increment game turn after black turn
    if (!white_turn)
        game_turn += 1;

    white_turn = !white_turn;
    half_turn_rule += 1;

    // Reset specific turn values
    moves_computed = false;

    _update_en_passant();
    _update_castling_rights();
    _update_check(); // Info could be passed in Move ?
    _update_fen_history();
}

float Board::game_state()
{
    // Fifty-Move rule + Game turn limit
    if (
        half_turn_rule >= 50 ||
        game_turn >= game_turn_max ||
        _threefold_repetition_rule() ||
        _insufficient_material_rule()
    )
        return 0.5;

    if (!moves_computed)
    {
        available_moves = find_moves();
        moves_computed = true;
    }

    // If no moves are available, it's either a Checkmate or a Stalemate
    if (available_moves.size() == 0)
        return check ? 0 : 0.5;

    return -1;
}

string Board::create_fen(bool with_turns)
{
    char fen[85];
    int fen_i = 0;

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

// --- PRIVATE METHODS ---

void Board::_main_parsing(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _game_turn)
{
    // Parse FEN data
    _parse_board(_board);
    white_turn = _color == "w";
    _parse_castling(_castling);
    _parse_en_passant(_en_passant);
    half_turn_rule = _half_turn_rule;
    game_turn = _game_turn;

    fen_history_index = 0;
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
    //     cout << "Castle parsing " << castles[i] << endl;

    // Find kings initial column indexes
    for (int i = 0; i < 8; i++)
    {
        if (board[7][i] == 'K')
            kings_initial_columns[0] = i;
        if (board[0][i] == 'k')
            kings_initial_columns[1] = i;
    }
    // for (int i = 0; i < 2; i++)
    //     cout << "King initial pos " << kings_initial_columns[i] << endl;
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

void Board::_apply_move(int src_x, int src_y, int dst_x, int dst_y, bool castle, char promotion, bool en_passant) {

    if (castle)
    {
        char king;
        char rook;
        if (white_turn)
        {
            king = 'K';
            rook = 'R';
        }
        else
        {
            king = 'k';
            rook = 'r';
        }

        // In this engine, castles are represented by moving the king to its own rook (As Chess960 rules)
        // First, we remove both pieces
        board[src_y][src_x] = EMPTY_CELL;
        board[dst_y][dst_x] = EMPTY_CELL;

        // Then, we force THE ONLY valid castle final position
        if (dst_x < src_x)
        {
            // Left castle
            board[src_y][2] = king;
            board[src_y][3] = rook;
        }
        else
        {
            // Right castle
            board[src_y][6] = king;
            board[src_y][5] = rook;
        }

        return ;
    }
    
    if (promotion)
    {
        // Promote the pawn : A valid piece must have been created in find_move
        board[src_y][src_x] = EMPTY_CELL;
        board[dst_y][dst_x] = promotion;
        return ;
    }
    
    if (en_passant)
    {
        // Eat the pawn
        board[src_y][dst_x] = EMPTY_CELL;
    }

    // When a pawn jump two cells: Save coordinates where the opponent pawn could take it
    if (tolower(board[src_y][src_x]) == 'p')
    {
        // Fifty-Move rule: Reset half turn counter if a pawn is moved (-1 because it will be incremented at the end of the turn)
        half_turn_rule = -1;

        if (abs(dst_y - src_y) == 2)
        {
            en_passant_x = dst_x;
            en_passant_y = dst_y > src_y ? dst_y - 1 : dst_y + 1;
        }
    }

    // Fifty-Move rule: Reset half turn counter if a piece is captured (-1 because it will be incremented at the end of the turn)
    else if (board[dst_y][dst_x] != EMPTY_CELL)
        half_turn_rule = -1;

    board[dst_y][dst_x] = board[src_y][src_x];
    board[src_y][src_x] = EMPTY_CELL;
}

void Board::_update_en_passant() {

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

void Board::_update_check()
{
    check = false;
}

void Board::_update_fen_history()
{
    // Remove half turn rule and game turn (For future Threefold rule comparisons)
    string fen = create_fen(false);

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
                if (knight_found)
                    return false;

                knight_found = true;
            }
            else if (piece == 'b')
            {
                // If there is more than one bishop (Not on the same color), it's not a material insufficient
                if (bishop_found && bishop_odd != (x + y) % 2)
                    return false;

                bishop_found = true;
                bishop_odd = (x + y) % 2; // Save the color of the first bishop. Even=0, Odd=1
            }
        }
    }

    return true;
}

vector<Move>    Board::_find_moves_pawns(int x, int y) {
    
    vector<Move> moves;

    return moves;
}

vector<Move>    Board::_find_moves_knights(int x, int y) {
    
    vector<Move> moves;

    return moves;
}

vector<Move>    Board::_find_moves_bishops(int x, int y) {
    
    vector<Move> moves;

    return moves;
}

vector<Move>    Board::_find_moves_rooks(int x, int y) {
    
    vector<Move> moves;

    return moves;
}

vector<Move>    Board::_find_moves_queens(int x, int y) {
    
    vector<Move> moves;

    return moves;
}

vector<Move>    Board::_find_moves_king(int x, int y) {
    
    vector<Move> moves;

    return moves;
}

// --- OPERATORS ---

bool    Board::operator ==(Board *test_board) {

    // Test equalities on all FEN data

    // Board pieces equality
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (this->board[y][x] != test_board->board[y][x])
                return false;

    // Turn equality
    if (this->white_turn != test_board->white_turn)
            return false;
    
    // Catling rights equality
    for (int i = 0; i < 4; i += 2)
    {
        // Could be at the same place
        // or the other one
        if (this->castles[i] >= 0 &&
            this->castles[i] != test_board->castles[i] &&
            this->castles[i] != test_board->castles[i + 1])
            return false;
        
        if (this->castles[i + 1] >= 0 &&
            this->castles[i + 1] != test_board->castles[i] &&
            this->castles[i + 1] != test_board->castles[i + 1])
            return false;
    }

    // En passant equality
    if (this->en_passant_available != test_board->en_passant_available ||
        this->en_passant_x != test_board->en_passant_x ||
        this->en_passant_y != test_board->en_passant_y)
            return false;

    // Half turn since last capture
    if (this->half_turn_rule != test_board->half_turn_rule)
            return false;

    // Game turn
    if (this->game_turn != test_board->game_turn)
            return false;

    return true;            
}
