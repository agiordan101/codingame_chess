#ifndef MOVE_HPP
# define MOVE_HPP

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

#endif
