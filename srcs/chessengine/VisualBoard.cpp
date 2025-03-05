#include "VisualBoard.hpp"

VisualBoard::VisualBoard() {
    std::locale::global(std::locale("C.UTF-8"));
    std::wcout.imbue(std::locale("C.UTF-8"));

    resetBoard();
}

void VisualBoard::resetBoard() {
    // Initialize the board with empty spaces
    memset(board, EMPTY_CELL, 64);
}

void VisualBoard::updateBoard(char piece, uint64_t bitboard) {

    uint64_t pos_mask = 1UL;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (bitboard & pos_mask)
            {
                if (board[y][x] != EMPTY_CELL)
                    board[y][x] = 'X';
                else
                    board[y][x] = piece;
            }
            pos_mask <<= 1;
        }
    }
}

void VisualBoard::printBoard() {

    cerr << "----+----------------" << endl;
    cerr << "yx  | 0 1 2 3 4 5 6 7" << endl;
    cerr << " uci| A B C D E F G H" << endl;
    cerr << "----+----------------" << endl;
    for (int y = 0; y < 8; y++)
    {
        wcout << y << " " << line_index_to_number(y) << " |";
        for (int x = 0; x < 8; x++)
            wcout << " " << convert_piece_to_unicode(board[y][x]);
        wcout << endl;
    }
    cerr << "----+----------------" << endl;
}

void VisualBoard::printSpecificBoard(char piece, uint64_t bitboard, string title)
{
    if (title != "")
        cerr << "VisualBoard title : " << title << endl;

    VisualBoard board = VisualBoard();
    board.updateBoard(piece, bitboard);
    board.printBoard();
}

VisualBoard VisualBoard::clone() {
    VisualBoard new_board;
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            new_board.board[y][x] = board[y][x];
    return new_board;
}
