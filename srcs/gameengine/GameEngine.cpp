#include "GameEngine.hpp"

// --- PUBLIC METHODS ---

GameEngine::GameEngine(AbstractAgent *agent)
{
    this->_agent = agent;
    this->_board = NULL;
}

void GameEngine::infinite_game_loop()
{
    this->_parse_first_turn();
    cout << "lastmove fen" << endl;

    while (1)
    {
        _parse_turn();

        vector<Move> moves = this->_board->get_available_moves();

        Move move = this->_agent->choose_from(this->_board, moves);
        cout << move.to_uci();

        vector<string> stats = this->_agent->get_stats();
        for (string stat : stats)
            cout << " " << stat;
        cout << endl;

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
