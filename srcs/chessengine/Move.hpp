#ifndef MOVE_HPP
# define MOVE_HPP

# pragma region Board

# include <iostream>
# include <vector>

# include "ChessEngine.hpp"

using namespace std;

class Move {

    public:
        int src_x;
        int src_y;
        int dst_x;
        int dst_y;
        char promotion;

        Move(string uci);
        Move(int _src_x, int _src_y, int _dst_x, int _dst_y, char _promotion);

        void log();
        string to_uci(bool regular_rules_castling = false);

        bool operator ==(const Move *other);

        static bool compare_move_vector(vector<Move> movelst1, vector<Move> movelst2);
        
    private:
        static bool _is_move_in_movelst(Move *move, vector<Move> movelst);
};

# pragma endregion Board

// --- BIT BOARD IMPLEMENTATION---

# pragma region BitBoard

// # include <iostream>
// # include <vector>

// # include "AbstractMove.hpp"
// # include "ChessEngine.hpp"

// using namespace std;

// class BitMove: public AbstractMove {

//     public:
//         char piece;
//         uint64_t src;
//         uint64_t dst;
//         char promotion;

//         BitMove(string uci);
//         BitMove(char piece, uint64_t src, uint64_t dst, char _promotion);

//         void log() override;
//         string to_uci(bool regular_rules_castling = false) override;
// };

# pragma endregion BitBoard

#endif
