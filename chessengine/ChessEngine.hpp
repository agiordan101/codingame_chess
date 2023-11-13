
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
    // a -> 0
    return tolower(column_name) - 'a';
}

inline char column_index_to_name(int column_id)
{
    // 0 -> a
    return column_id + 'a';
}

inline void algebraic_to_coord(string algebraic, int *x, int *y)
{
    // a1 -> 0, 0
    const char *algebraic_char = algebraic.c_str();
    *x = column_name_to_index(algebraic_char[0]);
    *y = algebraic_char[1];
}

inline string coord_to_algebraic(int x, int y)
{
    // 0, 0 -> a1
    char algebraic[2] = {column_index_to_name(x), (char)(y + 'a')};
    return string(algebraic);
}

#endif
