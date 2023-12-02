#include "../chessengine/Move.hpp"
#include <vector>

class RandomAgent {

    public:
        RandomAgent();
        Move choose_from(vector<Move> moves);
};
