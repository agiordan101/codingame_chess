#include "Board.hpp"

#pragma region apply_move

int apply_move_validMove_ApplyIt(int testIndex, Board *initial_board, Board *requested_board, Move* move)
{
    initial_board->apply_move(*move);

    if (!(*initial_board == requested_board))
    {
        cerr << "\n---------- Board - apply_move_validMove_ApplyIt() - Test " << testIndex << " - !!! FAILURE !!! ----------" << endl;
        cerr << "- Move applied : " << endl;
        move->log();

        cerr << "\n- Final board : " << endl;
        initial_board->log();
        cerr << "\n- Requested board : " << endl;
        requested_board->log();

        return 0;
    }

    return 1;
}

int apply_move_testLauncher()
{
    int successCount = 0;

    // Regular Black moves - (Reset the half turn rule due to a pawn advance)
    successCount += apply_move_validMove_ApplyIt(
        1,
        new Board("8/p7/8/8/8/8/8/8 b - - 5 1"),
        new Board("8/8/p7/8/8/8/8/8 w - - 0 2"),
        new Move(0, 1, 0, 2, 0)
    );
    // 2 advance black pawn that create a "en passant"
    successCount += apply_move_validMove_ApplyIt(
        2,
        new Board("8/1p6/8/2P5/8/8/8/8 b - - 0 1"),
        new Board("8/8/8/1pP5/8/8/8/8 w - b6 0 2"),
        new Move(1, 1, 1, 3, 0)
    );

    // Regular White moves - (Don't reset the half turn rule)
    successCount += apply_move_validMove_ApplyIt(
        3,
        new Board("8/8/8/8/8/8/8/3Q4 w - - 5 1"),
        new Board("8/8/8/8/6Q1/8/8/8 b - - 6 1"),
        new Move(3, 7, 6, 4, 0)
    );
    // 2 advance white pawn that create a "en passant"
    successCount += apply_move_validMove_ApplyIt(
        4,
        new Board("8/8/8/8/3p4/8/4P3/8 w - - 0 1"),
        new Board("8/8/8/8/3pP3/8/8/8 b - e3 0 1"),
        new Move(4, 6, 4, 4, 0)
    );

    // Black Chess960 Castle left
    successCount += apply_move_validMove_ApplyIt(
        5,
        new Board("r3k3/8/8/8/8/8/8/8 b a - 0 1"),
        new Board("2kr4/8/8/8/8/8/8/8 w - - 1 2"),
        new Move(4, 0, 0, 0, 0)
    );
    // Black Chess960 Castle right
    successCount += apply_move_validMove_ApplyIt(
        6,
        new Board("1k5r/8/8/8/8/8/8/8 b h - 0 1"),
        new Board("5rk1/8/8/8/8/8/8/8 w - - 1 2"),
        new Move(1, 0, 7, 0, 0)
    );
    // White Chess960 Castle left
    successCount += apply_move_validMove_ApplyIt(
        7,
        new Board("8/8/8/8/8/8/8/R5K1 w A - 0 1"),
        new Board("8/8/8/8/8/8/8/2KR4 b - - 1 1"),
        new Move(6, 7, 0, 7, 0)
    );
    // White Chess960 Castle right
    successCount += apply_move_validMove_ApplyIt(
        8,
        new Board("8/8/8/8/8/8/8/4K2R w H - 0 1"),
        new Board("8/8/8/8/8/8/8/5RK1 b - - 1 1"),
        new Move(4, 7, 7, 7, 0)
    );

    // Black Standard Castle left
    successCount += apply_move_validMove_ApplyIt(
        51,
        new Board("r3k3/8/8/8/8/8/8/8 b a - 0 1", false),
        new Board("2kr4/8/8/8/8/8/8/8 w - - 1 2", false),
        new Move(4, 0, 0, 0, 0)
    );
    // Black Standard Castle right
    successCount += apply_move_validMove_ApplyIt(
        61,
        new Board("1k5r/8/8/8/8/8/8/8 b h - 0 1", false),
        new Board("5rk1/8/8/8/8/8/8/8 w - - 1 2", false),
        new Move(1, 0, 7, 0, 0)
    );
    // White Standard Castle left
    successCount += apply_move_validMove_ApplyIt(
        71,
        new Board("8/8/8/8/8/8/8/R5K1 w A - 0 1", false),
        new Board("8/8/8/8/8/8/8/2KR4 b - - 1 1", false),
        new Move(6, 7, 0, 7, 0)
    );
    // White Standard Castle right
    successCount += apply_move_validMove_ApplyIt(
        81,
        new Board("8/8/8/8/8/8/8/4K2R w H - 0 1", false),
        new Board("8/8/8/8/8/8/8/5RK1 b - - 1 1", false),
        new Move(4, 7, 7, 7, 0)
    );

    // Castles rights deletion
    successCount += apply_move_validMove_ApplyIt(
        82,
        new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"),
        new Board("8/8/8/8/8/8/4K3/R6R b - - 1 1"),
        new Move(4, 7, 4, 6, 0)
    );
    successCount += apply_move_validMove_ApplyIt(
        83,
        new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"),
        new Board("8/8/8/8/8/8/R7/4K2R b H - 1 1"),
        new Move(0, 7, 0, 6, 0)
    );
    successCount += apply_move_validMove_ApplyIt(
        84,
        new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"),
        new Board("8/8/8/8/8/8/7R/R3K21 b A - 1 1"),
        new Move(7, 7, 7, 6, 0)
    );

    // Queen promotion White
    successCount += apply_move_validMove_ApplyIt(
        9,
        new Board("8/5P2/8/8/8/8/8/8 w - - 0 1"),
        new Board("5Q2/8/8/8/8/8/8/8 b - - 0 1"),
        new Move(5, 1, 5, 0, 'Q')
    );
    successCount += apply_move_validMove_ApplyIt(
        10,
        new Board("8/3P4/8/8/8/8/8/8 w - - 0 1"),
        new Board("3N4/8/8/8/8/8/8/8 b - - 0 1"),
        new Move(3, 1, 3, 0, 'N')
    );

    // Queen promotion Black
    successCount += apply_move_validMove_ApplyIt(
        11,
        new Board("8/8/8/8/8/8/5p2/8 b - - 0 1"),
        new Board("8/8/8/8/8/8/8/5r2 w - - 0 2"),
        new Move(5, 6, 5, 7, 'r')
    );
    successCount += apply_move_validMove_ApplyIt(
        12,
        new Board("8/8/8/8/8/8/3p4/8 b - - 0 1"),
        new Board("8/8/8/8/8/8/8/3b4 w - - 0 2"),
        new Move(3, 6, 3, 7, 'b')
    );

    // En passant White
    successCount += apply_move_validMove_ApplyIt(
        13,
        new Board("8/8/8/4pP2/8/8/8/8 w - b3 0 1"),
        new Board("8/8/4P3/8/8/8/8/8 b - - 0 1"),
        new Move(5, 3, 4, 2, 0)
    );
    successCount += apply_move_validMove_ApplyIt(
        14,
        new Board("8/8/8/8/2pP4/8/8/8 w - d3 0 1"),
        new Board("8/8/8/8/8/3p4/8/8 b - - 0 1"),
        new Move(2, 4, 3, 5, 0)
    );

    // Reset the half turn rule when a piece is captured
    successCount += apply_move_validMove_ApplyIt(
        15,
        new Board("8/8/4r3/8/8/8/4B3/8 b - - 5 1"),
        new Board("8/8/8/8/8/8/4r3/8 w - - 0 2"),
        new Move(4, 2, 4, 6, 0)
    );

    return successCount;
}

