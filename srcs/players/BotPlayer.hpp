#ifndef BOTPLAYER_HPP
# define BOTPLAYER_HPP

#include "AbstractPlayer.hpp"

class BotPlayer: public AbstractPlayer {

    public:
        BotPlayer(string agent_name);
        Move choose_from(vector<Move> moves) override;
};

#endif