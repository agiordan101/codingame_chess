
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

inline int  COLUMN_name_to_index(char column_name)
{
    return tolower(column_name) - 97;
}

inline void algebraic_to_coord(string algebraic, int &x, int &y)
{
    // a1 -> 0, 0
    return ;
}

inline string coord_to_algebraic(int x, int y)
{
    // 0, 0 -> a1
    return string();
}

#endif