#pragma endregion apply_move

#pragma region find_moves

bool is_move_in_movelst(Move *move, vector<Move> movelst)
{
    for (Move move_f : movelst)
    {
        if (*move == &move_f)
            return true;
    }

    return false;
}

bool is_move_in_movelst(Move move, Move *movelst[10], int move_count)
{
    for (int i = 0; i < move_count; i++)
    {
        if (move == movelst[i])
            return true;
    }

    return false;
}

int find_moves_RegularCases_FindAllMoves(int testIndex, Board *board, Move *requested_moves[10], int requested_moves_count)
{
    // Act
    vector<Move> moves_found = board->find_moves();

    // Assert all requested moves were found by the engine
    bool success = true;
    for (int i = 0; i < requested_moves_count; i++)
    {
        if (!is_move_in_movelst(requested_moves[i], moves_found))
        {
            if (success)
            {
                cerr << "\n---------- Board - find_moves_RegularCases_FindAllMoves() - Test " << testIndex << " - !!! FAILURE !!! ----------" << endl;
                board->log();
            }

            cerr << "- This requested move wasn't found by the engine : " << endl;
            requested_moves[i]->log();
            success = false;
        }
    }

    // Assert all moves found by the engine are requested
    for (int i = 0; i < moves_found.size(); i++)
    {
        if (!is_move_in_movelst(moves_found[i], requested_moves, requested_moves_count))
        {
            if (success)
            {
                cerr << "\n---------- Board - find_moves_RegularCases_FindAllMoves() - Test " << testIndex << " - !!! FAILURE !!! ----------" << endl;
                board->log();
            }

            cerr << "- This move found by the engine isn't requested : " << endl;
            moves_found[i].log();
            success = false;
        }
    }

    // Assert no duplicates
    if (moves_found.size() != requested_moves_count)
    {
        if (success)
        {
            cerr << "\n---------- Board - find_moves_RegularCases_FindAllMoves() - Test " << testIndex << " - !!! FAILURE !!! ----------" << endl;
            board->log();
        }

        cerr << "- Moves found by the engine : " << endl;
        for (int i = 0; i < moves_found.size(); i++)
            moves_found[i].log();

        cerr << "- Requested moves : " << endl;
        for (int i = 0; i < requested_moves_count; i++)
            requested_moves[i]->log();

        success = false;
    }

    return success ? 1 : 0;
}

