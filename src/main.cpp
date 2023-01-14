#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
// #include <bitset>
// #include <climits>
#include <cstring>

// To debug: cerr << "\tChessBot: Debug messages..." << endl;

using namespace std;

/* CONSTANTS */

bool    crazyHouse = false;
int     maxMoves = 0;
bool    DRAW = false;
string  LASTMOVE = "";


/* CLASSES */

class Board {
    /*
        White = 1
        Black = -1
    */

    int         board[8][8];
    string      fen;
    int         player_turn;
    char        white_castle[2];
    int         white_castle_i;
    char        black_castle[2];
    int         black_castle_i;
    string      en_passant;
    int         half_turn_remaning;
    int         game_turn;

    public:

        Board(string board, string color, string castling, string en_passant, int half_move_clock, int full_move) {
            /*  
                Black - lower case
                White - upper case
                rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1
            */
            parse_board(board);
            player_turn = color == "w" ? 1 : -1;
            parse_castling(castling);
            en_passant = en_passant;
            half_turn_remaning = half_move_clock;
            game_turn = full_move;
        }

        void log() {
            cerr << "\tChessBot: Board: Board:" << endl;
            for (int y = 0; y < 8; y++)
            {
                for (int x = 0; x < 8; x++)
                    if (board[y][x] < 0)
                        cerr << " " + board[y][x];
                    else
                        cerr << "  " + board[y][x];
                cerr << endl;
            }
            cerr << "\tChessBot: Board: Color: " << (player_turn ? "White" : "Black") << endl;
            cerr << "\tChessBot: Board: Castling White: " << white_castle[0] << " | " << white_castle[1] << endl;
            cerr << "\tChessBot: Board: Castling Black: " << white_castle[0] << " | " << white_castle[1] << endl;
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

        int     parse_board(string fen) {

            int i = -1;
            int cell_i = 0;

            while (fen[++i] != ' ') {
                
                if (isdigit(fen[i]))
                    cell_i += atoi(&fen[i]);

                else
                    switch (fen[i]) {
                        case 'p':
                            board[cell_i / 8][cell_i % 8] = pawns;
                        case 'n':
                            board[cell_i / 8][cell_i % 8] = knights;
                        case 'b':
                            board[cell_i / 8][cell_i % 8] = bishops;
                        case 'r':
                            board[cell_i / 8][cell_i % 8] = rooks;
                        case 'q':
                            board[cell_i / 8][cell_i % 8] = queens;
                        case 'k':
                            board[cell_i / 8][cell_i % 8] = king;
                        case 'P':
                            board[cell_i / 8][cell_i % 8] = -pawns;
                        case 'N':
                            board[cell_i / 8][cell_i % 8] = -knights;
                        case 'B':
                            board[cell_i / 8][cell_i % 8] = -bishops;
                        case 'R':
                            board[cell_i / 8][cell_i % 8] = -rooks;
                        case 'Q':
                            board[cell_i / 8][cell_i % 8] = -queens;
                        case 'K':
                            board[cell_i / 8][cell_i % 8] = -king;
                    }
                cell_i++;
            }
            return i;
        }

        void    parse_castling(string castling)
        {
            white_castle_i = 0;
            black_castle_i = 0;
            memset(white_castle, '-', 2);
            memset(black_castle, '-', 2);
            for (int i = 0; i < castling.length(); i++)
            {
                if (islower(castling[i]))
                    black_castle[black_castle_i++] = castling[i];
                if (isupper(castling[i]))
                    white_castle[white_castle_i++] = castling[i];
            }
        }

};

/* PARSING FUNCTIONS */

void parse_first_turn() {

    int constants_count;
    cin >> constants_count; cin.ignore();

    for (int i = 0; i < constants_count; i++) {
        string name;
        string value;
        cin >> name >> value; cin.ignore();
        
        if (name == "crazyHouse")
            crazyHouse = value == "true";
        else if (name == "maxMoves")
            maxMoves = stoi(value);
    }
    cerr << "\tChessBot: crazyHouse = " << crazyHouse << endl;
    cerr << "\tChessBot: maxMoves = " << maxMoves << endl;
}

Board   *parse_following_turns() {

    string board;
    string color;
    string castling;
    string en_passant;
    int half_move_clock;
    int full_move;

    cin >> board >> color >> castling >> en_passant >> half_move_clock >> full_move;
    //cin.ignore();
    cin >> LASTMOVE;
    //cin.ignore();
    cin >> DRAW;
    //cin.ignore();

    return new Board(board, color, castling, en_passant, half_move_clock, full_move);
    // cerr << "\tChessBot: board = " << board << endl;
    // cerr << "\tChessBot: color = " << color << endl;
    // cerr << "\tChessBot: castling = " << castling << endl;
    // cerr << "\tChessBot: en_passant = " << en_passant << endl;
    // cerr << "\tChessBot: half_move_clock = " << half_move_clock << endl;
    // cerr << "\tChessBot: full_move = " << full_move << endl;
    // cerr << "\tChessBot: lastMove = " << lastMove << endl;
    // cerr << "\tChessBot: draw = " << draw << endl;
}


int main()
{
    cerr << "\tChessBot: My chess bot is starting !...!" << endl;
    parse_first_turn();

    // Set requested inputs for following turns 
    cout << "fen lastmove draw" << endl;

    while (1) {
        Board *board = parse_following_turns();
        board->log();

        cout << "random" << endl;
    }
}
