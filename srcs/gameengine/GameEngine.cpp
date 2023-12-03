#include "GameEngine.hpp"

GameEngine::GameEngine(Board* board, AbstractPlayer* white_player, AbstractPlayer* black_player)
{
    this->_board = board;
    this->_white_player = white_player;
    this->_black_player = black_player;
    this->players[0] = white_player;
    this->players[1] = black_player;
    this->player_turn = 0;
}

float GameEngine::start_games(int n_games)
{
    float white_score = 0;
    for (int i = 0; i < n_games; i++)
    {
        float game_score = this->game_loop();
        cerr << "Game " << i << " - White score: " << game_score << endl;

        white_score += game_score;
    }

    return white_score;
}

float GameEngine::game_loop()
{
    Board *current_board = this->_board->clone();

    // -1 = Game continue | 0 = Black win | 0.5 = Draw | 1 = White win
    float game_state = current_board->game_state();
    while (game_state == -1)
    {
        vector<Move> moves = current_board->find_moves();

        Move move = this->players[this->player_turn]->choose_from(moves);

        current_board->apply_move(move);

        this->player_turn = (this->player_turn + 1) % 2;
        game_state = current_board->game_state();
    }

    return game_state;
}
