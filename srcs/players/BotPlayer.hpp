#ifndef BOTPLAYER_HPP
# define BOTPLAYER_HPP

#include "AbstractPlayer.hpp"

class BotPlayer: public AbstractPlayer {

    public:
        BotPlayer(AbstractAgent *agent);
        Move choose_from(vector<Move> moves) override;
};

#endif