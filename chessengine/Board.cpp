#include "Board.hpp"

// --- PUBLIC METHODS ---

Board::Board(string _board, string _color, string _castling, string _en_passant, int _half_move_clock, int _full_move) {
    /*  
        White - upper case
        Black - lower case
        rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1
    */
    _fen_board = _board;
    _parse_board(_board);
    player_turn = _color == "w" ? 1 : -1;
    lower_case_pieces = _color == "b";
    _parse_castling(_castling);
    en_passant = _en_passant;
    half_turn_remaning = _half_move_clock;
    game_turn = _full_move;
}

void Board::log() {
    // cerr << "\n\tBoard description " << board << endl;
    cerr << "\tBoard: Color w=1|b=0: " << player_turn << endl;
    cerr << "\tBoard: Castling: w " << castles[0] << " | " << castles[1] << " | b " << castles[2] << " | " << castles[3] << endl;
    cerr << "\tBoard: En passant: " << en_passant << endl;
    cerr << "\tBoard: half_turn_remaning: " << half_turn_remaning << endl;
    cerr << "\tBoard: game_turn: " << game_turn << endl;
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
            if (lower_case_pieces)
            {
                if (isupper(piece_letter))
                    continue;
            }
            else if (islower(piece_letter))
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

    this->available_moves = moves;
    return moves;
}

void Board::apply_move(Move move)
{
    return _apply_move(move.src_x, move.src_y, move.dst_x, move.dst_y, move.castle, move.promotion, move.en_passant);
}

int Board::is_end_game()
{
    if (available_moves.size() == 0)
        return 1;

    return 0;
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
    // cerr << "Castle parsing end for: " << castling_fen << endl;
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