int find_pawn_moves_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[10];

    // Advances 0 and 1 and 2 - White
    requested_moves[0] = new Move(6, 6, 6, 5, 0); // Advance 1 but not 2
    requested_moves[1] = new Move(7, 6, 7, 5, 0); // Advance 1
    requested_moves[2] = new Move(7, 6, 7, 4, 0); // Advance 2
    success_count += find_moves_RegularCases_FindAllMoves(
        0,
        new Board("8/8/8/8/6#1/5#2/5PPP/8 w - - 0 1"),
        requested_moves,
        3
    );
    // Advances 0 and 1 and 2 - Black
    requested_moves[0] = new Move(1, 1, 1, 2, 0); // Advance 1 but not 2
    requested_moves[1] = new Move(2, 1, 2, 2, 0); // Advance 1
    requested_moves[2] = new Move(2, 1, 2, 3, 0); // Advance 2
    success_count += find_moves_RegularCases_FindAllMoves(
        1,
        new Board("8/ppp5/#7/1#6/8/8/8/8 b - - 0 1"),
        requested_moves,
        3
    );

    // Captures (Not behind) (Not allies) - White
    requested_moves[0] = new Move(0, 6, 1, 5, 0); // Right capture
    requested_moves[1] = new Move(7, 6, 6, 5, 0); // Left captures
    success_count += find_moves_RegularCases_FindAllMoves(
        2,
        new Board("8/8/2T#T3/3P4/8/#t4t#/P6P/1t4t1 w - - 0 1"),
        requested_moves,
        2
    );
    // Captures (Not behind) (Not allies) - Black
    requested_moves[0] = new Move(0, 1, 1, 2, 0);  // Left capture
    requested_moves[1] = new Move(7, 1, 6, 2, 0);  // Right capture
    success_count += find_moves_RegularCases_FindAllMoves(
        3,
        new Board("1T4T1/p6p/#T4T#/8/3p4/2t#t3/8/8 b - - 0 1"),
        requested_moves,
        2
    );

    // "en passant" - White - Left
    requested_moves[0] = new Move(0, 3, 1, 2, 0);
    success_count += find_moves_RegularCases_FindAllMoves(
        4,
        new Board("8/8/#7/Pp6/1#6/8/8/8 w - b6 0 1"),
        requested_moves,
        1
    );
    // "en passant" - White - Right
    requested_moves[0] = new Move(2, 3, 1, 2, 0);
    success_count += find_moves_RegularCases_FindAllMoves(
        5,
        new Board("8/8/2#5/1pP5/1#6/8/8/8 w - b6 0 1"),
        requested_moves,
        1
    );

    // "en passant" - Black - Left
    requested_moves[0] = new Move(1, 4, 0, 5, 0);
    success_count += find_moves_RegularCases_FindAllMoves(
        6,
        new Board("8/8/8/#7/Pp6/1#6/8/8 b - a3 0 1"),
        requested_moves,
        1
    );
    // "en passant" - Black - Right
    requested_moves[0] = new Move(1, 4, 2, 5, 0);
    success_count += find_moves_RegularCases_FindAllMoves(
        7,
        new Board("8/8/8/2#5/1pP5/1#6/8/8 b - c3 0 1"),
        requested_moves,
        1
    );

    // Promotions - White
    requested_moves[0] = new Move(0, 1, 0, 0, 'N');
    requested_moves[1] = new Move(0, 1, 0, 0, 'B');
    requested_moves[2] = new Move(0, 1, 0, 0, 'R');
    requested_moves[3] = new Move(0, 1, 0, 0, 'Q');
    success_count += find_moves_RegularCases_FindAllMoves(
        8,
        new Board("7#/P6P/8/8/8/8/8/8 w - - 0 1"),
        requested_moves,
        4
    );
    // Promotions - Black
    requested_moves[0] = new Move(0, 6, 0, 7, 'n');
    requested_moves[1] = new Move(0, 6, 0, 7, 'b');
    requested_moves[2] = new Move(0, 6, 0, 7, 'r');
    requested_moves[3] = new Move(0, 6, 0, 7, 'q');
    success_count += find_moves_RegularCases_FindAllMoves(
        9,
        new Board("8/8/8/8/8/8/p6p/7# b - - 0 1"),
        requested_moves,
        4
    );

    // No piece -> No moves
    success_count += find_moves_RegularCases_FindAllMoves(
        10,
        new Board("8/8/8/8/8/8/8/8 w - - 0 1"),
        requested_moves,
        0
    );

    return success_count;
}

    // For both sides - Knight tests - 1 in the center Return all Regular move (8) (With obstacles)
    // For both sides - Knight tests - 4 on edges      Return all Regular move (6/8) (2 blocked due to 2 opponent pieces)

    // For both sides - Bishop tests - Return all Regular moves. 1 line should eat an opponent piece. Some lines are obstrued
    // For both sides - Rook tests - Return all Regular moves. 1 line should eat an opponent piece. Some lines are obstrued
    // For both sides - Queen tests - Return all Regular moves. Some line should eat an opponent piece. Some lines are obstrued

    // For both sides - King tests - Return all Regular moves. Some line should eat an opponent piece. Some lines are obstrued

    // For both sides - Check tests - King cannot move on a threated cell (By all other pieces)
    // For both sides - Check tests - Pieces cannot moves if the king get checked
    // For both sides - Other tests - No move found (Stale mate)

