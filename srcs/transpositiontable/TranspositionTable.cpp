#include "TranspositionTable.hpp"

TranspositionTable::TranspositionTable()
{
    this->_inserted_node_count = 0;
    this->_node_size = sizeof(s_MinMaxNode);
    this->_max_node_count = TT_MEMORY_SIZE / _node_size;
    this->_table = (s_MinMaxNode*)malloc(_max_node_count * _node_size);
    if (this->_table == NULL)
    {
        cerr << "TranspositionTable: malloc failed !" << endl;
        exit(1);
    }

    // Generate random hashes
    this->_black_turn_hash = rand();

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            for (int p = 0; p < 12; p++)
                this->_random_hashs[y][x][p] = rand();

    for (int i = 0; i < 16; i++)
        this->_castling_right_hashs[i] = rand();

    cerr << "TranspositionTable: _max_node_count=" << _max_node_count << " _node_size=" << _node_size << endl;
}

int TranspositionTable::create_zobrist_key(Board *board)
{
    int hash = 0;

    if (!board->is_white_turn())
    {
        hash ^= this->_black_turn_hash;
    }

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int piece = board->get_cell(x, y);
            if (piece != EMPTY_CELL)
            {
                hash ^= this->_random_hashs[y][x][piece];
            }
        }
    }

    int castling_rights = board->get_castling_rights();
    hash ^= this->_castling_right_hashs[castling_rights];

    return hash;
}

int TranspositionTable::update_key(Board *board, Move *move, int zobrist_key)
{
    int src_piece = board->get_cell(move->src_x, move->src_y);
    int dst_piece = board->get_cell(move->dst_x, move->dst_y);

    // "Revert" the src piece/square pair from the key
    zobrist_key ^= this->_random_hashs[move->src_y][move->src_x][src_piece];

    // "Insert" the new dst piece/square pair in the key
    zobrist_key ^= this->_random_hashs[move->dst_y][move->dst_x][src_piece];
    
    // If a piece is taken in the destination cell, "revert" its piece/square pair from the key
    if (dst_piece != EMPTY_CELL)
        zobrist_key ^= this->_random_hashs[move->dst_y][move->dst_x][dst_piece];

    return zobrist_key;
}

s_MinMaxNode *TranspositionTable::get_node(int zobrist_key)
{
    int index = zobrist_key % this->_max_node_count;
    s_MinMaxNode *node = &this->_table[index];

    if (node->zobrist_key == zobrist_key)
        return node;

    cerr << "TranspositionTable: COLLISION: zobrist_key=" << zobrist_key << " index=" << index << " node->zobrist_key=" << node->zobrist_key << endl;
    return NULL;
}

void TranspositionTable::insert_node(int zobrist_key, s_MinMaxNode *node)
{
    this->_inserted_node_count++;

    s_MinMaxNode *old_node = this->get_node(zobrist_key);

    if (old_node == NULL)
    {
        collision_handler(node, old_node);
        return;
    }

    memcpy(old_node, node, sizeof(s_MinMaxNode));
}

void TranspositionTable::collision_handler(s_MinMaxNode *new_node, s_MinMaxNode *node)
{
    // OVERWRITE THE NODE
    memcpy(node, new_node, sizeof(s_MinMaxNode));
}
