#include "ChessEngine.hpp"
#include "Move.hpp"

// --- PUBLIC METHODS ---

Move::Move()
{
    this->src_x = 0;
    this->src_y = 0;
    this->dst_x = 0;
    this->dst_y = 0;
    this->castle = false;
    this->promotion = '_';
    this->en_passant = false;
}

Move::Move(string uci, char piece)
{
    // Normal move + castles:   e2e4
    // Promotion:               e7e8q
    const char *uci_char = uci.c_str();

    this->src_x = column_name_to_index(uci_char[0]);
    this->src_y = uci_char[1];
    this->dst_x = column_name_to_index(uci_char[2]);
    this->dst_y = uci_char[3];

    // A castle in CodinGame with Chess960 rule is represented by moving the king to its own rook
    this->castle = tolower(piece) == 'k' && abs(this->dst_x - this->src_x) > 1;

    // A fifth character represent the promotion  
    this->promotion = uci_char[4] ? uci_char[4] : '_';

    // en_passant is generated when a pawn advance 2 cells
    this->en_passant = tolower(piece) == 'p' && abs(this->dst_y - this->src_y) == 2;
}

Move::Move(int _src_x, int _src_y, int _dst_x, int _dst_y, bool _castle, char _promotion, bool _en_passant)
{
    this->src_x = _src_x;
    this->src_y = _src_y;
    this->dst_x = _dst_x;
    this->dst_y = _dst_y;
    this->castle = _castle;
    this->promotion = _promotion;
    this->en_passant = _en_passant;
}

void Move::log() {
    cout << "Move: src_x = " << this->src_x << endl;
    cout << "Move: src_y = " << this->src_y << endl;
    cout << "Move: dst_x = " << this->dst_x << endl;
    cout << "Move: dst_y = " << this->dst_y << endl;
    cout << "Move: Is castling ? - " << this->castle << endl;
    cout << "Move: Promote to " << this->promotion << endl;
    cout << "Move: En passant ? - " << this->en_passant << endl;
}

string Move::get_uci()
{
    // Normal move + castles:   e2e4
    // Promotion:               e7e8q
    string uci = coord_to_algebraic(this->src_x, this->src_y) + coord_to_algebraic(this->dst_x, this->dst_y);

    if (this->en_passant)
        uci += string(1, this->en_passant);
    return uci;
}

// --- OPERATORS ---

bool Move::operator==(const Move &other)
{
    return false;
}
