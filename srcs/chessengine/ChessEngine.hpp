
#ifndef CHESSENGINE_HPP
# define CHESSENGINE_HPP

using namespace std;

# include <iostream>

# define BITBOARD_IMPLEMENTATION 1
# define CHESS960_RULES 1

# define EMPTY_CELL '.'

/* ENUMS */

# define GAME_CONTINUE -2
# define BLACK_WIN -1
# define DRAW 0
# define WHITE_WIN 1

// enum for Move type
enum castle_info_e
{
    NOINFO,
    NOTCASTLE,
    WHITELEFT,
    WHITERIGHT,
    BLACKLEFT,
    BLACKRIGHT
};


/* NOTATION FUNCTIONS */

inline int  column_name_to_index(char column_name)
{
    // 'a' -> 0
    return tolower(column_name) - 'a';
}

inline char column_index_to_name(int column_id)
{
    // 7 -> 'h'
    return column_id + 'a';
}

inline int  line_number_to_index(char line_number)
{
    // '8' -> 0
    return 8 - (line_number - '0');
}

inline char line_index_to_number(int line_index)
{
    // 7 -> '1'
    return (8 - line_index) + '0';
}

inline void algebraic_to_coord(string algebraic, int *x, int *y)
{
    // "a8" -> 0, 0
    const char *algebraic_char = algebraic.c_str();
    *x = column_name_to_index(algebraic_char[0]);
    *y = line_number_to_index(algebraic_char[1]);
}

inline string coord_to_algebraic(int x, int y)
{
    // 7, 7 -> "h1"
    char algebraic[2] = {column_index_to_name(x), line_index_to_number(y)};
    return string(algebraic);
}

inline uint64_t algebraic_to_bitboard(string algebraic)
{
    // TODO: Hardcode all possibilities for faster execution
    // "a8" -> ...001011...
    int x, y;
    algebraic_to_coord(algebraic, &x, &y);
    return 1UL << (y * 8 + x);
}

inline string bitboard_to_algebraic(uint64_t bitboard)
{
    // TODO: Hardcode all possibilities for faster execution
    // ...001011... -> "a8"
    int x = 0;
    int y = 0;
    uint64_t mask = 1UL;
    for (int i = 0; i < 64; i++)
    {
        if (bitboard & mask)
        {
            x = i % 8;
            y = i / 8;
            break;
        }

        mask <<= 1;
    }

    return coord_to_algebraic(x, y);
}

/* PIECES FUNCTIONS */

inline float get_piece_value(char piece)
{
    switch (tolower(piece))
    {
        case 'p':
            return 1;
        case 'n':
            return 3;
        case 'b':
            return 3.2;
        case 'r':
            return 5;
        case 'q':
            return 9;
        // case 'k':
        //     return 100;
        default:
            return 0;
    }
}

inline wchar_t convert_piece_to_unicode(char piece)
{
    switch (piece)
    {
        case 'P':
            return L'♟';
        case 'N':
            return L'♞';
        case 'B':
            return L'♝';
        case 'R':
            return L'♜';
        case 'Q':
            return L'♛';
        case 'K':
            return L'♚';
        case 'p':
            return L'♙';
        case 'n':
            return L'♘';
        case 'b':
            return L'♗';
        case 'r':
            return L'♖';
        case 'q':
            return L'♕';
        case 'k':
            return L'♔';
        default:
            return piece;
    }
}

#endif
