#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

/**

Board representation:

    12 * 64-bits vars for each pieces :
        - 6 black :
            - pawns, knights, bishops, rooks, queens, king
        - 6 white
            - pawns, knights, bishops, rooks, queens, king

    C++ classes is ok

    blablabllablba

 **/

int main()
{
    cerr << "My chess bot is starting" << endl;

    int constants_count;
    cin >> constants_count; cin.ignore();
    for (int i = 0; i < constants_count; i++) {
        string name;
        string value;
        cin >> name >> value; cin.ignore();
    }

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << "fen" << endl;

    // game loop
    while (1) {
        string board;
        string color;
        string castling;
        string en_passant;
        int half_move_clock;
        int full_move;
        cin >> board >> color >> castling >> en_passant >> half_move_clock >> full_move; cin.ignore();

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        // cout << "random" << endl;
        cout << "e2e4" << endl;
    }
}

class Player {

    public:
        long long   pawns;
        long long   knights;
        long long   bishops;
        long long   rooks;
        long long   queens;
        long long   king;
        long long   board;
        bool        castle_k;
        bool        castle_q;

        // Player() {

        // }

};


class Board {

    Player      *players[2];
    Player      *white;
    Player      *black;
    string      fen;
    bool        player_turn;
    long long   en_passant;
    int         half_turn_remaning;
    int         game_turn;

    public:
        Board (string fen) {
            white = new Player();
            black = new Player();
            players[0] = white;
            players[1] = black;
            fen = fen;
            player_turn = true;
            en_passant = 0;
            half_turn_remaning = 50;
            game_turn = 1;
        }

    private:

        void parse_fen(string fen) {
            /*  
                rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w AHah - 0 1
            */
            long long   cell_mask = 1;

            int i = -1;
            while (fen[++i] != ' ') {
                
                if ('0' <= fen[i] && isdigit(fen[i]))
                    cell_mask <<= atoi(&fen[i]);

                switch (fen[i]) {
                    case 'p':
                        white->pawns |= cell_mask;
                    case 'n':
                        white->knights |= cell_mask;
                    case 'b':
                        white->bishops |= cell_mask;
                    case 'r':
                        white->rooks |= cell_mask;
                    case 'q':
                        white->queens |= cell_mask;
                    case 'k':
                        white->knights |= cell_mask;
                    case 'P':
                        black->pawns |= cell_mask;
                    case 'N':
                        black->knights |= cell_mask;
                    case 'B':
                        black->bishops |= cell_mask;
                    case 'R':
                        black->rooks |= cell_mask;
                    case 'Q':
                        black->queens |= cell_mask;
                    case 'K':
                        black->knights |= cell_mask;
                }

                cell_mask <<= 1;
            }
            i++;

            if (fen[i])
                player_turn = fen[i] == 'w';
            i += 2;

            for (int p = 0; p < 2; p++)
            {
                if (fen[i++] != '-')
                    players[p]->castle_k = true;
                if (fen[i++] != '-')
                    players[p]->castle_q = true;
            }

            
        }

};
