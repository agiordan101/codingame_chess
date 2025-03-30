#ifndef TRANSPOSITIONTABLE_HPP
#define TRANSPOSITIONTABLE_HPP

#include "../chessengine/Board.hpp"
#include <stdlib.h>

#define TT_MEMORY_SIZE 1000000000 // in bytes
#define TT_MODULO_MASK

typedef struct MinMaxNode
{
        int   zobrist_key;
        int   depth;
        bool  leaf_node;
        float quality;

        vector<pair<Move, float>> child_move_quality_pairs;
} s_MinMaxNode;

class TranspositionTable
{
    public:
        TranspositionTable();

        s_MinMaxNode *get_node(int zobrist_key); // NULL if the node doesn't exist
        void          insert_node(int zobrist_key, s_MinMaxNode *node);

    private:
        int           _max_node_count;
        int           _node_size; // in bytes
        s_MinMaxNode *_table;

        void collision_handler(s_MinMaxNode *new_node, s_MinMaxNode *node);
};

#endif
