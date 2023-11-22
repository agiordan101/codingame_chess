#include "ChessEngine.hpp"
#include "Move.hpp"

// --- PUBLIC METHODS ---

Move::Move(string uci)
{
    // Normal move, castles:   e2e4, 
    // Promotion:              e7e8q
    const char *uci_char = uci.c_str();

    this->src_x = column_name_to_index(uci_char[0]);
    this->src_y = line_number_to_index(uci_char[1]);
    this->dst_x = column_name_to_index(uci_char[2]);
    this->dst_y = line_number_to_index(uci_char[3]);

    // A fifth character represent the promotion  
    this->promotion = uci_char[4] ? uci_char[4] : 0;
}

Move::Move(int _src_x, int _src_y, int _dst_x, int _dst_y, char _promotion)
{
    this->src_x = _src_x;
    this->src_y = _src_y;
    this->dst_x = _dst_x;
    this->dst_y = _dst_y;
    this->promotion = _promotion;
}

void Move::log() {
    cout << "Move: src_x = " << this->src_x << endl;
    cout << "Move: src_y = " << this->src_y << endl;
    cout << "Move: dst_x = " << this->dst_x << endl;
    cout << "Move: dst_y = " << this->dst_y << endl;
    cout << "Move: Promote to " << (this->promotion ? this->promotion : EMPTY_CELL) << endl;
}

string Move::to_uci(bool regular_rules_castling)
{
    int tmp_x = this->dst_x;
    // Standard castles :       e1g1 or e1c1
    if (regular_rules_castling)
        tmp_x = this->dst_x < this->src_x ? 2 : 6;

    // Normal move + castles:   e2e4
    string uci = coord_to_algebraic(this->src_x, this->src_y) + coord_to_algebraic(tmp_x, this->dst_y);

    // Promotion:               e7e8q
    if (this->promotion)
        uci += string(1, tolower(this->promotion));
    
    return uci;
}

// --- OPERATORS ---

bool Move::operator==(const Move *other)
{
    return
        this->src_x == other->src_x &&
        this->src_y == other->src_y &&
        this->dst_x == other->dst_x &&
        this->dst_y == other->dst_y &&
        this->promotion == other->promotion
    ;
}
