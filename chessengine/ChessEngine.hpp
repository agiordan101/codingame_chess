
#ifndef ChessEngine
# define ChessEngine

#include <iostream>

using namespace std;

/* STRUCTURES */

typedef enum _pieces_value {
    pawns_v = 1,
    knights_v = 3,
    bishops_v = 3,
    rooks_v = 5,
    queens_v = 9,
    king_v = 42
}           e_pieces_value;

/* STATIC FUNCTIONS */

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
    // a8 -> 0, 0
    const char *algebraic_char = algebraic.c_str();
    *x = column_name_to_index(algebraic_char[0]);
    *y = line_number_to_index(algebraic_char[1]);
}

inline string coord_to_algebraic(int x, int y)
{
    // 7, 7 -> h1
    char algebraic[2] = {column_index_to_name(x), line_index_to_number(y)};
    return string(algebraic);
}

#endif
