#include "GameEngine.hpp"

// --- PUBLIC METHODS ---

GameEngine::GameEngine(BotPlayer *player)
{
    this->_player = player;
    this->_cg_last_move = NULL;
    this->_cg_board = NULL;
    this->_board = NULL;
}

void GameEngine::infinite_game_loop()
{
    this->_parse_first_turn();
    cout << "lastmove fen" << endl;

    while (1)
    {
        _parse_turn();

        // First turns
        if (this->_cg_board->game_turn == 1)
        {
            this->_board = this->_cg_board->clone();
            cerr << "\nGameEngine: Initial board:" << endl;
            this->_board->log();
        }
        else
            this->_board->apply_move(*this->_cg_last_move);

        vector<Move> moves = this->_board->get_available_moves();

        Move move = this->_player->choose_from(this->_board, moves);

        this->_board->apply_move(move);

        float game_state = this->_board->get_game_state();
        if (game_state != GAME_CONTINUE)
        {
            cerr << "\nGameEngine: Game is over :" << game_state << endl;
            this->_board->log();
            delete this->_board;
        }

        cout << move.to_uci() << endl;
    }
}

// --- PRIVATE METHODS ---

void GameEngine::_parse_first_turn()
{
    int constants_count;
    string name;
    string value;
    bool    crazyHouse = false;
    int     maxMoves = 0;

    cin >> constants_count; cin.ignore();

    for (int i = 0; i < constants_count; i++) {
        cin >> name >> value; cin.ignore();
        
        if (name == "crazyHouse")
            crazyHouse = value == "1";
        else if (name == "maxMoves")
            maxMoves = stoi(value);
    }
}

void    GameEngine::_parse_turn()
{
    // Parse last move
    string move;
    cin >> move;

    this->_cg_last_move = move == "none" ? NULL : new Move(move);

    // Parse FEN
    string board;
    string color;
    string castling;
    string en_passant;
    string half_move_clock_str;
    string full_move_str;

    cin >> board >> color >> castling >> en_passant >> half_move_clock_str >> full_move_str;
    
    int half_move_clock = stoi(half_move_clock_str);
    int full_move = stoi(full_move_str);

    if (this->_cg_board)
        delete this->_cg_board;
    this->_cg_board = new Board(board, color, castling, en_passant, half_move_clock, full_move);

    cin.ignore();
}
