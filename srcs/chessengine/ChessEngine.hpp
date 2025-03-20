
#ifndef CHESSENGINE_HPP
#define CHESSENGINE_HPP

using namespace std;

#include <bits/stdc++.h>
#include <iostream>
#include <locale>
#include <stdio.h>
#include <string.h>
#include <vector>

#define BITBOARD_IMPLEMENTATION 0
#define CHESS960_RULES          1

#define PRINT_ENGINE_DATA 0
#define PRINT_TURNS       0
#define PRINT_ATTACKS     0

#define EMPTY_CELL '-'

// Bit masks representing chess board columns and lines, using UCI notation
#define BITMASK_ALL_CELLS   0xFFFFFFFFFFFFFFFFUL
#define BITMASK_WHITE_CELLS 0xAA55AA55AA55AA55UL
#define BITMASK_BLACK_CELLS 0x55AA55AA55AA55AAUL

#define BITMASK_LINE_81 0xFF000000000000FFUL
#define BITMASK_LINE_7  0x000000000000FF00UL
#define BITMASK_LINE_65 0x00000000FFFF0000UL
#define BITMASK_LINE_43 0x0000FFFF00000000UL
#define BITMASK_LINE_2  0x00FF000000000000UL

#define BITMASK_CASTLE_BLACK_LEFT_KING  0x0000000000000004UL
#define BITMASK_CASTLE_BLACK_LEFT_ROOK  0x0000000000000008UL
#define BITMASK_CASTLE_BLACK_RIGHT_KING 0x0000000000000040UL
#define BITMASK_CASTLE_BLACK_RIGHT_ROOK 0x0000000000000020UL
#define BITMASK_CASTLE_WHITE_LEFT_KING  0x0400000000000000UL
#define BITMASK_CASTLE_WHITE_LEFT_ROOK  0x0800000000000000UL
#define BITMASK_CASTLE_WHITE_RIGHT_KING 0x4000000000000000UL
#define BITMASK_CASTLE_WHITE_RIGHT_ROOK 0x2000000000000000UL

/* ENUMS */

#define GAME_CONTINUE -2
#define BLACK_WIN     -1
#define DRAW          0
#define WHITE_WIN     1

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

// enum for sliding pieces rays direction
enum ray_dir_e
{
    NORTH,
    NORTHEAST,
    EAST,
    SOUTHEAST,
    SOUTH,
    SOUTHWEST,
    WEST,
    NORTHWEST
};

/* PIECES FUNCTIONS */

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

/* NOTATION FUNCTIONS */

inline int column_name_to_index(char column_name)
{
    // 'a' -> 0
    return tolower(column_name) - 'a';
}

inline char column_index_to_name(int column_id)
{
    // 7 -> 'h'
    return column_id + 'a';
}

inline int line_number_to_index(char line_number)
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
    int      x = 0;
    int      y = 0;
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

/* BITWISE OPERATIONS */

inline uint64_t _count_bits(uint64_t bitboard)
{
    // __builtin_popcountll() returns the number of set bits
    return __builtin_popcountll(bitboard);
}

inline uint64_t _count_trailing_zeros(uint64_t bitboard)
{
    // __builtin_ctzll() returns the number of trailing zeros in the bitboard
    // (Zeros on the right)
    return __builtin_ctzll(bitboard);
}

inline uint64_t _count_leading_zeros(uint64_t bitboard)
{
    // __builtin_clzll() returns the number of leading zeros in the bitboard
    // (Zeros on the left)
    return 63 - __builtin_clzll(bitboard);
}

inline uint64_t _get_least_significant_bit(uint64_t bitboard)
{
    return 1UL << _count_trailing_zeros(bitboard);
}

inline uint64_t _get_most_significant_bit(uint64_t bitboard)
{
    return 1UL << _count_leading_zeros(bitboard);
}

#endif
