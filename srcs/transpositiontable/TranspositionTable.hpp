#ifndef TRANSPOSITIONTABLE_HPP
#define TRANSPOSITIONTABLE_HPP

#include "../chessengine/Board.hpp"
#include <stdlib.h>

#define TT_MEMORY_SIZE 2000000000 // in bytes

typedef struct MinMaxNode
{
        int    zobrist_key;
        string last_position_fen;
        int    game_turn;
        bool   leaf_node;
        float  quality;

        vector<pair<Move, float>> child_move_quality_pairs;

        float alpha; // First time we create a node, some of its cousins are not created yet
        float beta;  // Se alpha beta values can be more aggresive in next visits
} s_MinMaxNode;

class TranspositionTable
{
    public:
        TranspositionTable();

        s_MinMaxNode *get_node(int zobrist_key);

    private:
        int           _max_node_count;
        int           _node_size; // in bytes
        s_MinMaxNode *_table;
};

#endif
