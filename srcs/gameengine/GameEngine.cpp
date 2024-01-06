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
    float   result = 0;
    int     white_win = 0;
    int     black_win = 0;
    int     draws = 0;

    fprintf(
        stderr,
        "\nStarting %d games - White: %s - Black: %s\n",
        n_games,
        this->_white_player->get_name().c_str(),
        this->_black_player->get_name().c_str()
    );

    for (int i = 1; i <= n_games; i++)
    {
        result = this->game_loop();

        if (result == BLACK_WIN)
        {
            cerr << "B";
            black_win++;
        }
        else if (result == WHITE_WIN)
        {
            cerr << "W";
            white_win++;
        }
        else
        {
            cerr << "-";
            draws++;
        }
    }

    // Calculate percentages
    int white_percentage = ((float)white_win / n_games) * progress_bar_size;
    int black_percentage = ((float)black_win / n_games) * progress_bar_size;
    int draw_percentage = progress_bar_size - white_percentage - black_percentage;

    // Print results
    fprintf(
        stderr,
        "\nWDL results - [%s%s%s] - %d/%d/%d\n",
        string(white_percentage, 'W').c_str(), 
        string(draw_percentage, ' ').c_str(), 
        string(black_percentage, 'B').c_str(), 
        white_win, draws, black_win
    );

    return white_win;
}

float GameEngine::game_loop()
{
    Board *current_board = this->_board->clone();

    this->_player_turn = current_board->is_white_turn() ? 0 : 1;

    float game_state = current_board->get_game_state();
    while (game_state == GAME_CONTINUE)
    {
        vector<Move> moves = current_board->get_available_moves();

        Move move = this->_players[this->_player_turn]->choose_from(current_board, moves);

        current_board->apply_move(move);
        // current_board->log();

        this->_player_turn = (this->_player_turn + 1) % 2;
        game_state = current_board->get_game_state();
    }

    delete current_board;
    return game_state;
}
