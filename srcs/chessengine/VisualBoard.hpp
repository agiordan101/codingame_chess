
#ifndef VISUALBOARD_HPP
# define VISUALBOARD_HPP

# include "ChessEngine.hpp"

class VisualBoard {

    public:
        char    board[8][8];

        VisualBoard();

        void    resetBoard();
        void    updateBoard(char piece, uint64_t bitboard);
        void    printRawBoard();
        void    printBoard();

        void    printSpecificBoard(char piece, uint64_t bitboard, string title = "");

        VisualBoard clone();
};

#endif