#pragma endregion find_moves

#pragma region create_fen

int create_fen_unittest(int testIndex, Board *board, string requested_fen)
{
    string fen = board->create_fen();

    if (fen != requested_fen)
    {
        cerr << "\n---------- Board - create_fen_unittest() - Test " << testIndex << " - !!! FAILURE !!! ----------" << endl;
        cerr << "- Final board : " << endl;
        board->log();
        cerr << "\n- Created fen   : " << fen << endl;
        cerr << "- Requested fen : " << requested_fen << endl;

        return 0;
    }

    return 1;
}

int create_fen_testLauncher()
{
    int success_count = 0;

    // Pieces test
    success_count += create_fen_unittest(
        1,
        new Board("r1bqkbnr/8/8/8/8/8/8/RNBQKB1R w - - 0 1"),
        "r1bqkbnr/8/8/8/8/8/8/RNBQKB1R w - - 0 1"
    );

    // Pieces test 2. Edge test
    success_count += create_fen_unittest(
        1,
        new Board("1nb2bn1/8/8/3pP3/8/8/8/1NB2BN1 w - - 0 1"),
        "1nb2bn1/8/8/3pP3/8/8/8/1NB2BN1 w - - 0 1"
    );

    // Empty board test + castling test
    success_count += create_fen_unittest(
        2,
        new Board("8/8/8/8/8/8/8/8 b AHah - 1 2"),
        "8/8/8/8/8/8/8/8 b AHah - 1 2"
    );

    // Not full castling rights test
    success_count += create_fen_unittest(
        3,
        new Board("8/8/8/8/8/8/8/8 w Ac - 0 1"),
        "8/8/8/8/8/8/8/8 w Ac - 0 1"
    );

    // En passant test
    success_count += create_fen_unittest(
        4,
        new Board("8/8/8/8/8/8/8/8 w - h7 0 1"),
        "8/8/8/8/8/8/8/8 w - h7 0 1"
    );

    // Half turn rule test
    success_count += create_fen_unittest(
        5,
        new Board("8/8/8/8/8/8/8/8 w - - 12 1"),
        "8/8/8/8/8/8/8/8 w - - 12 1"
    );

    // Game turn test
    success_count += create_fen_unittest(
        6,
        new Board("8/8/8/8/8/8/8/8 w - - 0 101"),
        "8/8/8/8/8/8/8/8 w - - 0 101"
    );

    return success_count;
}

#pragma endregion create_fen

#pragma region game_state

int game_state_unittest(int testIndex, Board *board, vector<Move> moves, bool check, float requested_game_state)
{
    board->available_moves = moves;
    board->moves_found = true;
    board->check = check;
    float game_state = board->game_state();

    if (game_state != requested_game_state)
    {
        cerr << "\n---------- Board - game_state_unittest() - Test " << testIndex << " - !!! FAILURE !!! ----------" << endl;
        cerr << "- Board : " << endl;
        board->log();

        cerr << "\n- Actual game_state    : " << game_state << endl;
        cerr << "- Requested game_state : " << requested_game_state << endl;

        return 0;
    }

    return 1;
}

