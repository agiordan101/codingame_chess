#include "header/main.hpp"

// To debug: cerr << "\tChessBot: Debug messages..." << endl;

/* CONSTANTS */

bool    crazyHouse = false;
int     maxMoves = 0;
bool    DRAW = false;
string  LASTMOVE = "";

/* PARSING FUNCTIONS */

void parse_first_turn() {

    int constants_count;
    string name;
    string value;

    cin >> constants_count; cin.ignore();

    for (int i = 0; i < constants_count; i++) {
        cin >> name >> value; cin.ignore();
        
        if (name == "crazyHouse")
            crazyHouse = value == "1";
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
    cin.ignore();
    // cin >> LASTMOVE;
    // cin.ignore();
    // cin >> DRAW;
    // cin.ignore();

    cerr << "\tChessBot: board = " << board << endl;
    cerr << "\tChessBot: color = " << color << endl;
    cerr << "\tChessBot: castling = " << castling << endl;
    cerr << "\tChessBot: en_passant = " << en_passant << endl;
    cerr << "\tChessBot: half_move_clock = " << half_move_clock << endl;
    cerr << "\tChessBot: full_move = " << full_move << endl;
    // cerr << "\tChessBot: lastMove = " << lastMove << endl;
    // cerr << "\tChessBot: draw = " << draw << endl;
    Board *b = new Board(board, color, castling, en_passant, half_move_clock, full_move);
    return b;
}

int main()
{
    cerr << "\tChessBot: My chess bot is starting !...!" << endl;
    parse_first_turn();

    // Set requested inputs for following turns 
    cout << "fen" << endl;
    // cout << "fen lastmove draw" << endl;

    while (1) {
        Board *board = parse_following_turns();
        board->log();

        cout << "random" << endl;
        // exit(0);
    }
}
