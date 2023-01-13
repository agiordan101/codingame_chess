#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

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

        cout << "random" << endl;
    }
}
