#include "Move.hpp"

# pragma region Board

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
    this->promotion = tolower(_promotion); // Promotion is always lowercase, because its UCI representation is lowercase
}

void Move::log() {
    cerr << "Move: src_x = " << this->src_x << endl;
    cerr << "Move: src_y = " << this->src_y << endl;
    cerr << "Move: dst_x = " << this->dst_x << endl;
    cerr << "Move: dst_y = " << this->dst_y << endl;
    cerr << "Move: Promote to " << (this->promotion ? this->promotion : EMPTY_CELL) << endl;
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
        uci += string(1, this->promotion);
    
    return uci;
}

bool Move::compare_move_vector(vector<Move> movelst1, vector<Move> movelst2)
{
    bool success = true;

    // Assert all legal moves were found by the engine
    for (Move move_f : movelst1)
    {
        if (!_is_move_in_movelst(&move_f, movelst2))
        {
            cerr << "- Move from vector 1 isn't in vector 2 : " << move_f.to_uci() << endl;
            move_f.log();
            success = false;
        }
    }

    // Assert all moves found by the engine are legal
    for (Move move_f : movelst2)
    {
        if (!_is_move_in_movelst(&move_f, movelst1))
        {
            cerr << "- Move from vector 2 isn't in vector 1 : " << move_f.to_uci() << endl;
            move_f.log();
            success = false;
        }
    }

    return success;
}

// --- PRIVATE METHODS ---

bool Move::_is_move_in_movelst(Move *move, vector<Move> movelst)
{
    for (Move move_f : movelst)
    {
        if (*move == &move_f)
            return true;
    }

    return false;
};

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

# pragma region Board

// --- BIT BOARD IMPLEMENTATION---

# pragma region BitBoard

// #include "Move.hpp"

// // --- PUBLIC METHODS ---

// Move::Move(string uci)
// {
//     // Normal move, castles:   e2e4,
//     // Promotion:              e7e8q
//     this->piece = '.';
//     this->src = algebraic_to_bitboard(uci.substr(0, 2));
//     this->dst = algebraic_to_bitboard(uci.substr(2, 2));

//     // A fifth character represent the promotion  
//     this->promotion = uci.length() > 4 ? uci[4] : 0;
// }

// Move::Move(char piece, uint64_t _src, uint64_t _dst, char _promotion)
// {
//     this->piece = piece;
//     this->src = _src;
//     this->dst = _dst;
//     this->promotion = tolower(_promotion); // Promotion is always lowercase, because its UCI representation is lowercase
// }

// void Move::log() {
//     cerr << "Move: Piece = " << this->piece << endl;
//     cerr << "Move: src = " << bitboard_to_algebraic(this->src) << endl;
//     cerr << "Move: dst = " << bitboard_to_algebraic(this->dst) << endl;
//     cerr << "Move: Promote to " << (this->promotion ? this->promotion : EMPTY_CELL) << endl;
// }

// string Move::to_uci(bool chess960_rules)
// {
//     uint64_t tmp_dst = this->dst;
//     // Standard castles :       e1g1 or e1c1
//     if (!chess960_rules)
//         tmp_dst = this->dst < this->src ? 1UL << 58 : 1UL << 62;

//     // Normal move + castles:   e2e4
//     string uci = bitboard_to_algebraic(this->src) + bitboard_to_algebraic(tmp_dst);

//     // Promotion:               e7e8q
//     if (this->promotion)
//         uci += string(1, this->promotion);
    
//     return uci;
// }

# pragma endregion BitBoard
