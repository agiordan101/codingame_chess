#include "TranspositionTable.hpp"

TranspositionTable::TranspositionTable()
{
    this->_inserted_node_count = 0;
    this->_node_size = sizeof(s_MinMaxNode);
    this->_max_node_count = TT_MEMORY_SIZE / _node_size;
    this->_table = (s_MinMaxNode *)malloc(_max_node_count * _node_size);
    if (this->_table == NULL)
    {
        cerr << "TranspositionTable: malloc failed !" << endl;
        exit(1);
    }

    // Generate random hashes
    this->_black_turn_hash = rand();

    for (int i = 0; i < 64; i++)
        for (int p = 0; p < 12; p++)
            this->_random_hashs[i][p] = rand();

    for (int i = 0; i < 16; i++)
        this->_castling_right_hashs[i] = rand();

    cerr << "TranspositionTable: _max_node_count=" << _max_node_count
         << " _node_size=" << _node_size << endl;
}

int TranspositionTable::create_zobrist_key(Board *board)
{
    int hash = 0;

    if (!board->is_white_turn())
    {
        hash ^= this->_black_turn_hash;
    }

    uint64_t mask = 1UL;
    for (int i = 0; i < 64; i++)
    {
        int piece = board->get_cell(mask);
        if (piece != EMPTY_CELL)
        {
            uint64_t mask_i = _count_trailing_zeros(mask);
            hash ^= this->_random_hashs[mask_i][piece];
        }
    }

    int castling_rights = board->get_castling_rights();
    hash ^= this->_castling_right_hashs[castling_rights];

    return hash;
}

int TranspositionTable::update_key(Board *board, Move *move, int zobrist_key)
{
    // TODO: Don't pass board.
    //  Move own src piece and we can send the dst piece
    // TODO: Use char instead of int
    int src_piece = move->piece;
    int dst_piece = board->get_cell(move->dst);

    uint64_t src_i = _count_trailing_zeros(move->src);
    uint64_t dst_i = _count_trailing_zeros(move->dst);

    // "Revert" the src piece/square pair from the key
    zobrist_key ^= this->_random_hashs[src_i][src_piece];

    // "Insert" the new dst piece/square pair in the key
    zobrist_key ^= this->_random_hashs[dst_i][src_piece];

    // If a piece is taken in the destination cell, "revert" its piece/square pair from the key
    if (dst_piece != EMPTY_CELL)
        zobrist_key ^= this->_random_hashs[dst_i][dst_piece];

    // TODO: Correctly handle en-passant moves
    // TODO: Correctly handle castling moves

    return zobrist_key;
}

s_MinMaxNode *TranspositionTable::get_node(int zobrist_key)
{
    int           index = zobrist_key % this->_max_node_count;
    s_MinMaxNode *node = &this->_table[index];

    // Return the node if it was already seen
    if (node->zobrist_key)
    {
        // Same zobrist key means same position
        if (node->zobrist_key == zobrist_key)
            return node;

        // Else there is a collision
        cerr << "TranspositionTable: COLLISION: zobrist_key=" << zobrist_key << " index=" << index
             << " node->zobrist_key=" << node->zobrist_key << endl;
    }

    // Else this node is new
    return NULL;
}

void TranspositionTable::insert_node(int zobrist_key, s_MinMaxNode *node)
{
    this->_inserted_node_count++;

    s_MinMaxNode *actual_node = this->get_node(zobrist_key);

    if (actual_node == NULL)
    {
        collision_handler(node, actual_node);
        return;
    }

    *actual_node = *node;
}

void TranspositionTable::collision_handler(s_MinMaxNode *new_node, s_MinMaxNode *node)
{
    // OVERWRITE THE NODE
    *node = *new_node;
}
