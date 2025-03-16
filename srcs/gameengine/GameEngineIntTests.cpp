#include "GameEngineIntTests.hpp"

// --- PUBLIC METHODS ---

GameEngineIntTests::GameEngineIntTests(BotPlayer *player)
{
    this->_player = player;
    this->_cg_last_move = NULL;
    this->_cg_board = NULL;
    this->_board = NULL;
}

void GameEngineIntTests::infinite_game_loop()
{
    this->_parse_first_turn();
    cout << "lastmove fen" << endl;
    // cout << "lastmove fen moves" << endl;

    while (1)
    {
        // cerr << "\nGameEngineIntTests: " << this->_player->get_name() << endl;
        _parse_turn();

        // First turns
        if (this->_cg_board->game_turn == 1)
        {
            this->_board = this->_cg_board->clone();
            cerr << "\nGameEngineIntTests: loop: Initial board:" << endl;
            this->_board->log(true);
        }
        else
        {
            this->_board->apply_move(*this->_cg_last_move);
            cerr << "\nGameEngineIntTests: loop: Board AFTER last move: " << this->_cg_last_move->to_uci() << endl;
            this->_board->log(true);
        }

        if (this->_fen && !(*this->_board == this->_cg_board))
        {
            cerr << "\nGameEngineIntTests: loop: Board is not the same as the one from CG: " << endl;
            cerr << "GameEngineIntTests: loop: Board from CG:" << endl;
            this->_cg_board->log();
            cerr << "GameEngineIntTests: loop: Board from ChessEngine:" << endl;
            this->_board->log(true);
            this->_board->log_history(3);

            exit(1);
        }

        cerr << "move computed: " << this->_board->moves_computed << endl;
        vector<Move> moves = this->_board->get_available_moves();

        // Print move UCI
        for (Move move : moves)
           cerr << move.to_uci() << " ";
        cerr << endl;

        // // Compare move list
        // if (Move::compare_move_vector(moves, this->_possible_moves))
        //     cerr << "GameEngineIntTests: loop: " << moves.size() << " legal moves were correctly found by the engine" << endl;
        // else
        // {
        //     cerr << "GameEngineIntTests: loop: Vector1: My chess board | Vector2: CG moves" << endl;
        //     this->_board->log(true);

        //     exit(1);
        // }

        Move move = this->_player->choose_from(this->_board, moves);
        cerr << "Choosen move: " << move.to_uci() << endl;

        this->_board->apply_move(move);

        cerr << "\nGameEngineIntTests: loop: Board AFTER applying the choosen move: " << move.to_uci() << endl;
        this->_board->log(true);

        cout << move.to_uci() << endl;

        // float game_state = this->_board->get_game_state();
        // if (game_state != GAME_CONTINUE)
        // {
        //     cerr << "GameEngineIntTests: loop: Game is over :" << game_state << endl;
        //     delete this->_board;
        // }
    }
}

// --- PRIVATE METHODS ---

void GameEngineIntTests::_parse_first_turn()
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
    // cerr << "GameEngineIntTests: first turn: crazyHouse = " << crazyHouse << endl;
    // cerr << "GameEngineIntTests: first turn: maxMoves = " << maxMoves << endl;
}

void    GameEngineIntTests::_parse_turn()
{
    // Parse last move
    string move;
    cin >> move;
    // cerr << "GameEngineIntTests: parsing: last_move = " << move << endl;

    this->_cg_last_move = move == "none" ? NULL : new Move(move);

    // Parse FEN
    string board;
    string color;
    string castling;
    string en_passant;
    string half_move_clock_str;
    string full_move_str;

    cin >> board >> color >> castling >> en_passant >> half_move_clock_str >> full_move_str;

    cerr << "GameEngineIntTests: parsing: board = " << board << endl;
    cerr << "GameEngineIntTests: parsing: color = " << color << endl;
    cerr << "GameEngineIntTests: parsing: castling = " << castling << endl;
    cerr << "GameEngineIntTests: parsing: en_passant = " << en_passant << endl;
    cerr << "GameEngineIntTests: parsing: half_move_clock_str = " << half_move_clock_str << endl;
    cerr << "GameEngineIntTests: parsing: full_move_str = " << full_move_str << endl;

    int half_move_clock = stoi(half_move_clock_str);
    int full_move = stoi(full_move_str);

    if (this->_cg_board)
        delete this->_cg_board;
    this->_cg_board = new Board(board, color, castling, en_passant, half_move_clock, full_move);

    // // Parse available moves
    // string move_count;

    // cin >> move_count;
    // cerr << "move_count = " << move_count << endl;
    // this->_possible_moves_count = stoi(move_count);

    // this->_possible_moves = vector<Move>();
    // for (int i = 0; i < this->_possible_moves_count; i++)
    // {
    //     cin >> move;
    //     this->_possible_moves.push_back(Move(move));
    // }

    cin.ignore();
}
