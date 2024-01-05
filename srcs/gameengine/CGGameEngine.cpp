#include "CGGameEngine.hpp"

// --- PUBLIC METHODS ---

CGGameEngine::CGGameEngine(BotPlayer *player, bool lastmove, bool fen, bool moves, bool draw, bool game, bool score)
{
    this->_player = player;
    this->_lastmove = lastmove;
    this->_fen = fen;
    this->_moves = moves;
    this->_draw = draw;
    this->_game = game;
    this->_score = score;

    this->_last_move = NULL;
    this->_board = NULL;
    this->_cg_board = NULL;
}

void CGGameEngine::parse_first_turn()
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
    cerr << "CGGameEngine: first turn: crazyHouse = " << crazyHouse << endl;
    cerr << "CGGameEngine: first turn: maxMoves = " << maxMoves << endl;
}

void CGGameEngine::set_requested_inputs()
{
    // Set requested inputs for following turns
    string requested_inputs = "";

    if (this->_lastmove)
        requested_inputs += "lastmove ";
    if (this->_fen)
        requested_inputs += "fen ";
    if (this->_moves)
        requested_inputs += "moves ";
    // if (this->_draw)
    //     requested_inputs += "draw ";
    // if (this->_game)
    //     requested_inputs += "game ";
    // if (this->_score)
    //     requested_inputs += "score ";

    cout << requested_inputs << endl;
}

void CGGameEngine::infinite_game_loop()
{
    this->parse_first_turn();
    this->set_requested_inputs();

    while (1)
    {
        parse_turn();

        if (this->_lastmove)
        {
            // Fisrt turns
            if (this->_cg_board->game_turn == 1)
            {
                this->_board = this->_cg_board->clone();
                cerr << "\nCGGameEngine: loop: Initial board:" << endl;
                this->_board->log();
            }
            else
            {
                this->_board->apply_move(*this->_last_move);
                cerr << "\nCGGameEngine: loop: Board AFTER last move: " << this->_last_move->to_uci() << endl;
                this->_board->log();
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
        }
        else
        {
            this->_board = this->_cg_board->clone();
        }

        vector<Move> moves = this->_board->get_available_moves();

        if (this->_moves)
        {
            if (Move::compare_move_vector(moves, this->_possible_moves))
                cerr << "CGGameEngine: loop: " << moves.size() << " legal moves were correctly found by the engine" << endl;
            else
            {
                cerr << "CGGameEngine: loop: Vector1: My chess board | Vector2: CG moves" << endl;
                this->_board->log();

                exit(1);
            }
        }

        Move move = this->_player->choose_from(this->_board, moves);

        this->_board->apply_move(move);

        cout << move.to_uci() << endl;

        cerr << "\nCGGameEngine: loop: Board AFTER applying the choosen move: " << move.to_uci() << endl;
        this->_board->log();

        float game_state = this->_board->game_state();
        if (game_state != GAME_CONTINUE)
        {
            cerr << "CGGameEngine: loop: Game is over :" << game_state << endl;
            delete this->_board;
        }
    }
}

// --- PRIVATE METHODS ---

void    CGGameEngine::parse_turn()
{
    if (this->_lastmove)
    {
        string move;
        cin >> move;
        cerr << "CGGameEngine: parsing: last_move = " << move << endl;

        this->_last_move = move == "none" ? NULL : new Move(move);
    }
    if (this->_fen)
    {
        string board;
        string color;
        string castling;
        string en_passant;
        string half_move_clock_str;
        string full_move_str;

        cin >> board >> color >> castling >> en_passant >> half_move_clock_str >> full_move_str;
        
        int half_move_clock = stoi(half_move_clock_str);
        int full_move = stoi(full_move_str);

        // Print all variables
        cerr << "CGGameEngine: parsing: board = " << board << endl;
        cerr << "CGGameEngine: parsing: color = " << color << endl;
        cerr << "CGGameEngine: parsing: castling = " << castling << endl;
        cerr << "CGGameEngine: parsing: en_passant = " << en_passant << endl;
        cerr << "CGGameEngine: parsing: half_move_clock_str = " << half_move_clock_str << endl;
        cerr << "CGGameEngine: parsing: full_move_str = " << full_move_str << endl;

        if (this->_cg_board)
            delete this->_cg_board;
        this->_cg_board = new Board(board, color, castling, en_passant, half_move_clock, full_move);
    }
    if (this->_moves)
    {
        string move;
        string move_count;

        cin >> move_count;
        this->_possible_moves_count = stoi(move_count);

        this->_possible_moves = vector<Move>();
        for (int i = 0; i < this->_possible_moves_count; i++)
        {
            cin >> move;
            this->_possible_moves.push_back(Move(move));
        }
    }
    // if (this->_draw)
    // {

    // }
    // if (this->_game)
    // {

    // }
    // if (this->_score)
    // {

    // }

    cin.ignore();
}
