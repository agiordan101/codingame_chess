#include "GameEngine.hpp"

// --- PUBLIC METHODS ---

GameEngine::GameEngine(BotPlayer *player)
{
    this->_player = player;
    this->_board = NULL;
}

void GameEngine::infinite_game_loop()
{
    this->_parse_first_turn();
    cout << "lastmove fen" << endl;

    // float elapsed_time;
    while (1)
    {
        _parse_turn();

        // elapsed_time = (float)(clock() - this->_turn_clock_start) / CLOCKS_PER_SEC * 1000;
        // cerr << "\nGameEngine: After turn parsing: " << elapsed_time << "/50 ms" << endl;

        // // First turns
        // if (this->_cg_board->game_turn == 1)
        // {
        //     this->_board = this->_cg_board->clone();
        //     // cerr << "\nGameEngine: Initial board:" << endl;
        //     // this->_board->log();
        // }
        // else
        //     this->_board->apply_move(*this->_cg_last_move);

        // elapsed_time = (float)(clock() - this->_turn_clock_start) / CLOCKS_PER_SEC * 1000;
        // cerr << "\nGameEngine: Before move generation: " << elapsed_time << "/50 ms" << endl;

        vector<Move> moves = this->_board->get_available_moves();

        // elapsed_time = (float)(clock() - this->_turn_clock_start) / CLOCKS_PER_SEC * 1000;
        // cerr << "\nGameEngine: Before algorithm: " << elapsed_time << "/50 ms" << endl;

        Move move = this->_player->choose_from(this->_board, moves);

        // elapsed_time = (float)(clock() - this->_turn_clock_start) / CLOCKS_PER_SEC * 1000;
        // cerr << "\nGameEngine: After algorithm: " << elapsed_time << "/50 ms" << endl;

        vector<string> stats = this->_player->get_stats();

        cout << move.to_uci();

        for (string stat : stats)
            cout << " " << stat;
        cout << endl;

        // clock_t turn_clock_end = clock();
        // elapsed_time = (float)(clock() - this->_turn_clock_start) / CLOCKS_PER_SEC * 1000;
        // cerr << "\nGameEngine: Turn duration: " << elapsed_time << "/50 ms" << endl;

        // if (elapsed_time > 50)
        // {
        //     cerr << "\nGameEngine: Turn duration is too long !" << endl;
        //     exit(1);
        // }

        this->_board->apply_move(move);

        float game_state = this->_board->get_game_state();
        if (game_state != GAME_CONTINUE)
        {
            cerr << "\nGameEngine: Game is over : " << game_state << endl;
            this->_board->log(true);
        }
    }
}

// --- PRIVATE METHODS ---

void GameEngine::_parse_first_turn()
{
    int    constants_count;
    string name;
    string value;
    bool   crazy_house = false;
    int    max_moves = 0;

    cin >> constants_count;
    cin.ignore();

    for (int i = 0; i < constants_count; i++)
    {
        cin >> name >> value;
        cin.ignore();

        if (name == "crazyHouse")
            crazy_house = value == "1";
        else if (name == "maxMoves")
            max_moves = stoi(value);
    }

    this->crazy_house = crazy_house;
    this->max_moves = max_moves;
}

void GameEngine::_parse_turn()
{
    // Parse last move
    string move;
    cin >> move;

    // Parse FEN
    string board;
    string color;
    string castling;
    string en_passant;
    string half_move_clock_str;
    string full_move_str;

    cin >> board >> color >> castling >> en_passant >> half_move_clock_str >> full_move_str;

    this->_turn_clock_start = clock();

    int half_move_clock = stoi(half_move_clock_str);
    int full_move = stoi(full_move_str);

    if (full_move == 1)
        this->_board = new Board(board, color, castling, en_passant, half_move_clock, full_move);
    else
        this->_board->apply_move(Move(move));

    cin.ignore();
}