int game_state_testLauncher()
{
    int success_count = 0;
    vector<Move> moves_exists;
    vector<Move> moves_empty;

    moves_exists.push_back(Move(0, 0, 0, 0, 0));

    // 1 - Fifty-Move rule
    success_count += game_state_unittest(
        1,
        new Board("8/3k4/3p4/8/3P4/3K4/8/8 w - - 50 1"),
        moves_exists,
        false,
        0.5
    );

    // 2 - Game turn max reached
    success_count += game_state_unittest(
        2,
        new Board("8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 125"),
        moves_exists,
        false,
        0.5
    );

    // 3 - Current player checkmate
    success_count += game_state_unittest(
        3,
        new Board("8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 0"),
        moves_empty,
        true,
        0
    );

    // 4 - Stalemate
    success_count += game_state_unittest(
        4,
        new Board("8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 0"),
        moves_empty,
        false,
        0.5
    );

    // Insufficient material: King vs king
    success_count += game_state_unittest(
        5,
        new Board("8/8/3K4/8/8/3k4/8/8 w - - 0 0"),
        moves_exists,
        false,
        0.5
    );

    // Insufficient material: King+knight vs king
    success_count += game_state_unittest(
        6,
        new Board("8/8/3K4/8/3n4/3k4/8/8 w - - 0 0"),
        moves_exists,
        false,
        0.5
    );

    // Insufficient material: King+bishop vs king
    success_count += game_state_unittest(
        7,
        new Board("8/8/3K4/3B4/8/3k4/8/8 w - - 0 0"),
        moves_exists,
        false,
        0.5
    );

    // Insufficient material: King+bishop vs king+bishop if both bishops are on the same square color.
    success_count += game_state_unittest(
        8,
        new Board("8/8/3K4/3B4/4b3/3k4/8/8 w - - 0 0"),
        moves_exists,
        false,
        0.5
    );

    // Game continue (2 bishops on different square color)
    success_count += game_state_unittest(
        9,
        new Board("8/8/3K4/3B4/3b4/3k4/8/8 w - - 0 0"),
        moves_exists,
        false,
        -1
    );

    // Game continue (2 knights)
    success_count += game_state_unittest(
        9,
        new Board("8/8/3K4/3N4/3n4/3k4/8/8 w - - 0 0"),
        moves_exists,
        false,
        -1
    );

    // Game continue (Queen)
    success_count += game_state_unittest(
        9,
        new Board("8/8/3K4/3Q4/8/3k4/8/8 w - - 0 0"),
        moves_exists,
        false,
        -1
    );

    // Game continue (Rook)
    success_count += game_state_unittest(
        9,
        new Board("8/8/3K4/8/3r4/3k4/8/8 w - - 0 0"),
        moves_exists,
        false,
        -1
    );

    Board *board = new Board("8/3K4/3Q4/8/8/3q4/3k4/8 w - - 0 0");

    // Threefold Repetition rule fails (Only 1 repetition)
    board->apply_move(Move(3, 2, 4, 2, 0)); // White move right
    board->apply_move(Move(3, 5, 4, 5, 0)); // Black move right
    board->apply_move(Move(4, 2, 3, 2, 0)); // White move left
    board->apply_move(Move(4, 5, 3, 5, 0)); // Black move left
    success_count += game_state_unittest(
        9,
        board,
        moves_exists,
        false,
        -1
    );

    // Threefold Repetition rule succeed -> 2 repetitions
    board->apply_move(Move(3, 2, 4, 2, 0)); // White move right
    board->apply_move(Move(3, 5, 4, 5, 0)); // Black move right
    board->apply_move(Move(4, 2, 3, 2, 0)); // White move left
    board->apply_move(Move(4, 5, 3, 5, 0)); // Black move left
    success_count += game_state_unittest(
        9,
        board,
        moves_exists,
        false,
        0.5
    );

    return success_count;
}

#pragma endregion game_state

int mainTestBoard()
{
    int successCount = 0;

    // Use '#' to block moves
    // Use 't' and 'T' to simulate opponent pieces

    successCount += apply_move_testLauncher();
    successCount += find_pawn_moves_testLauncher();
    successCount += create_fen_testLauncher();
    successCount += game_state_testLauncher();

    return successCount;
}
