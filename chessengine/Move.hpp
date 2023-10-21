
class Move {

    public:
        int src_x;
        int src_y;
        int dst_x;
        int dst_y;
        bool castle;
        bool promotion;
        bool en_passant;

        bool operator ==(const Move &other);
};
