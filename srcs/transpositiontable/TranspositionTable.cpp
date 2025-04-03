#include "TranspositionTable.hpp"

TranspositionTable::TranspositionTable()
{
    this->_node_size = sizeof(s_TranspositionTableNode);
    this->_max_node_count = TT_MEMORY_SIZE / _node_size;
    this->_table = (s_TranspositionTableNode *)malloc(_max_node_count * _node_size);
    if (this->_table == NULL)
    {
        cerr << "TranspositionTable: malloc failed !" << endl;
        exit(1);
    }

    cerr << "TranspositionTable: _max_node_count=" << _max_node_count / 1000000
         << "millions _node_size=" << _node_size << " bytes" << endl;
}

s_TranspositionTableNode *TranspositionTable::get_node(int zobrist_key)
{
    int                       index = zobrist_key % this->_max_node_count;
    s_TranspositionTableNode *node = &this->_table[index];

    return node;
}
