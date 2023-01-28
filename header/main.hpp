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

    typedef enum _pieces {
        pawns = 1,
        knights = 3,
        bishops = 3,
        rooks = 5,
        queens = 9,
        king = 42
    }           e_pieces;

    /* CLASSES */

    class Move {

        int src_x;
        int src_y;
        int dst_x;
        int dst_y;

        public:
            
            bool operator ==(const Move &other) const
            {
                return this->src_x == other.src_x && this->src_y == other.src_y && this->dst_x == other.dst_x && this->dst_y == other.dst_y;
            }
    };

    /* STATIC FUNCTIONS */

    int     PIECE_letter_to_value(char piece)
    {
        switch (piece) {
            case 'p':
                return pawns;
                break ;
            case 'n':
                return knights;
                break ;
            case 'b':
                return bishops;
                break ;
            case 'r':
                return rooks;
                break ;
            case 'q':
                return queens;
                break ;
            case 'k':
                return king;
                break ;
            case 'P':
                return -pawns;
                break ;
            case 'N':
                return -knights;
                break ;
            case 'B':
                return -bishops;
                break ;
            case 'R':
                return -rooks;
                break ;
            case 'Q':
                return -queens;
                break ;
            case 'K':
                return -king;
                break ;
            default:
                return 0;
        }
    }

    int     COLUMN_name_to_index(char column_name)
    {
        return tolower(column_name) - 97;
    }

}



/*
    CLASSES
*/

class Board {
    /*
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

    private:

        void    _parse_board(string fen_board);
        void    _parse_castling(string castling_fen);

};

#endif
