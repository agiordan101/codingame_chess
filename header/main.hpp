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

    typedef enum _pieces_num {
        pawns_n = 1,
        knights_n = 2,
        bishops_n = 3,
        rooks_n = 4,
        queens_n = 5,
        king_n = 6
    }           e_pieces_num;

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

    /* STATIC FUNCTIONS */

    inline int     PIECE_letter_to_value(char piece)
    {
        switch (piece) {
            case 'p':
                return pawns_n;
                break ;
            case 'n':
                return knights_n;
                break ;
            case 'b':
                return bishops_n;
                break ;
            case 'r':
                return rooks_n;
                break ;
            case 'q':
                return queens_n;
                break ;
            case 'k':
                return king_n;
                break ;
            case 'P':
                return -pawns_n;
                break ;
            case 'N':
                return -knights_n;
                break ;
            case 'B':
                return -bishops_n;
                break ;
            case 'R':
                return -rooks_n;
                break ;
            case 'Q':
                return -queens_n;
                break ;
            case 'K':
                return -king_n;
                break ;
            default:
                return 0;
        }
    }

    inline int     COLUMN_name_to_index(char column_name)
    {
        return tolower(column_name) - 97;
    }

}



/*
    CLASSES
*/

class Board {
    /*
        Board represent all FEN data
        White = 1
        Black = -1
    */

    int         board[8][8];
    int         player_turn;
    int         castles[4];     // 2 first for White | 2 last for Black
    string      en_passant;
    int         half_turn_remaning;
    int         game_turn;

    public:

        Board(string _board, string _color, string _castling, string _en_passant, int _half_move_clock, int _full_move);
        void    log();
        int     apply_move(int src_x, int src_y, int dst_x, int dst_y, bool castle, ChessEngine::e_pieces_num promotion, bool en_passant);

    private:

        void    _parse_board(string fen_board);
        void    _parse_castling(string castling_fen);

};

#endif
