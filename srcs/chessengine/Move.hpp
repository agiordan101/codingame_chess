#include <iostream>

using namespace std;

#ifndef MOVE_HPP
# define MOVE_HPP

class Move {

    public:
        int src_x;
        int src_y;
        int dst_x;
        int dst_y;
        char promotion;
        bool is_castle;

        Move(string uci);
        Move(int _src_x, int _src_y, int _dst_x, int _dst_y, char _promotion, bool _is_castle = false);

        void log();
        string to_uci(bool regular_rules_castling = false);

        bool operator ==(const Move *other);
};

#endif
