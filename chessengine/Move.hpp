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
        Move(int _src_x, int _src_y, int _dst_x, int _dst_y, bool _castle, char _promotion, bool _en_passant);
        
        void log();

        bool operator ==(const Move &other);
};
