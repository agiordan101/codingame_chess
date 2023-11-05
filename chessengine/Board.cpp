#include "Board.hpp"

// --- PUBLIC METHODS ---

Board::Board(string _board, string _color, string _castling, string _en_passant, int _half_turn_rule, int _full_move) {
    /*  
        White - upper case
        Black - lower case
        rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1
    */
    _fen_board = _board;
    _parse_board(_board);
    white_turn = _color == "w";
    _parse_castling(_castling);
    _parse_en_passant(_en_passant);
    half_turn_rule = _half_turn_rule;
    game_turn = _full_move;
}

void Board::log() {
    // cerr << "\n\tBoard description " << board << endl;
    cerr << "Board: Turn: " << (white_turn ? "White" : "Black") << endl;
    cerr << "Board: Castling: w " << castles[0] << " | w " << castles[1] << " | b " << castles[2] << " | b " << castles[3] << endl;
    cerr << "Board: Kings initial columns: w " << kings_initial_columns[0] << " - b " << kings_initial_columns[1] << endl;
    cerr << "Board: En passant: Available=" << en_passant_available << " Coordonates= " << en_passant_x << " " << en_passant_y << endl;
    cerr << "Board: half_turn_rule: " << half_turn_rule << endl;
    cerr << "Board: game_turn: " << game_turn << endl;
    show_board();
}

void Board::show_board() {

    cerr << " ---------------" << endl;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
            cerr << " " << (board[y][x] == 0 ? '0' : (char)(board[y][x]));
        cerr << endl;
    }
    cerr << " ---------------" << endl;
}

void Board::reset_board(string new_fen_board) {

    if (!new_fen_board.empty())
        _fen_board = new_fen_board;

    // cerr << "Reset board to fen: " << _fen_board << endl;
    _parse_board(_fen_board);
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
            if (piece_letter == 0)
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

    moves.push_back(Move());

    this->available_moves = moves;
    return moves;
}

void Board::apply_move(Move move)
{
    return _apply_move(move.src_x, move.src_y, move.dst_x, move.dst_y, move.castle, move.promotion, move.en_passant);
}

void Board::next_turn() {

    white_turn = !white_turn;
    half_turn_rule += 1;
    game_turn += 1;

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

    // Disable white castles if they are still available
    if (castles[0] != -1 || castles[1] != -1)
    {
        // Disable player castle if its king moves
        if (board[kings_initial_columns[0]][7] != 'K')
        {
            castles[0] = -1;
            castles[1] = -1;
        }

        // Disable side castles if the rook moves
        if (castles[0] != -1 && board[castles[0]][7] != 'R')
            castles[0] = -1;
        if (castles[1] != -1 && board[castles[1]][7] != 'R')
            castles[1] = -1;
    }

    // Disable black castles if they are still available
    if (castles[2] != -1 || castles[3] != -1)
    {
        // Disable player castle if its king moves
        if (board[kings_initial_columns[1]][0] != 'K')
        {
            castles[2] = -1;
            castles[3] = -1;
        }

        // Disable side castles if the rook moves
        if (castles[2] != -1 && board[castles[2]][0] != 'r')
            castles[2] = -1;
        if (castles[3] != -1 && board[castles[3]][0] != 'r')
            castles[3] = -1;
    }
}

int Board::is_end_game()
{
    if (available_moves.size() == 0)
        return 1;

    //Implement all weird rules
    // 50 half turns etc...

    return 0;
}

// --- PRIVATE METHODS ---

void Board::_parse_board(string fen_board) {

    int cell_i = 0;

    memset(board, 0, sizeof(int) * 64);
    for (int i = 0; i < fen_board.length(); i++) {

        int piece = fen_board[i];
        
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
    int _castles[4] = {-1, -1, -1, -1};
    int white_castles_i = 0;
    int black_castles_i = 0;

    for (int i = 0; i < castling_fen.length(); i++)
    {
        if (islower(castling_fen[i]))
            _castles[white_castles_i++] = COLUMN_name_to_index(castling_fen[i]);
        if (isupper(castling_fen[i]))
            _castles[2 + black_castles_i++] = COLUMN_name_to_index(castling_fen[i]);
    }
    memcpy(castles, _castles, sizeof(int) * 4);
    
    // Find kings initial columns
    for (int i = 0; i < 8; i++)
    {
        if (board[i][0] == 'k')
            kings_initial_columns[0] = i;
        if (board[i][7] == 'K')
            kings_initial_columns[1] = i;
    }
}

void Board::_parse_en_passant(string _en_passant)
{
    en_passant_available = false;
    en_passant_x = -1;
    en_passant_y = -1;
}

void Board::_apply_move(int src_x, int src_y, int dst_x, int dst_y, bool castle, int promotion, bool en_passant) {

    board[dst_y][dst_x] = board[src_y][src_x];
    board[src_y][src_x] = 0;
    if (castle)
    {
        // Rook move
        int side = dst_x < src_x ? -1 : 1;
        int x;
        for (x = dst_x; 0 <= x && x < 8; x += side)
            if (toupper(board[dst_y][x]) == 'R')
                break ;

        board[dst_y][dst_x - side] = board[dst_y][x];
        board[dst_y][x] = 0;
    }
    else if (promotion)
    {
        // Promote the pawn (toupper necesary ? We'll see...)
        board[dst_y][dst_x] = board[dst_y][dst_x] > 'a' ? tolower(promotion) : toupper(promotion);
    }
    else if (en_passant)
    {
        // Eat the pawn
        board[src_y][dst_x] = 0;
    }

    // Only when a pawn jump two cells
    if (en_passant)
    {
        // Save coordinates where the opponent pawn could take it
        en_passant_x = dst_x;
        en_passant_y = dst_y > src_y ? dst_y - 1 : dst_y + 1;
    }

    next_turn();
}

vector<Move>    Board::_find_moves_pawns(int x, int y) {
    
    vector<Move> moves;

    // Don't forget to set en_passant boolean at true when a pawn jump two cells
    
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

bool    Board::operator ==(string fen_board) {

    Board *test_board = new Board(fen_board, "w", "Ahah", "-", 0, 0);

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (this->board[y][x] != test_board->board[y][x])
                return false;
    return true;            
}
