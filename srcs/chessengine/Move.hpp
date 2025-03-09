#ifndef MOVE_HPP
# define MOVE_HPP

# include "ChessEngine.hpp"

using namespace std;

# if BITBOARD_IMPLEMENTATION == 1

class Move {

    
    public:
        // UCI information
        string uci;
        uint64_t src;
        uint64_t dst;
        char promotion; // Piece created by the promotion (always lowercase, because its UCI representation is lowercase)
        
        // Extra information
        char            piece;         // Piece moved
        castle_info_e   castle_info;   // Castle information, for optimization and UCI generation

        Move(string _uci);
        Move(char _piece, uint64_t src, uint64_t dst, char _promotion = 0, castle_info_e _castle_info = NOTCASTLE);

        void log();
        string to_uci();
        string to_uci(bool chess960_rules, bool castling);

        bool operator ==(Move *other);

        static bool compare_move_vector(vector<Move> movelst1, vector<Move> movelst2);
        
    private:
        static bool _is_move_in_movelst(Move *move, vector<Move> movelst);
};

# else

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

        bool operator ==(Move *other);

        static bool compare_move_vector(vector<Move> movelst1, vector<Move> movelst2);
        
    private:
        static bool _is_move_in_movelst(Move *move, vector<Move> movelst);
};

# endif

#endif
