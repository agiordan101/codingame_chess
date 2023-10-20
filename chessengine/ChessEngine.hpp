
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

inline int     COLUMN_name_to_index(char column_name)
{
    return tolower(column_name) - 97;
}

#endif
