#ifndef BOTPLAYER_HPP
# define BOTPLAYER_HPP

#include "AbstractPlayer.hpp"

class BotPlayer: public AbstractPlayer {

    public:
        BotPlayer(AbstractAgent *agent);
        virtual Move choose_from(Board *board, vector<Move> moves) override;
    
    private:
        AbstractAgent* _agent;
};

#endif