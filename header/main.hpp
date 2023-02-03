# ifndef MAIN
# define MAIN

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <cstring>

using namespace std;

namespace ChessEngine {

    /* STRUCTURES */

    typedef enum _pieces_value {
        pawns_v = 1,
        knights_v = 3,
        bishops_v = 3,
        rooks_v = 5,
        queens_v = 9,
        king_v = 42
    }           e_pieces_value;

    /* STATIC FUNCTIONS */

    inline int     COLUMN_name_to_index(char column_name)
    {
        return tolower(column_name) - 97;
    }

    /* CLASSES */

    class Move {

        int src_x;
        int src_y;
        int dst_x;
        int dst_y;
        bool castle;
        bool promotion;
        bool en_passant;

        public:
            
            bool operator ==(const Move &other) const
            {
                return this->src_x == other.src_x && this->src_y == other.src_y && this->dst_x == other.dst_x && this->dst_y == other.dst_y;
            }
    };
        
    class Board {
        /*
            Board represent all FEN data
            White =  1 = Upper case
            Black = -1 = Lower case
        */

        int         board[8][8];
        int         player_turn;
        int         castles[4];     // 2 first for White | 2 last for Black
        string      en_passant;
        int         half_turn_remaning;
        int         game_turn;
        ChessEngine::Move   moves[100];
        int                 moves_count;

        public:

            Board(string _board, string _color, string _castling, string _en_passant, int _half_move_clock, int _full_move);
            void    log();
            void    show_board();
            void    reset_board(string new_fen_board);

            void    find_moves();
            void    apply_move(int src_x, int src_y, int dst_x, int dst_y, bool castle, int promotion, bool en_passant);
            
            bool    operator ==(string fen_board);

        private:

            string  _fen_board; 
            void    _parse_board(string fen_board);
            void    _parse_castling(string castling_fen);

    };

}

using namespace ChessEngine;

#endif
