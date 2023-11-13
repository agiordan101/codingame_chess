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

Move::Move(string uci)
{
    this->src_x = 0;
    this->src_y = 0;
    this->dst_x = 0;
    this->dst_y = 0;
    this->castle = false;
    this->promotion = '_';
    this->en_passant = false;

    //Implement the uci parsing
    // Normal move: e2e4, ...
    // Castling:    e1g1 (Pareil src -> dst)
    // Promotion:   e7e8q
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
    // Normal move: e2e4, ...
    // Castling:    e1g1 (Pareil src -> dst)
    // Promotion:   e7e8q
    return string();
}

// --- OPERATORS ---

bool Move::operator==(const Move &other)
{
    return false;
}
