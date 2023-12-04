#include "GameEngine.hpp"

GameEngine::GameEngine(Board* board, AbstractPlayer* white_player, AbstractPlayer* black_player)
{
    this->_board = board;
    this->_white_player = white_player;
    this->_black_player = black_player;
    this->_players[0] = white_player;
    this->_players[1] = black_player;
    this->_player_turn = 0;
}

float GameEngine::start_games(int n_games, int progress_bar_size)
{
    float result = 0;
    float white_win = 0;
    float black_win = 0;
    float draws = 0;
    for (int i = 1; i <= n_games; i++)
    {
        result = this->game_loop();
        // cerr << "Game " << i << " - White win: " << result << endl;

        if (result == 0)
            black_win++;
        else if (result == 1)
            white_win++;
        else
            draws++;

        // Calculate percentages
        int white_percentage = (white_win / i) * progress_bar_size;
        int black_percentage = (black_win / i) * progress_bar_size;
        int draw_percentage = progress_bar_size - white_percentage - black_percentage;

        // Print progress bar
        cerr << "Game " << i << " - ["
             << string(white_percentage, 'W') 
             << string(draw_percentage, ' ') 
             << string(black_percentage, 'B') 
             << "] - "
             << white_win << "/" << draws << "/" << black_win
             << endl;
    }

    return white_win;
}

float GameEngine::game_loop()
{
    Board *current_board = this->_board->clone();

    this->_player_turn = current_board->is_white_turn() ? 0 : 1;

    // -1 = Game continue | 0 = Black win | 0.5 = Draw | 1 = White win
    float game_state = current_board->game_state();
    while (game_state == -1)
    {
        vector<Move> moves = current_board->get_available_moves();

        Move move = this->_players[this->_player_turn]->choose_from(current_board, moves);

        current_board->apply_move(move);
        // current_board->log();

        this->_player_turn = (this->_player_turn + 1) % 2;
        game_state = current_board->game_state();
    }

    delete current_board;
    return game_state;
}
