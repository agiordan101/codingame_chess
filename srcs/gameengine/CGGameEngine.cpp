#include "CGGameEngine.hpp"

// --- PUBLIC METHODS ---

CGGameEngine::CGGameEngine(BotPlayer *player)
{
    this->_player = player;
    this->_cg_last_move = NULL;
    this->_cg_board = NULL;
    this->_board = NULL;
}

void CGGameEngine::infinite_game_loop()
{
    this->_parse_first_turn();
    cout << "lastmove fen" << endl;
    // cout << "lastmove fen moves" << endl;

    while (1)
    {
        // cerr << "\nCGGameEngine: " << this->_player->get_name() << endl;
        _parse_turn();

        // First turns
        if (this->_cg_board->game_turn == 1)
        {
            this->_board = this->_cg_board->clone();
            cerr << "\nCGGameEngine: loop: Initial board:" << endl;
            this->_board->log();
        }
        else
        {
            this->_board->apply_move(*this->_cg_last_move);
            // cerr << "\nCGGameEngine: loop: Board AFTER last move: " << this->_cg_last_move->to_uci() << endl;
            // this->_board->log();
        }

        if (this->_fen && !(*this->_board == this->_cg_board))
        {
            cerr << "\nCGGameEngine: loop: Board is not the same as the one from CG: " << endl;
            cerr << "CGGameEngine: loop: Board from CG:" << endl;
            this->_cg_board->log();
            cerr << "CGGameEngine: loop: Board from ChessEngine:" << endl;
            this->_board->log();
            this->_board->log_history(3);

            exit(1);
        }

        vector<Move> moves = this->_board->get_available_moves();

        // if (this->_moves)
        // {
        //     if (Move::compare_move_vector(moves, this->_possible_moves))
        //         cerr << "CGGameEngine: loop: " << moves.size() << " legal moves were correctly found by the engine" << endl;
        //     else
        //     {
        //         cerr << "CGGameEngine: loop: Vector1: My chess board | Vector2: CG moves" << endl;
        //         this->_board->log();

        //         exit(1);
        //     }
        // }

        Move move = this->_player->choose_from(this->_board, moves);
        // cerr << "Choosen move: " << move.to_uci() << endl;

        this->_board->apply_move(move);

        // cerr << "\nCGGameEngine: loop: Board AFTER applying the choosen move: " << move.to_uci() << endl;
        // this->_board->log();

        cout << move.to_uci() << endl;

        // float game_state = this->_board->get_game_state();
        // if (game_state != GAME_CONTINUE)
        // {
        //     cerr << "CGGameEngine: loop: Game is over :" << game_state << endl;
        //     delete this->_board;
        // }
    }
}

// --- PRIVATE METHODS ---

void CGGameEngine::_parse_first_turn()
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
    // cerr << "CGGameEngine: first turn: crazyHouse = " << crazyHouse << endl;
    // cerr << "CGGameEngine: first turn: maxMoves = " << maxMoves << endl;
}

void    CGGameEngine::_parse_turn()
{
    // Parse last move
    string move;
    cin >> move;
    // cerr << "CGGameEngine: parsing: last_move = " << move << endl;

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

    // cerr << "CGGameEngine: parsing: board = " << board << endl;
    // cerr << "CGGameEngine: parsing: color = " << color << endl;
    // cerr << "CGGameEngine: parsing: castling = " << castling << endl;
    // cerr << "CGGameEngine: parsing: en_passant = " << en_passant << endl;
    // cerr << "CGGameEngine: parsing: half_move_clock_str = " << half_move_clock_str << endl;
    // cerr << "CGGameEngine: parsing: full_move_str = " << full_move_str << endl;

    if (this->_cg_board)
        delete this->_cg_board;
    this->_cg_board = new Board(board, color, castling, en_passant, half_move_clock, full_move);

    // Parse available moves
    // if (this->_moves)
    // {
    //     string move;
    //     string move_count;

    //     cin >> move_count;
    //     this->_possible_moves_count = stoi(move_count);

    //     this->_possible_moves = vector<Move>();
    //     for (int i = 0; i < this->_possible_moves_count; i++)
    //     {
    //         cin >> move;
    //         this->_possible_moves.push_back(Move(move));
    //     }
    // }

    cin.ignore();
}
