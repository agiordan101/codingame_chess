#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
// #include <bitset>
// #include <climits>
#include <cstring>

using namespace std;

#include "../header/main.h"

int main()
{
    
    string color;
    string castling;
    string en_passant;
    int half_move_clock;
    int full_move;
    string fen_board;
    
    fen_board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    color = "w";
    castling = "ahAH";
    en_passant = "e2e4";
    half_move_clock = 0;
    full_move = 1;

    Board *board = new Board(fen_board, color, castling, en_passant, half_move_clock, full_move);
    board->log();
}