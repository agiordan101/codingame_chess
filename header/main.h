#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <cstring>

using namespace std;

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

        Board(string _board, string _color, string _castling, string _en_passant, int _half_move_clock, int _full_move) {
            /*  
                White - upper case
                Black - lower case
                rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1
            */
            parse_board(_board);
            player_turn = _color == "w" ? 1 : -1;
            parse_castling(_castling);
            en_passant = _en_passant;
            half_turn_remaning = _half_move_clock;
            game_turn = _full_move;
        }

        void log() {
            cerr << "\n\tChessBot: Board description " << board << endl;
            // for (int y = 0; y < 8; y++)
            //     for (int x = 0; x < 8; x++)
            //         cerr << " " << board[y][x] << endl;
            cerr << "\tChessBot: Board: Color w=1|b=0: " << player_turn << endl;
            cerr << "\tChessBot: Board: Castling: w " << castles[0] << " | " << castles[1] << " | b " << castles[2] << " | " << castles[3] << endl;
            cerr << "\tChessBot: Board: En passant: " << en_passant << endl;
            cerr << "\tChessBot: Board: half_turn_remaning: " << half_turn_remaning << endl;
            cerr << "\tChessBot: Board: game_turn: " << game_turn << endl;
        }

    private:
        typedef enum _pieces {
            pawns = 1,
            knights = 3,
            bishops = 3,
            rooks = 5,
            queens = 9,
            king = 42
        }           e_pieces;

        int     _piece_to_value(char piece)
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

        int     _to_column_nbr(char column_name)
        {
            return tolower(column_name) - 97;
        }

        void    parse_board(string fen_board) {

            int cell_i = 0;

            memset(board, 0, sizeof(int) * 64);
            for (int i = 0; i < fen_board.length(); i++) {

                int piece = fen_board[i];
                
                if (isdigit(fen_board[i]))
                    cell_i += atoi(&fen_board[i]);
                else if (piece != '/')
                {
                    board[cell_i / 8][cell_i % 8] = _piece_to_value(piece);
                    cerr << cell_i / 8 << " | " << cell_i % 8 << " -> " << (char)(piece) << " = " << board[cell_i / 8][cell_i % 8] << endl;
                    cell_i++;
                }
            }
        }

        void    parse_castling(string castling_fen)
        {
            int _castles[4] = {-1, -1, -1, -1};
            int white_castles_i = 0;
            int black_castles_i = 0;

            for (int i = 0; i < castling_fen.length(); i++)
            {
                if (islower(castling_fen[i]))
                    _castles[white_castles_i++] = _to_column_nbr(castling_fen[i]);
                if (isupper(castling_fen[i]))
                    _castles[2 + black_castles_i++] = _to_column_nbr(castling_fen[i]);
            }
            memcpy(castles, _castles, sizeof(int) * 4);
            cerr << "Castle parsing end for: " << castling_fen << endl;
        }

};
