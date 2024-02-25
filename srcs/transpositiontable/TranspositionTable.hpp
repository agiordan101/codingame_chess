#ifndef TRANSPOSITIONTABLE_HPP
# define TRANSPOSITIONTABLE_HPP

# include "../chessengine/Board.hpp"
# include <stdlib.h>

# define TT_MEMORY_SIZE 10000000 // in bytes
# define TT_MODULO_MASK 

typedef struct MinMaxNode
{
    // Primary information, filled the first time
    int zobrist_key;
    int depth;
    bool leaf_node;
    float quality;

    // Secondary information, filled the second time, while expanding the node
    vector<Move>   child_moves;
    vector<float>    child_qualities;
    vector<int>    child_zobrist_keys;

    // float alpha;
    // float beta;
    // string best_move; // Why ?
}               s_MinMaxNode;

class TranspositionTable
{
    private:
        int _max_node_count;
        int _node_size;     // in bytes
        s_MinMaxNode* _table;

        int _black_turn_hash;
        int _random_hashs[8][8][12];
        int _castling_right_hashs[16]; // 4 bits for each castling right = 2^4 = 16

        int _inserted_node_count;

        void    collision_handler(s_MinMaxNode* new_node, s_MinMaxNode* node);

    public:
        TranspositionTable();

        int             create_zobrist_key(Board *board);
        s_MinMaxNode*   get_node(int zobrist_key);   // NULL if the node doesn't exist
        void            insert_node(int zobrist_key, s_MinMaxNode* node);
        int             update_key(Board *board, Move *move, int zobrist_key);
};

#endif
