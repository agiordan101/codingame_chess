#include <iostream>

using namespace std;

class Move {

    public:
        int src_x;
        int src_y;
        int dst_x;
        int dst_y;
        bool castle;
        char promotion;
        bool en_passant;

        Move();
        Move(string uci, char src_piece, char dst_piece, bool chess960_rule = true);
        Move(int _src_x, int _src_y, int _dst_x, int _dst_y, bool _castle, char _promotion, bool _en_passant);
        
        void log();
        string to_uci(bool chess960_rule = true);

        bool operator ==(const Move *other);
};
