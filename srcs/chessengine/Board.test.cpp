#include "Board.hpp"

#pragma region apply_move

int apply_move_validMove_ApplyIt(
    int testIndex, Board *initial_board, Board *requested_board, Move *move
)
{
    Board test_board = initial_board->clone();

    test_board.apply_move(*move);

    if (!(test_board == requested_board))
    {
        cerr << "\n---------- Board - apply_move_validMove_ApplyIt() - Test " << testIndex
             << " - !!! FAILURE !!! ----------" << endl;
        cerr << "\n- Initial board : " << endl;
        initial_board->log();

        cerr << "- Move applied : " << endl;
        move->log();

        cerr << "\n- Final board : " << endl;
        test_board.log();
        cerr << "\n- Requested board : " << endl;
        requested_board->log();

        return 0;
    }

    return 1;
}

int apply_move_testLauncher_uci()
{
    int successCount = 0;

    // Regular Black moves - (Reset the half turn rule due to a pawn advance)
    successCount += apply_move_validMove_ApplyIt(
        101, new Board("8/p7/8/8/8/8/8/8 b - - 5 1"), new Board("8/8/p7/8/8/8/8/8 w - - 0 2"),
        new Move("a7a6")

    );
    // 2 advance black pawn that create a "en passant"
    successCount += apply_move_validMove_ApplyIt(
        102, new Board("8/1p6/8/2P5/8/8/8/8 b - - 0 1"), new Board("8/8/8/1pP5/8/8/8/8 w - b6 0 2"),
        new Move("b7b5")

    );
    // 2 "en passant" in a row
    successCount += apply_move_validMove_ApplyIt(
        103, new Board("8/1p6/8/2P5/4P3/8/8/8 b - e4 0 1"),
        new Board("8/8/8/1pP5/4P3/8/8/8 w - b6 0 2"), new Move("b7b5")
    );

    // Regular White moves - (Don't reset the half turn rule)
    successCount += apply_move_validMove_ApplyIt(
        104, new Board("8/8/8/8/8/8/8/3Q4 w - - 5 1"), new Board("8/8/8/8/6Q1/8/8/8 b - - 6 1"),
        new Move("d1g4")
    );
    // 2 advance white pawn that create a "en passant"
    successCount += apply_move_validMove_ApplyIt(
        105, new Board("8/8/8/8/3p4/8/4P3/8 w - - 0 1"), new Board("8/8/8/8/3pP3/8/8/8 b - e3 0 1"),
        new Move("e2e4")
    );

    // Black Chess960 Castle left
    successCount += apply_move_validMove_ApplyIt(
        106, new Board("r3k3/8/8/8/8/8/8/8 b a - 0 1"), new Board("2kr4/8/8/8/8/8/8/8 w - - 1 2"),
        new Move("e8a8")
    );
    // Black Chess960 Castle right
    successCount += apply_move_validMove_ApplyIt(
        107, new Board("1k5r/8/8/8/8/8/8/8 b h - 0 1"), new Board("5rk1/8/8/8/8/8/8/8 w - - 1 2"),
        new Move("b8h8")
    );
    // White Chess960 Castle left
    successCount += apply_move_validMove_ApplyIt(
        108, new Board("8/8/8/8/8/8/8/R5K1 w A - 0 1"), new Board("8/8/8/8/8/8/8/2KR4 b - - 1 1"),
        new Move("g1a1")
    );
    // White Chess960 Castle right
    successCount += apply_move_validMove_ApplyIt(
        109, new Board("8/8/8/8/8/8/8/4K2R w H - 0 1"), new Board("8/8/8/8/8/8/8/5RK1 b - - 1 1"),
        new Move("e1h1")
    );
    // White king takes black rook, not castling
    successCount += apply_move_validMove_ApplyIt(
        110, new Board("8/8/8/8/8/8/8/1Kr5 w - - 2 78"), new Board("8/8/8/8/8/8/8/2K5 b - - 0 78"),
        new Move("b1c1")
    );

    // Black Standard Castle left
    successCount += apply_move_validMove_ApplyIt(
        111, new Board("r3k3/8/8/8/8/8/8/8 b a - 0 1", false),
        new Board("2kr4/8/8/8/8/8/8/8 w - - 1 2", false), new Move("e8a8")
    );
    // Black Standard Castle right
    successCount += apply_move_validMove_ApplyIt(
        112, new Board("1k5r/8/8/8/8/8/8/8 b h - 0 1", false),
        new Board("5rk1/8/8/8/8/8/8/8 w - - 1 2", false), new Move("b8h8")
    );
    // White Standard Castle left
    successCount += apply_move_validMove_ApplyIt(
        113, new Board("8/8/8/8/8/8/8/R5K1 w A - 0 1", false),
        new Board("8/8/8/8/8/8/8/2KR4 b - - 1 1", false), new Move("g1a1")
    );
    // White Standard Castle right
    successCount += apply_move_validMove_ApplyIt(
        114, new Board("8/8/8/8/8/8/8/4K2R w H - 0 1", false),
        new Board("8/8/8/8/8/8/8/5RK1 b - - 1 1", false), new Move("e1h1")
    );

    // Castles rights test - King moves
    successCount += apply_move_validMove_ApplyIt(
        115, new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"),
        new Board("8/8/8/8/8/8/4K3/R6R b - - 1 1"), new Move("e1e2")
    );
    // Castles rights test - Rook moves
    successCount += apply_move_validMove_ApplyIt(
        116, new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"),
        new Board("8/8/8/8/8/8/R7/4K2R b H - 1 1"), new Move("a1a2")
    );
    // Castles rights test - Rook moves
    successCount += apply_move_validMove_ApplyIt(
        117, new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"),
        new Board("8/8/8/8/8/8/7R/R3K21 b A - 1 1"), new Move("h1h2")
    );

    // Promotion White
    successCount += apply_move_validMove_ApplyIt(
        119, new Board("8/3P4/8/8/8/8/8/8 w - - 0 1"), new Board("3N4/8/8/8/8/8/8/8 b - - 0 1"),
        new Move("d7d8n")
    );

    // Promotion Black
    successCount += apply_move_validMove_ApplyIt(
        121, new Board("8/8/8/8/8/8/3p4/8 b - - 0 1"), new Board("8/8/8/8/8/8/8/3b4 w - - 0 2"),
        new Move("d2d1b")
    );

    // En passant White
    successCount += apply_move_validMove_ApplyIt(
        123, new Board("8/8/8/4pP2/8/8/8/8 w - e6 0 1"), new Board("8/8/4P3/8/8/8/8/8 b - - 0 1"),
        new Move("f5e6")
    );
    // En passant Black
    successCount += apply_move_validMove_ApplyIt(
        125, new Board("8/8/8/8/2pP4/8/8/8 b - d3 0 1"), new Board("8/8/8/8/8/3p4/8/8 w - - 0 2"),
        new Move("c4d3")
    );

    // Reset the half turn rule when a piece is captured
    successCount += apply_move_validMove_ApplyIt(
        126, new Board("8/8/4r3/8/8/8/4B3/8 b - - 5 1"), new Board("8/8/8/8/8/8/4r3/8 w - - 0 2"),
        new Move("e6e2")
    );

    // White captures a piece
    successCount += apply_move_validMove_ApplyIt(
        127, new Board("8/8/8/6r1/8/4B3/8/8 w - - 5 1"), new Board("8/8/8/6B1/8/8/8/8 b - - 0 1"),
        new Move("e3g5")
    );
    // Black captures a piece
    successCount += apply_move_validMove_ApplyIt(
        128, new Board("8/8/8/6b1/8/4R3/8/8 b - - 5 1"), new Board("8/8/8/8/8/4b3/8/8 w - - 0 2"),
        new Move("g5e3")
    );

    // White promotes to Queen while capturing a piece
    successCount += apply_move_validMove_ApplyIt(
        129, new Board("4q3/3P4/8/8/8/8/8/8 w - - 5 1"), new Board("4Q3/8/8/8/8/8/8/8 b - - 0 1"),
        new Move("d7e8q")
    );
    // Black promotes to Queen while capturing a piece
    successCount += apply_move_validMove_ApplyIt(
        130, new Board("8/8/8/8/8/8/3p4/4Q3 b - - 5 1"), new Board("8/8/8/8/8/8/8/4q3 w - - 0 2"),
        new Move("d2e1q")
    );

    return successCount;
}

int apply_move_testLauncher()
{
    int successCount = 0;

    // Regular Black moves - (Reset the half turn rule due to a pawn advance)
    successCount += apply_move_validMove_ApplyIt(
        1, new Board("8/p7/8/8/8/8/8/8 b - - 5 1"), new Board("8/8/p7/8/8/8/8/8 w - - 0 2"),
        new Move('p', 1UL << 8, 1UL << 16)
    );
    // 2 advance black pawn that create a "en passant"
    successCount += apply_move_validMove_ApplyIt(
        2, new Board("8/1p6/8/2P5/8/8/8/8 b - - 0 1"), new Board("8/8/8/1pP5/8/8/8/8 w - b6 0 2"),
        new Move('p', 1UL << 9, 1UL << 25)
    );
    // 2 "en passant" in a row
    successCount += apply_move_validMove_ApplyIt(
        3, new Board("8/1p6/8/2P5/4P3/8/8/8 b - e4 0 1"),
        new Board("8/8/8/1pP5/4P3/8/8/8 w - b6 0 2"), new Move('p', 1UL << 9, 1UL << 25)
    );

    // Regular White moves - (Don't reset the half turn rule)
    successCount += apply_move_validMove_ApplyIt(
        4, new Board("8/8/8/8/8/8/8/3Q4 w - - 5 1"), new Board("8/8/8/8/6Q1/8/8/8 b - - 6 1"),
        new Move('Q', 1UL << 59, 1UL << 38)
    );
    // 2 advance white pawn that create a "en passant"
    successCount += apply_move_validMove_ApplyIt(
        5, new Board("8/8/8/8/3p4/8/4P3/8 w - - 0 1"), new Board("8/8/8/8/3pP3/8/8/8 b - e3 0 1"),
        new Move('P', 1UL << 52, 1UL << 36)
    );

    // Black Chess960 Castle left
    successCount += apply_move_validMove_ApplyIt(
        6, new Board("r3k3/8/8/8/8/8/8/8 b a - 0 1"), new Board("2kr4/8/8/8/8/8/8/8 w - - 1 2"),
        new Move('k', 1UL << 4, 1UL << 0, 0, BLACKLEFT)
    );
    // Black Chess960 Castle right
    successCount += apply_move_validMove_ApplyIt(
        7, new Board("1k5r/8/8/8/8/8/8/8 b h - 0 1"), new Board("5rk1/8/8/8/8/8/8/8 w - - 1 2"),
        new Move('k', 1UL << 1, 1UL << 7, 0, BLACKRIGHT)
    );
    // White Chess960 Castle left
    successCount += apply_move_validMove_ApplyIt(
        8, new Board("8/8/8/8/8/8/8/R5K1 w A - 0 1"), new Board("8/8/8/8/8/8/8/2KR4 b - - 1 1"),
        new Move('K', 1UL << 62, 1UL << 56, 0, WHITELEFT)
    );
    // White Chess960 Castle right
    successCount += apply_move_validMove_ApplyIt(
        9, new Board("8/8/8/8/8/8/8/4K2R w H - 0 1"), new Board("8/8/8/8/8/8/8/5RK1 b - - 1 1"),
        new Move('K', 1UL << 60, 1UL << 63, 0, WHITERIGHT)
    );
    // White king takes black rook, not castling
    successCount += apply_move_validMove_ApplyIt(
        10, new Board("8/8/8/8/8/8/8/1Kr5 w - - 2 78"), new Board("8/8/8/8/8/8/8/2K5 b - - 0 78"),
        new Move('K', 1UL << 57, 1UL << 58, 0, NOTCASTLE)
    );

    // Black Standard Castle left
    successCount += apply_move_validMove_ApplyIt(
        11, new Board("r3k3/8/8/8/8/8/8/8 b a - 0 1", false),
        new Board("2kr4/8/8/8/8/8/8/8 w - - 1 2", false),
        new Move('k', 1UL << 4, 1UL << 0, 0, BLACKLEFT)
    );
    // Black Standard Castle right
    successCount += apply_move_validMove_ApplyIt(
        12, new Board("1k5r/8/8/8/8/8/8/8 b h - 0 1", false),
        new Board("5rk1/8/8/8/8/8/8/8 w - - 1 2", false),
        new Move('k', 1UL << 1, 1UL << 7, 0, BLACKRIGHT)
    );
    // White Standard Castle left
    successCount += apply_move_validMove_ApplyIt(
        13, new Board("8/8/8/8/8/8/8/R5K1 w A - 0 1", false),
        new Board("8/8/8/8/8/8/8/2KR4 b - - 1 1", false),
        new Move('K', 1UL << 62, 1UL << 56, 0, WHITELEFT)
    );
    // White Standard Castle right
    successCount += apply_move_validMove_ApplyIt(
        14, new Board("8/8/8/8/8/8/8/4K2R w H - 0 1", false),
        new Board("8/8/8/8/8/8/8/5RK1 b - - 1 1", false),
        new Move('K', 1UL << 60, 1UL << 63, 0, WHITERIGHT)
    );

    // Castles rights test - King moves
    successCount += apply_move_validMove_ApplyIt(
        15, new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"), new Board("8/8/8/8/8/8/4K3/R6R b - - 1 1"),
        new Move('K', 1UL << 60, 1UL << 52, 0, NOTCASTLE)
    );
    // Castles rights test - Rook moves
    successCount += apply_move_validMove_ApplyIt(
        16, new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"), new Board("8/8/8/8/8/8/R7/4K2R b H - 1 1"),
        new Move('R', 1UL << 56, 1UL << 48)
    );
    // Castles rights test - Rook moves
    successCount += apply_move_validMove_ApplyIt(
        17, new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"),
        new Board("8/8/8/8/8/8/7R/R3K21 b A - 1 1"), new Move('R', 1UL << 63, 1UL << 55)
    );

    // Promotion White
    successCount += apply_move_validMove_ApplyIt(
        18, new Board("8/5P2/8/8/8/8/8/8 w - - 0 1"), new Board("5Q2/8/8/8/8/8/8/8 b - - 0 1"),
        new Move('P', 1UL << 13, 1UL << 5, 'Q')
    );

    // Promotion Black
    successCount += apply_move_validMove_ApplyIt(
        20, new Board("8/8/8/8/8/8/5p2/8 b - - 0 1"), new Board("8/8/8/8/8/8/8/5r2 w - - 0 2"),
        new Move('p', 1UL << 53, 1UL << 61, 'r')
    );

    // En passant White
    successCount += apply_move_validMove_ApplyIt(
        22, new Board("8/8/8/4pP2/8/8/8/8 w - e6 0 1"), new Board("8/8/4P3/8/8/8/8/8 b - - 0 1"),
        new Move('P', 1UL << 29, 1UL << 20)
    );
    // En passant Black
    successCount += apply_move_validMove_ApplyIt(
        24, new Board("8/8/8/8/2pP4/8/8/8 b - d3 0 1"), new Board("8/8/8/8/8/3p4/8/8 w - - 0 2"),
        new Move('p', 1UL << 34, 1UL << 43)
    );

    // Reset the half turn rule when a piece is captured
    successCount += apply_move_validMove_ApplyIt(
        26, new Board("8/8/4r3/8/8/8/4B3/8 b - - 5 1"), new Board("8/8/8/8/8/8/4r3/8 w - - 0 2"),
        new Move('r', 1UL << 20, 1UL << 52)
    );

    // White captures a piece
    successCount += apply_move_validMove_ApplyIt(
        27, new Board("8/8/8/6r1/8/4B3/8/8 w - - 5 1"), new Board("8/8/8/6B1/8/8/8/8 b - - 0 1"),
        new Move('B', 1UL << 44, 1UL << 30)
    );
    // Black captures a piece
    successCount += apply_move_validMove_ApplyIt(
        28, new Board("8/8/8/6b1/8/4R3/8/8 b - - 5 1"), new Board("8/8/8/8/8/4b3/8/8 w - - 0 2"),
        new Move('b', 1UL << 30, 1UL << 44)
    );

    // White promotes to Queen while capturing a piece
    successCount += apply_move_validMove_ApplyIt(
        29, new Board("4q3/3P4/8/8/8/8/8/8 w - - 5 1"), new Board("4Q3/8/8/8/8/8/8/8 b - - 0 1"),
        new Move('P', 1UL << 11, 1UL << 4, 'Q')
    );
    // Black promotes to Queen while capturing a piece
    successCount += apply_move_validMove_ApplyIt(
        30, new Board("8/8/8/8/8/8/3p4/4Q3 b - - 5 1"), new Board("8/8/8/8/8/8/8/4q3 w - - 0 2"),
        new Move('p', 1UL << 51, 1UL << 60, 'q')
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

int find_moves_RegularCases_FindAllMoves(
    int testIndex, Board *board, Move *requested_moves[10], size_t requested_moves_count
)
{
    // Act
    vector<Move> moves_found = board->get_available_moves();

    // Assert all requested moves were found by the engine
    bool success = true;
    for (size_t i = 0; i < requested_moves_count; i++)
    {
        if (!is_move_in_movelst(requested_moves[i], moves_found))
        {
            if (success)
            {
                cerr << "\n---------- Board - find_moves_RegularCases_FindAllMoves() - Test "
                     << testIndex << " - !!! FAILURE !!! ----------" << endl;
                board->log();
            }

            cerr << "- This requested move wasn't found by the engine : " << endl;
            requested_moves[i]->log();
            success = false;
        }
    }

    // Assert all moves found by the engine are requested
    for (size_t i = 0; i < moves_found.size(); i++)
    {
        if (!is_move_in_movelst(moves_found[i], requested_moves, requested_moves_count))
        {
            if (success)
            {
                cerr << "\n---------- Board - find_moves_RegularCases_FindAllMoves() - Test "
                     << testIndex << " - !!! FAILURE !!! ----------" << endl;
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
            cerr << "\n---------- Board - find_moves_RegularCases_FindAllMoves() - Test "
                 << testIndex << " - !!! FAILURE !!! ----------" << endl;
            board->log();
        }

        cerr << "- Moves found by the engine : " << endl;
        for (size_t i = 0; i < moves_found.size(); i++)
            moves_found[i].log();

        cerr << "- Requested moves : " << endl;
        for (size_t i = 0; i < requested_moves_count; i++)
            requested_moves[i]->log();

        success = false;
    }

    return success ? 1 : 0;
}

int find_pawn_moves_testLauncher()
{
    int   success_count = 0;
    Move *requested_moves[20];

    // All White possible moves
    requested_moves[0] = new Move('P', 1UL << 49, 1UL << 41);  // Advance 1 (Against enemy)
    requested_moves[1] = new Move('P', 1UL << 49, 1UL << 33);  // Advance 2 (Against enemy)
    requested_moves[2] = new Move('P', 1UL << 49, 1UL << 42);  // Capture right
    requested_moves[3] = new Move('P', 1UL << 51, 1UL << 42);  // Capture left
    requested_moves[4] = new Move('P', 1UL << 51, 1UL << 43);  // Advance 1 (Against enemy)
    requested_moves[5] = new Move('P', 1UL << 53, 1UL << 45);  // Advance 1 (Against ally)
    requested_moves[6] = new Move('P', 1UL << 53, 1UL << 37);  // Advance 2 (Against ally)
    requested_moves[7] = new Move('P', 1UL << 46, 1UL << 38);  // Advance 1 (By the 1st blocker)
    requested_moves[8] = new Move('P', 1UL << 55, 1UL << 47);  // Advance 1 (Against ally)
    requested_moves[9] = new Move('P', 1UL << 39, 1UL << 31);  // Advance 1 (By the 2nd blocker)
    requested_moves[10] = new Move('P', 1UL << 8, 1UL, 'N');   // Promotion
    requested_moves[11] = new Move('P', 1UL << 8, 1UL, 'B');   // Promotion
    requested_moves[12] = new Move('P', 1UL << 8, 1UL, 'R');   // Promotion
    requested_moves[13] = new Move('P', 1UL << 8, 1UL, 'Q');   // Promotion
    requested_moves[14] = new Move('P', 1UL << 20, 1UL << 13); // En passant right
    requested_moves[15] = new Move('P', 1UL << 22, 1UL << 13); // En passant left
    requested_moves[16] = new Move('P', 1UL << 20, 1UL << 12); // Advance 1
    requested_moves[17] = new Move('P', 1UL << 22, 1UL << 14); // Advance 1
    success_count += find_moves_RegularCases_FindAllMoves(
        1, new Board("8/P7/4PpP1/8/3q3P/2q3P1/1PPP1PPP/4q3 w - f7 0 1"), requested_moves, 18
    );

    // All Black possible moves
    requested_moves[0] = new Move('p', 1UL << 9, 1UL << 17);  // Advance 1 (Against enemy)
    requested_moves[1] = new Move('p', 1UL << 9, 1UL << 25);  // Advance 2 (Against enemy)
    requested_moves[2] = new Move('p', 1UL << 9, 1UL << 18);  // Capture right
    requested_moves[3] = new Move('p', 1UL << 11, 1UL << 18); // Capture left
    requested_moves[4] = new Move('p', 1UL << 11, 1UL << 19); // Advance 1 (Against enemy)
    requested_moves[5] = new Move('p', 1UL << 13, 1UL << 21); // Advance 1 (Against ally)
    requested_moves[6] = new Move('p', 1UL << 13, 1UL << 29); // Advance 2 (Against ally)
    requested_moves[7] = new Move('p', 1UL << 22, 1UL << 30); // Advance 1 (By the 1st blocker)
    requested_moves[8] = new Move('p', 1UL << 15, 1UL << 23); // Advance 1 (Against ally)
    requested_moves[9] = new Move('p', 1UL << 31, 1UL << 39); // Advance 1 (By the 2nd blocker)
    requested_moves[10] = new Move('p', 1UL << 48, 1UL << 56, 'n'); // Promotion
    requested_moves[11] = new Move('p', 1UL << 48, 1UL << 56, 'b'); // Promotion
    requested_moves[12] = new Move('p', 1UL << 48, 1UL << 56, 'r'); // Promotion
    requested_moves[13] = new Move('p', 1UL << 48, 1UL << 56, 'q'); // Promotion
    requested_moves[14] = new Move('p', 1UL << 44, 1UL << 53);      // En passant right
    requested_moves[15] = new Move('p', 1UL << 46, 1UL << 53);      // En passant left
    requested_moves[16] = new Move('p', 1UL << 44, 1UL << 52);      // Advance 1
    requested_moves[17] = new Move('p', 1UL << 46, 1UL << 54);      // Advance 1
    success_count += find_moves_RegularCases_FindAllMoves(
        2, new Board("4Q3/1ppp1ppp/2Q3p1/3Q3p/8/4pPp1/p7/8 b - f2 0 1"), requested_moves, 18
    );

    return success_count;
}
int find_knight_moves_testLauncher()
{
    int   success_count = 0;
    Move *requested_moves[20];

    // Left up corner
    requested_moves[0] = new Move('N', 1UL, 1UL << 10);
    requested_moves[1] = new Move('N', 1UL, 1UL << 17);
    success_count += find_moves_RegularCases_FindAllMoves(
        3, new Board("N7/8/8/8/8/8/8/8 w - - 0 1"), requested_moves, 2
    );

    // Right up corner
    requested_moves[0] = new Move('N', 1UL << 7, 1UL << 13);
    requested_moves[1] = new Move('N', 1UL << 7, 1UL << 22);
    success_count += find_moves_RegularCases_FindAllMoves(
        4, new Board("7N/8/8/8/8/8/8/8 w - - 0 1"), requested_moves, 2
    );

    // Left down corner
    requested_moves[0] = new Move('n', 1UL << 56, 1UL << 41);
    requested_moves[1] = new Move('n', 1UL << 56, 1UL << 50);
    success_count += find_moves_RegularCases_FindAllMoves(
        5, new Board("8/8/8/8/8/8/8/n7 b - - 0 1"), requested_moves, 2
    );

    // Right down corner
    requested_moves[0] = new Move('n', 1UL << 63, 1UL << 46);
    requested_moves[1] = new Move('n', 1UL << 63, 1UL << 53);
    success_count += find_moves_RegularCases_FindAllMoves(
        6, new Board("8/8/8/8/8/8/8/7n b - - 0 1"), requested_moves, 2
    );

    // White - All move types
    requested_moves[0] = new Move('N', 1UL << 27, 1UL << 10); // Up left
    requested_moves[1] = new Move('N', 1UL << 27, 1UL << 12); // Up right
    requested_moves[2] = new Move('N', 1UL << 27, 1UL << 17); // Left up
    requested_moves[3] = new Move('N', 1UL << 27, 1UL << 21); // Right up
    requested_moves[4] = new Move('N', 1UL << 27, 1UL << 33); // Left down
    requested_moves[5] = new Move('N', 1UL << 27, 1UL << 37); // Right down
    requested_moves[6] = new Move('N', 1UL << 27, 1UL << 42); // Down left (Capture)
    requested_moves[7] = new Move('P', 1UL << 44, 1UL << 36); // Pawn blocker move
    success_count += find_moves_RegularCases_FindAllMoves(
        6, new Board("8/8/8/3N4/8/2q1P3/8/8 w - - 0 1"), requested_moves, 8
    );

    // Black - All move types
    requested_moves[0] = new Move('n', 1UL << 27, 1UL << 10); // Up left
    requested_moves[1] = new Move('n', 1UL << 27, 1UL << 12); // Up right
    requested_moves[2] = new Move('n', 1UL << 27, 1UL << 17); // Left up
    requested_moves[3] = new Move('n', 1UL << 27, 1UL << 21); // Right up
    requested_moves[4] = new Move('n', 1UL << 27, 1UL << 33); // Left down
    requested_moves[5] = new Move('n', 1UL << 27, 1UL << 37); // Right down
    requested_moves[6] = new Move('n', 1UL << 27, 1UL << 42); // Down left (Capture)
    requested_moves[7] = new Move('p', 1UL << 44, 1UL << 52); // Pawn blocker move
    success_count += find_moves_RegularCases_FindAllMoves(
        7, new Board("8/8/8/3n4/8/2Q1p3/8/8 b - - 0 1"), requested_moves, 8
    );

    return success_count;
}
int find_bishop_moves_testLauncher()
{
    int   success_count = 0;
    Move *requested_moves[20];

    // White - All move types
    requested_moves[0] = new Move('B', 1UL << 27, 1UL << 18);
    requested_moves[1] = new Move('B', 1UL << 27, 1UL << 9);
    requested_moves[2] = new Move('B', 1UL << 27, 1UL << 0);
    requested_moves[3] = new Move('B', 1UL << 27, 1UL << 20);
    requested_moves[4] =
        new Move('B', 1UL << 27, 1UL << 13); // Following cell is blocked by ally pawn
    requested_moves[5] = new Move('B', 1UL << 27, 1UL << 36);
    requested_moves[6] = new Move('B', 1UL << 27, 1UL << 45);
    requested_moves[7] = new Move('B', 1UL << 27, 1UL << 54);
    requested_moves[8] = new Move('B', 1UL << 27, 1UL << 63);
    requested_moves[9] = new Move('B', 1UL << 27, 1UL << 34);
    requested_moves[10] = new Move('B', 1UL << 27, 1UL << 41); // Capture enemy pawn
    success_count += find_moves_RegularCases_FindAllMoves(
        8, new Board("6P1/8/8/3B4/8/1p6/8/8 w - - 0 1"), requested_moves, 11
    );

    // Black - All move types
    requested_moves[0] = new Move('b', 1UL << 27, 1UL << 18);
    requested_moves[1] = new Move('b', 1UL << 27, 1UL << 9);
    requested_moves[2] = new Move('b', 1UL << 27, 1UL << 0);
    requested_moves[3] = new Move('b', 1UL << 27, 1UL << 20);
    requested_moves[4] =
        new Move('b', 1UL << 27, 1UL << 13); // Following cell is blocked by ally pawn
    requested_moves[5] = new Move('b', 1UL << 27, 1UL << 36);
    requested_moves[6] = new Move('b', 1UL << 27, 1UL << 45);
    requested_moves[7] = new Move('b', 1UL << 27, 1UL << 54);
    requested_moves[8] = new Move('b', 1UL << 27, 1UL << 63);
    requested_moves[9] = new Move('b', 1UL << 27, 1UL << 34);
    requested_moves[10] = new Move('b', 1UL << 27, 1UL << 41); // Capture enemy pawn
    requested_moves[11] = new Move('p', 1UL << 6, 1UL << 14);  // Blocker pawn move
    success_count += find_moves_RegularCases_FindAllMoves(
        9, new Board("6p1/8/8/3b4/8/1P6/8/8 b - - 0 1"), requested_moves, 12
    );

    return success_count;
}
int find_rook_moves_testLauncher()
{
    int   success_count = 0;
    Move *requested_moves[20];

    // White - All move types
    requested_moves[0] = new Move('R', 1UL << 27, 1UL << 11);
    requested_moves[1] = new Move('R', 1UL << 27, 1UL << 19);
    requested_moves[2] = new Move('R', 1UL << 27, 1UL << 24);
    requested_moves[3] = new Move('R', 1UL << 27, 1UL << 25);
    requested_moves[4] = new Move('R', 1UL << 27, 1UL << 26);
    requested_moves[5] = new Move('R', 1UL << 27, 1UL << 28);
    requested_moves[6] = new Move('R', 1UL << 27, 1UL << 29);
    requested_moves[7] = new Move('R', 1UL << 27, 1UL << 30); // Capture enemy pawn
    requested_moves[8] = new Move('R', 1UL << 27, 1UL << 35);
    requested_moves[9] = new Move('R', 1UL << 27, 1UL << 43);
    requested_moves[10] = new Move('R', 1UL << 27, 1UL << 51);
    requested_moves[11] = new Move('R', 1UL << 27, 1UL << 59);
    success_count += find_moves_RegularCases_FindAllMoves(
        10, new Board("3P4/8/8/3R2p1/8/8/8/8 w - - 0 1"), requested_moves, 12
    );

    // Black - All move types
    requested_moves[0] = new Move('r', 1UL << 27, 1UL << 11);
    requested_moves[1] = new Move('r', 1UL << 27, 1UL << 19);
    requested_moves[2] = new Move('r', 1UL << 27, 1UL << 24);
    requested_moves[3] = new Move('r', 1UL << 27, 1UL << 25);
    requested_moves[4] = new Move('r', 1UL << 27, 1UL << 26);
    requested_moves[5] = new Move('r', 1UL << 27, 1UL << 28);
    requested_moves[6] = new Move('r', 1UL << 27, 1UL << 29);
    requested_moves[7] = new Move('r', 1UL << 27, 1UL << 30); // Capture enemy pawn
    requested_moves[8] = new Move('r', 1UL << 27, 1UL << 35);
    requested_moves[9] = new Move('r', 1UL << 27, 1UL << 43);
    requested_moves[10] = new Move('r', 1UL << 27, 1UL << 51);
    requested_moves[11] = new Move('r', 1UL << 27, 1UL << 59);
    requested_moves[12] = new Move('p', 1UL << 3, 1UL << 11); // Pawn blocker move advance
    success_count += find_moves_RegularCases_FindAllMoves(
        10, new Board("3p4/8/8/3r2P1/8/8/8/8 b - - 0 1"), requested_moves, 13
    );

    return success_count;
}
int find_queen_moves_testLauncher()
{
    int   success_count = 0;
    Move *requested_moves[25];

    // White - All move types
    requested_moves[0] = new Move('Q', 1UL << 25, 1UL << 17);
    requested_moves[1] = new Move('Q', 1UL << 25, 1UL << 9); // Next cell is blocked
    requested_moves[2] = new Move('Q', 1UL << 25, 1UL << 18);
    requested_moves[3] = new Move('Q', 1UL << 25, 1UL << 11);
    requested_moves[4] = new Move('Q', 1UL << 25, 1UL << 4);
    requested_moves[5] = new Move('Q', 1UL << 25, 1UL << 26);
    requested_moves[6] = new Move('Q', 1UL << 25, 1UL << 27);
    requested_moves[7] = new Move('Q', 1UL << 25, 1UL << 28);
    requested_moves[8] = new Move('Q', 1UL << 25, 1UL << 29);
    requested_moves[9] = new Move('Q', 1UL << 25, 1UL << 30); // Capture enemy pawn
    requested_moves[10] = new Move('Q', 1UL << 25, 1UL << 34);
    requested_moves[11] = new Move('Q', 1UL << 25, 1UL << 43);
    requested_moves[12] = new Move('Q', 1UL << 25, 1UL << 52);
    requested_moves[13] = new Move('Q', 1UL << 25, 1UL << 61);
    requested_moves[14] = new Move('Q', 1UL << 25, 1UL << 33);
    requested_moves[15] = new Move('Q', 1UL << 25, 1UL << 41);
    requested_moves[16] = new Move('Q', 1UL << 25, 1UL << 49);
    requested_moves[17] = new Move('Q', 1UL << 25, 1UL << 57);
    requested_moves[18] = new Move('Q', 1UL << 25, 1UL << 32);
    requested_moves[19] = new Move('Q', 1UL << 25, 1UL << 24);
    requested_moves[20] = new Move('Q', 1UL << 25, 1UL << 16);
    success_count += find_moves_RegularCases_FindAllMoves(
        10, new Board("1P6/8/8/1Q4p1/8/8/8/8 w - - 0 1"), requested_moves, 21
    );

    // Black - All move types
    requested_moves[0] = new Move('q', 1UL << 25, 1UL << 17);
    requested_moves[1] = new Move('q', 1UL << 25, 1UL << 9); // Next cell is blocked
    requested_moves[2] = new Move('q', 1UL << 25, 1UL << 18);
    requested_moves[3] = new Move('q', 1UL << 25, 1UL << 11);
    requested_moves[4] = new Move('q', 1UL << 25, 1UL << 4);
    requested_moves[5] = new Move('q', 1UL << 25, 1UL << 26);
    requested_moves[6] = new Move('q', 1UL << 25, 1UL << 27);
    requested_moves[7] = new Move('q', 1UL << 25, 1UL << 28);
    requested_moves[8] = new Move('q', 1UL << 25, 1UL << 29);
    requested_moves[9] = new Move('q', 1UL << 25, 1UL << 30); // Capture enemy pawn
    requested_moves[10] = new Move('q', 1UL << 25, 1UL << 34);
    requested_moves[11] = new Move('q', 1UL << 25, 1UL << 43);
    requested_moves[12] = new Move('q', 1UL << 25, 1UL << 52);
    requested_moves[13] = new Move('q', 1UL << 25, 1UL << 61);
    requested_moves[14] = new Move('q', 1UL << 25, 1UL << 33);
    requested_moves[15] = new Move('q', 1UL << 25, 1UL << 41);
    requested_moves[16] = new Move('q', 1UL << 25, 1UL << 49);
    requested_moves[17] = new Move('q', 1UL << 25, 1UL << 57);
    requested_moves[18] = new Move('q', 1UL << 25, 1UL << 32);
    requested_moves[19] = new Move('q', 1UL << 25, 1UL << 24);
    requested_moves[20] = new Move('q', 1UL << 25, 1UL << 16);
    requested_moves[21] = new Move('p', 1UL << 1, 1UL << 9);
    success_count += find_moves_RegularCases_FindAllMoves(
        10, new Board("1p6/8/8/1q4P1/8/8/8/8 b - - 0 1"), requested_moves, 22
    );

    return success_count;
}
int find_king_moves_testLauncher()
{
    int   success_count = 0;
    Move *requested_moves[10];

    // White - All move types (King cannot move on attacked squares)
    requested_moves[0] = new Move('K', 1UL << 51, 1UL << 43, 0, NOTCASTLE); // N
    requested_moves[1] = new Move('K', 1UL << 51, 1UL << 44, 0, NOTCASTLE); // NE
    requested_moves[2] = new Move('K', 1UL << 51, 1UL << 60, 0, NOTCASTLE); // SE
    requested_moves[3] = new Move('K', 1UL << 51, 1UL << 58, 0, NOTCASTLE); // SW
    requested_moves[4] = new Move('K', 1UL << 51, 1UL << 50, 0, NOTCASTLE); // W
    requested_moves[5] = new Move('K', 1UL << 51, 1UL << 42, 0, NOTCASTLE); // NW
    requested_moves[6] = new Move('P', 1UL << 52, 1UL << 44);               // Pawn move
    requested_moves[7] = new Move('P', 1UL << 52, 1UL << 36);               // Pawn move
    success_count += find_moves_RegularCases_FindAllMoves(
        11, new Board("8/8/8/8/8/8/2pKP3/8 w - - 0 1"), requested_moves, 8
    );

    // Black - All move types (King cannot move on attacked squares)
    requested_moves[0] = new Move('k', 1UL << 9, 1UL << 2, 0, NOTCASTLE);  // NE
    requested_moves[1] = new Move('k', 1UL << 9, 1UL << 10, 0, NOTCASTLE); // E
    requested_moves[2] = new Move('k', 1UL << 9, 1UL << 18, 0, NOTCASTLE); // SE
    requested_moves[3] = new Move('k', 1UL << 9, 1UL << 17, 0, NOTCASTLE); // S
    requested_moves[4] = new Move('k', 1UL << 9, 1UL << 16, 0, NOTCASTLE); // SW
    requested_moves[5] = new Move('k', 1UL << 9, 1UL << 0, 0, NOTCASTLE);  // NW
    requested_moves[6] = new Move('p', 1UL << 8, 1UL << 16);               // Pawn move
    requested_moves[7] = new Move('p', 1UL << 8, 1UL << 24);               // Pawn move
    success_count += find_moves_RegularCases_FindAllMoves(
        12, new Board("8/pkP5/8/8/8/8/8/8 b - - 0 1"), requested_moves, 8
    );

    return success_count;
}

int find_moves_ckecks_testLauncher()
{
    int   success_count = 0;
    Move *requested_moves[15];

    // Pieces cannot move if they aren't unchecking the king - White
    requested_moves[0] = new Move('K', 1UL << 17, 1UL << 8, 0, NOTCASTLE);  // King NW
    requested_moves[1] = new Move('K', 1UL << 17, 1UL << 26, 0, NOTCASTLE); // King SE
    requested_moves[2] = new Move('K', 1UL << 17, 1UL << 25, 0, NOTCASTLE); // King S
    requested_moves[3] = new Move('K', 1UL << 17, 1UL << 24, 0, NOTCASTLE); // King SW
    requested_moves[4] = new Move('N', 1UL << 9, 1UL << 19);                // Knight block
    requested_moves[5] = new Move('Q', 1UL << 5, 1UL << 19);                // Queen block
    requested_moves[6] = new Move('Q', 1UL << 5, 1UL << 21);                // Queen block 2
    requested_moves[7] = new Move('B', 1UL << 27, 1UL << 18);               // Bishop block
    requested_moves[8] = new Move('B', 1UL << 27, 1UL << 20);               // Bishop block
    requested_moves[9] = new Move('P', 1UL << 29, 1UL << 21);               // Pawn block
    requested_moves[10] = new Move('P', 1UL << 29, 1UL << 22);              // Pawn capture
    success_count += find_moves_RegularCases_FindAllMoves(
        13, new Board("5Q2/1N4r1/1K4r1/3B1Pr1/7R/8/8/8 w - - 0 1"), requested_moves, 11
    );

    // Pieces cannot move if they aren't unchecking the king - Black
    requested_moves[0] = new Move('k', 1UL << 17, 1UL << 8, 0, NOTCASTLE);  // King NW
    requested_moves[1] = new Move('k', 1UL << 17, 1UL << 26, 0, NOTCASTLE); // King SE
    requested_moves[2] = new Move('k', 1UL << 17, 1UL << 25, 0, NOTCASTLE); // King S
    requested_moves[3] = new Move('k', 1UL << 17, 1UL << 24, 0, NOTCASTLE); // King SW
    requested_moves[4] = new Move('n', 1UL << 9, 1UL << 19);                // Knight block
    requested_moves[5] = new Move('q', 1UL << 5, 1UL << 19);                // Queen block
    requested_moves[6] = new Move('q', 1UL << 5, 1UL << 21);                // Queen block 2
    requested_moves[7] = new Move('b', 1UL << 27, 1UL << 18);               // Bishop block
    requested_moves[8] = new Move('b', 1UL << 27, 1UL << 20);               // Bishop block
    requested_moves[9] = new Move('p', 1UL << 15, 1UL << 22);               // Pawn capture
    success_count += find_moves_RegularCases_FindAllMoves(
        14, new Board("5q2/1n4Rp/1k4R1/3b2R1/7r/8/8/8 b - - 0 1"), requested_moves, 10
    );

    // Only the king can move if there are 2 checks - White
    requested_moves[0] = new Move('K', 1UL << 17, 1UL << 25, 0, NOTCASTLE); // King S
    requested_moves[1] = new Move('K', 1UL << 17, 1UL << 24, 0, NOTCASTLE); // King SW
    success_count += find_moves_RegularCases_FindAllMoves(
        15, new Board("5Q2/1N4r1/1K4r1/3B1Pr1/3b3R/8/8/8 w - - 0 1"), requested_moves, 2
    );

    // Only the king can move if there are 2 checks - Black
    requested_moves[0] = new Move('k', 1UL << 17, 1UL << 25, 0, NOTCASTLE); // King S
    requested_moves[1] = new Move('k', 1UL << 17, 1UL << 24, 0, NOTCASTLE); // King SW
    success_count += find_moves_RegularCases_FindAllMoves(
        16, new Board("5q2/1n4Rp/1k4R1/3b2R1/3B3r/8/8/8 b - - 0 1"), requested_moves, 2
    );

    // Take en-passant to uncheck the king - White
    requested_moves[0] = new Move('K', 1UL << 32, 1UL << 25, 0, NOTCASTLE); // King captures
    requested_moves[1] = new Move('K', 1UL << 32, 1UL << 41, 0, NOTCASTLE); // King SE
    requested_moves[2] = new Move('K', 1UL << 32, 1UL << 40, 0, NOTCASTLE); // King S
    requested_moves[3] = new Move('P', 1UL << 24, 1UL << 17, 0, NOTCASTLE); // Pawn takes en-passant
    success_count += find_moves_RegularCases_FindAllMoves(
        160, new Board("k7/8/5r2/Ppp5/K7/3n1pP1/8/8 w - b6 0 88"), requested_moves, 4
    );
    // Take en-passant to uncheck the king - White - Except if an hidding diagonal sliding piece is
    // behind the en-passant pawn !..
    requested_moves[0] = new Move('K', 1UL << 32, 1UL << 41, 0, NOTCASTLE); // King SE
    requested_moves[1] = new Move('K', 1UL << 32, 1UL << 40, 0, NOTCASTLE); // King S
    success_count += find_moves_RegularCases_FindAllMoves(
        161, new Board("k7/3b4/5r2/Ppp5/K7/3n1pP1/8/8 w - b6 0 88"), requested_moves, 2
    );

    // Take en-passant to uncheck the king - Black
    requested_moves[0] = new Move('k', 1UL << 24, 1UL << 33, 0, NOTCASTLE); // King captures
    requested_moves[1] = new Move('k', 1UL << 24, 1UL << 17, 0, NOTCASTLE); // King NE
    requested_moves[2] = new Move('k', 1UL << 24, 1UL << 16, 0, NOTCASTLE); // King N
    requested_moves[3] = new Move('p', 1UL << 32, 1UL << 41, 0, NOTCASTLE); // Pawn takes en-passant
    success_count += find_moves_RegularCases_FindAllMoves(
        162, new Board("8/8/3N1Pp1/k7/pPP5/5R2/8/K7 b - b3 0 88"), requested_moves, 4
    );
    // Take en-passant to uncheck the king - Black - Except if an hidding diagonal sliding piece is
    // behind the en-passant pawn !..
    requested_moves[0] = new Move('k', 1UL << 24, 1UL << 17, 0, NOTCASTLE); // King NE
    requested_moves[1] = new Move('k', 1UL << 24, 1UL << 16, 0, NOTCASTLE); // King N
    success_count += find_moves_RegularCases_FindAllMoves(
        163, new Board("8/8/3N1Pp1/k7/pPP5/5R2/3B4/K7 b - b3 0 88"), requested_moves, 2
    );

    return success_count;
}

int find_moves_not_unpinning_testLauncher()
{
    int   success_count = 0;
    Move *requested_moves[10];

    // Only the king can move if there are 2 checks - White
    requested_moves[0] = new Move('K', 1UL << 17, 1UL << 16, 0, NOTCASTLE); // King W
    requested_moves[1] = new Move('K', 1UL << 17, 1UL << 24, 0, NOTCASTLE); // King SW
    requested_moves[2] = new Move('B', 1UL << 10, 1UL << 3);  // Pinned bishop captures
    requested_moves[3] = new Move('Q', 1UL << 26, 1UL << 35); // Pinned queen captures
    requested_moves[4] = new Move('R', 1UL << 25, 1UL << 33); // Pinned rook captures
    success_count += find_moves_RegularCases_FindAllMoves(
        17, new Board("1q1b4/1NB5/1KPr4/1RQ5/1r1b4/8/8/8 w - - 0 1"), requested_moves, 5
    );

    // Only the king can move if there are 2 checks - Black
    requested_moves[0] = new Move('k', 1UL << 17, 1UL << 16, 0, NOTCASTLE); // King W
    requested_moves[1] = new Move('k', 1UL << 17, 1UL << 24, 0, NOTCASTLE); // King SW
    requested_moves[2] = new Move('b', 1UL << 10, 1UL << 3);  // Pinned bishop captures
    requested_moves[3] = new Move('q', 1UL << 26, 1UL << 35); // Pinned queen captures
    requested_moves[4] = new Move('r', 1UL << 25, 1UL << 33); // Pinned rook captures
    success_count += find_moves_RegularCases_FindAllMoves(
        18, new Board("1Q1B4/1nb5/1kpR4/1rq5/1R1B4/8/8/8 b - - 0 1"), requested_moves, 5
    );

    return success_count;
}

int find_moves_castles_testLauncher()
{
    int   success_count = 0;
    Move *requested_moves[20];

    /* VALID CASTLES */

    // Black - Standard castle
    requested_moves[0] = new Move('k', 1UL << 4, 1UL << 5, 0, NOTCASTLE);  // King E
    requested_moves[1] = new Move('k', 1UL << 4, 1UL << 13, 0, NOTCASTLE); // King SE
    requested_moves[2] = new Move('k', 1UL << 4, 1UL << 12, 0, NOTCASTLE); // King S
    requested_moves[3] = new Move('k', 1UL << 4, 1UL << 11, 0, NOTCASTLE); // King SW
    requested_moves[4] = new Move('k', 1UL << 4, 1UL << 3, 0, NOTCASTLE);  // King W
    requested_moves[5] = new Move('k', 1UL << 4, 1UL << 0, 0, BLACKLEFT);
    requested_moves[6] = new Move('r', 1UL << 0, 1UL << 1);   // Rook 1
    requested_moves[7] = new Move('r', 1UL << 0, 1UL << 2);   // Rook 2
    requested_moves[8] = new Move('r', 1UL << 0, 1UL << 3);   // Rook 3
    requested_moves[9] = new Move('r', 1UL << 0, 1UL << 8);   // Rook 4
    requested_moves[10] = new Move('r', 1UL << 0, 1UL << 16); // Rook capture
    requested_moves[11] = new Move('k', 1UL << 4, 1UL << 7, 0, BLACKRIGHT);
    requested_moves[12] = new Move('r', 1UL << 7, 1UL << 6);  // Rook 6
    requested_moves[13] = new Move('r', 1UL << 7, 1UL << 5);  // Rook 5
    requested_moves[14] = new Move('r', 1UL << 7, 1UL << 15); // Rook 6
    requested_moves[15] = new Move('r', 1UL << 7, 1UL << 23); // Rook capture
    success_count += find_moves_RegularCases_FindAllMoves(
        19, new Board("r3k2r/8/P6P/8/8/8/8/8 b ah - 0 1"), requested_moves, 16
    );
    // Black - Most right position
    requested_moves[0] = new Move('k', 1UL << 6, 1UL << 7, 0, NOTCASTLE);  // King E
    requested_moves[1] = new Move('k', 1UL << 6, 1UL << 15, 0, NOTCASTLE); // King SE
    requested_moves[2] = new Move('k', 1UL << 6, 1UL << 13, 0, NOTCASTLE); // King SW
    requested_moves[3] = new Move('r', 1UL << 5, 1UL << 0);                // Rook
    requested_moves[4] = new Move('r', 1UL << 5, 1UL << 1);                // Rook
    requested_moves[5] = new Move('r', 1UL << 5, 1UL << 2);                // Rook
    requested_moves[6] = new Move('r', 1UL << 5, 1UL << 3);                // Rook
    requested_moves[7] = new Move('r', 1UL << 5, 1UL << 4);                // Rook
    requested_moves[8] = new Move('r', 1UL << 5, 1UL << 13);               // Rook capture
    requested_moves[9] = new Move('r', 1UL << 5, 1UL << 21);               // Rook capture
    requested_moves[10] = new Move('k', 1UL << 6, 1UL << 5, 0, BLACKLEFT); // King
    success_count += find_moves_RegularCases_FindAllMoves(
        20, new Board("5rk1/8/5P2/8/8/8/8/8 b f - 0 1"), requested_moves, 11
    );
    // Black - Most left position
    requested_moves[0] = new Move('k', 1UL << 1, 1UL << 10, 0, NOTCASTLE);  // King SE
    requested_moves[1] = new Move('k', 1UL << 1, 1UL << 8, 0, NOTCASTLE);   // King SW
    requested_moves[2] = new Move('k', 1UL << 1, 1UL << 0, 0, NOTCASTLE);   // King W
    requested_moves[3] = new Move('r', 1UL << 2, 1UL << 10);                // Rook
    requested_moves[4] = new Move('r', 1UL << 2, 1UL << 18);                // Rook capture
    requested_moves[5] = new Move('r', 1UL << 2, 1UL << 3);                 // Rook
    requested_moves[6] = new Move('r', 1UL << 2, 1UL << 4);                 // Rook
    requested_moves[7] = new Move('r', 1UL << 2, 1UL << 5);                 // Rook
    requested_moves[8] = new Move('r', 1UL << 2, 1UL << 6);                 // Rook
    requested_moves[9] = new Move('r', 1UL << 2, 1UL << 7);                 // Rook
    requested_moves[10] = new Move('k', 1UL << 1, 1UL << 2, 0, BLACKRIGHT); // King
    success_count += find_moves_RegularCases_FindAllMoves(
        21, new Board("1kr5/8/2P5/8/8/8/8/8 b ac - 0 1"), requested_moves, 11
    );
    // Black - Left - On their actual cells
    requested_moves[0] = new Move('k', 1UL << 3, 1UL << 4, 0, NOTCASTLE);  // King E
    requested_moves[1] = new Move('k', 1UL << 3, 1UL << 12, 0, NOTCASTLE); // King SE
    requested_moves[2] = new Move('k', 1UL << 3, 1UL << 10, 0, NOTCASTLE); // King SW
    requested_moves[3] = new Move('r', 1UL << 2, 1UL << 10);               // Rook
    requested_moves[4] = new Move('r', 1UL << 2, 1UL << 18);               // Rook capture
    requested_moves[5] = new Move('r', 1UL << 2, 1UL << 1);                // Rook
    requested_moves[6] = new Move('r', 1UL << 2, 1UL << 0);                // Rook
    requested_moves[7] = new Move('k', 1UL << 3, 1UL << 2, 0, BLACKLEFT);  // King
    success_count += find_moves_RegularCases_FindAllMoves(
        210, new Board("2rk4/8/2P5/8/8/8/8/8 b c - 0 1"), requested_moves, 8
    );
    // Black - Right - On their actual cells
    requested_moves[0] = new Move('k', 1UL << 5, 1UL << 4, 0, NOTCASTLE);  // King W
    requested_moves[1] = new Move('k', 1UL << 5, 1UL << 12, 0, NOTCASTLE); // King SW
    requested_moves[2] = new Move('k', 1UL << 5, 1UL << 14, 0, NOTCASTLE); // King SE
    requested_moves[3] = new Move('r', 1UL << 6, 1UL << 14);               // Rook
    requested_moves[4] = new Move('r', 1UL << 6, 1UL << 22);               // Rook capture
    requested_moves[5] = new Move('r', 1UL << 6, 1UL << 7);                // Rook
    requested_moves[6] = new Move('k', 1UL << 5, 1UL << 6, 0, BLACKRIGHT); // King
    success_count += find_moves_RegularCases_FindAllMoves(
        211, new Board("5kr1/8/6P1/8/8/8/8/8 b g - 0 1"), requested_moves, 7
    );

    // White - Standard castle
    requested_moves[0] = new Move('K', 1UL << 60, 1UL << 61, 0, NOTCASTLE); // King E
    requested_moves[1] = new Move('K', 1UL << 60, 1UL << 59, 0, NOTCASTLE); // King W
    requested_moves[2] = new Move('K', 1UL << 60, 1UL << 51, 0, NOTCASTLE); // King NW
    requested_moves[3] = new Move('K', 1UL << 60, 1UL << 52, 0, NOTCASTLE); // King N
    requested_moves[4] = new Move('K', 1UL << 60, 1UL << 53, 0, NOTCASTLE); // King NE
    requested_moves[5] = new Move('K', 1UL << 60, 1UL << 56, 0, WHITELEFT);
    requested_moves[6] = new Move('R', 1UL << 56, 1UL << 57);  // Rook 1
    requested_moves[7] = new Move('R', 1UL << 56, 1UL << 58);  // Rook 2
    requested_moves[8] = new Move('R', 1UL << 56, 1UL << 59);  // Rook 3
    requested_moves[9] = new Move('R', 1UL << 56, 1UL << 48);  // Rook 4
    requested_moves[10] = new Move('R', 1UL << 56, 1UL << 40); // Rook capture
    requested_moves[11] = new Move('K', 1UL << 60, 1UL << 63, 0, WHITERIGHT);
    requested_moves[12] = new Move('R', 1UL << 63, 1UL << 62); // Rook 1
    requested_moves[13] = new Move('R', 1UL << 63, 1UL << 61); // Rook 2
    requested_moves[14] = new Move('R', 1UL << 63, 1UL << 55); // Rook 3
    requested_moves[15] = new Move('R', 1UL << 63, 1UL << 47); // Rook capture
    success_count += find_moves_RegularCases_FindAllMoves(
        22, new Board("8/8/8/8/8/p6p/8/R3K2R w AH - 0 1"), requested_moves, 16
    );
    // White - Most right position
    requested_moves[0] = new Move('K', 1UL << 62, 1UL << 53, 0, NOTCASTLE);  // King NW
    requested_moves[1] = new Move('K', 1UL << 62, 1UL << 55, 0, NOTCASTLE);  // King NE
    requested_moves[2] = new Move('R', 1UL << 63, 1UL << 55);                // Rook N
    requested_moves[3] = new Move('R', 1UL << 63, 1UL << 47);                // Rook capture
    requested_moves[4] = new Move('R', 1UL << 61, 1UL << 53);                // Rook N
    requested_moves[5] = new Move('R', 1UL << 61, 1UL << 45);                // Rook capture
    requested_moves[6] = new Move('R', 1UL << 61, 1UL << 60);                // Rook
    requested_moves[7] = new Move('R', 1UL << 61, 1UL << 59);                // Rook
    requested_moves[8] = new Move('R', 1UL << 61, 1UL << 58);                // Rook
    requested_moves[9] = new Move('R', 1UL << 61, 1UL << 57);                // Rook
    requested_moves[10] = new Move('R', 1UL << 61, 1UL << 56);               // Rook
    requested_moves[11] = new Move('K', 1UL << 62, 1UL << 61, 0, WHITELEFT); // King NE
    success_count += find_moves_RegularCases_FindAllMoves(
        23, new Board("8/8/8/8/8/5p1p/8/5RKR w FH - 0 1"), requested_moves, 12
    );
    // White - Most left position
    requested_moves[0] = new Move('K', 1UL << 57, 1UL << 48, 0, NOTCASTLE);   // King NW
    requested_moves[1] = new Move('K', 1UL << 57, 1UL << 50, 0, NOTCASTLE);   // King NE
    requested_moves[2] = new Move('R', 1UL << 56, 1UL << 48);                 // Rook 1
    requested_moves[3] = new Move('R', 1UL << 56, 1UL << 40);                 // Rook capture
    requested_moves[4] = new Move('R', 1UL << 58, 1UL << 50);                 // Rook 3
    requested_moves[5] = new Move('R', 1UL << 58, 1UL << 42);                 // Rook capture
    requested_moves[6] = new Move('R', 1UL << 58, 1UL << 59);                 // Rook
    requested_moves[7] = new Move('R', 1UL << 58, 1UL << 60);                 // Rook
    requested_moves[8] = new Move('R', 1UL << 58, 1UL << 61);                 // Rook
    requested_moves[9] = new Move('R', 1UL << 58, 1UL << 62);                 // Rook
    requested_moves[10] = new Move('R', 1UL << 58, 1UL << 63);                // Rook
    requested_moves[11] = new Move('K', 1UL << 57, 1UL << 58, 0, WHITERIGHT); // King NE
    success_count += find_moves_RegularCases_FindAllMoves(
        24, new Board("8/8/8/8/8/p1p5/8/RKR5 w AC - 0 1"), requested_moves, 12
    );
    // White - Left - On their actual cells
    requested_moves[0] = new Move('K', 1UL << 59, 1UL << 52, 0, NOTCASTLE); // King NE
    requested_moves[1] = new Move('K', 1UL << 59, 1UL << 60, 0, NOTCASTLE); // King E
    requested_moves[2] = new Move('K', 1UL << 59, 1UL << 50, 0, NOTCASTLE); // King NW
    requested_moves[3] = new Move('R', 1UL << 58, 1UL << 50);               // Rook
    requested_moves[4] = new Move('R', 1UL << 58, 1UL << 42);               // Rook capture
    requested_moves[5] = new Move('R', 1UL << 58, 1UL << 57);               // Rook
    requested_moves[6] = new Move('R', 1UL << 58, 1UL << 56);               // Rook
    requested_moves[7] = new Move('K', 1UL << 59, 1UL << 58, 0, WHITELEFT); // King
    success_count += find_moves_RegularCases_FindAllMoves(
        241, new Board("8/8/8/8/8/2p5/8/2RK4 w C - 0 1"), requested_moves, 8
    );
    // White - Right - On their actual cells
    requested_moves[0] = new Move('K', 1UL << 61, 1UL << 54, 0, NOTCASTLE);  // King NE
    requested_moves[1] = new Move('K', 1UL << 61, 1UL << 60, 0, NOTCASTLE);  // King W
    requested_moves[2] = new Move('K', 1UL << 61, 1UL << 52, 0, NOTCASTLE);  // King NW
    requested_moves[3] = new Move('R', 1UL << 62, 1UL << 54);                // Rook
    requested_moves[4] = new Move('R', 1UL << 62, 1UL << 46);                // Rook capture
    requested_moves[5] = new Move('R', 1UL << 62, 1UL << 63);                // Rook
    requested_moves[6] = new Move('K', 1UL << 61, 1UL << 62, 0, WHITERIGHT); // King
    success_count += find_moves_RegularCases_FindAllMoves(
        242, new Board("8/8/8/8/8/6p1/8/5KR1 w G - 0 1"), requested_moves, 7
    );

    /* NO RIGHTS */

    // Black - No castle rights
    requested_moves[0] = new Move('k', 1UL << 4, 1UL << 5, 0, NOTCASTLE);  // King E
    requested_moves[1] = new Move('k', 1UL << 4, 1UL << 13, 0, NOTCASTLE); // King SE
    requested_moves[2] = new Move('k', 1UL << 4, 1UL << 12, 0, NOTCASTLE); // King S
    requested_moves[3] = new Move('k', 1UL << 4, 1UL << 11, 0, NOTCASTLE); // King SW
    requested_moves[4] = new Move('k', 1UL << 4, 1UL << 3, 0, NOTCASTLE);  // King W
    requested_moves[5] = new Move('r', 1UL << 0, 1UL << 1);                // Rook 1
    requested_moves[6] = new Move('r', 1UL << 0, 1UL << 2);                // Rook 2
    requested_moves[7] = new Move('r', 1UL << 0, 1UL << 3);                // Rook 3
    requested_moves[8] = new Move('r', 1UL << 0, 1UL << 8);                // Rook 4
    requested_moves[9] = new Move('r', 1UL << 0, 1UL << 16);               // Rook capture
    requested_moves[10] = new Move('r', 1UL << 7, 1UL << 6);               // Rook 6
    requested_moves[11] = new Move('r', 1UL << 7, 1UL << 5);               // Rook 5
    requested_moves[12] = new Move('r', 1UL << 7, 1UL << 15);              // Rook 6
    requested_moves[13] = new Move('r', 1UL << 7, 1UL << 23);              // Rook capture
    success_count += find_moves_RegularCases_FindAllMoves(
        25,
        new Board("r3k2r/8/P6P/8/8/8/8/8 b AH - 0 1"), // These castle rights are for the opponent
        requested_moves, 14
    );
    // White - No castle rights
    requested_moves[0] = new Move('K', 1UL << 60, 1UL << 61, 0, NOTCASTLE); // King E
    requested_moves[1] = new Move('K', 1UL << 60, 1UL << 59, 0, NOTCASTLE); // King W
    requested_moves[2] = new Move('K', 1UL << 60, 1UL << 51, 0, NOTCASTLE); // King NW
    requested_moves[3] = new Move('K', 1UL << 60, 1UL << 52, 0, NOTCASTLE); // King N
    requested_moves[4] = new Move('K', 1UL << 60, 1UL << 53, 0, NOTCASTLE); // King NE
    requested_moves[5] = new Move('R', 1UL << 56, 1UL << 57);               // Rook 1
    requested_moves[6] = new Move('R', 1UL << 56, 1UL << 58);               // Rook 2
    requested_moves[7] = new Move('R', 1UL << 56, 1UL << 59);               // Rook 3
    requested_moves[8] = new Move('R', 1UL << 56, 1UL << 48);               // Rook 4
    requested_moves[9] = new Move('R', 1UL << 56, 1UL << 40);               // Rook capture
    requested_moves[10] = new Move('R', 1UL << 63, 1UL << 62);              // Rook 1
    requested_moves[11] = new Move('R', 1UL << 63, 1UL << 61);              // Rook 2
    requested_moves[12] = new Move('R', 1UL << 63, 1UL << 55);              // Rook 3
    requested_moves[13] = new Move('R', 1UL << 63, 1UL << 47);              // Rook capture
    success_count += find_moves_RegularCases_FindAllMoves(
        26,
        new Board("8/8/8/8/8/p6p/8/R3K2R w ah - 0 1"), // These castle rights are for the opponent
        requested_moves, 14
    );

    /* IN CHECK */

    // Black - King is in check
    requested_moves[0] = new Move('k', 1UL << 4, 1UL << 13, 0, NOTCASTLE); // King SE
    requested_moves[1] = new Move('k', 1UL << 4, 1UL << 12, 0, NOTCASTLE); // King S
    requested_moves[2] = new Move('k', 1UL << 4, 1UL << 11, 0, NOTCASTLE); // King SW
    success_count += find_moves_RegularCases_FindAllMoves(
        27, new Board("r3k2R/8/8/8/8/8/8/8 b a - 0 1"), requested_moves, 3
    );
    // White - King is in check
    requested_moves[0] = new Move('K', 1UL << 60, 1UL << 51, 0, NOTCASTLE); // King NW
    requested_moves[1] = new Move('K', 1UL << 60, 1UL << 52, 0, NOTCASTLE); // King N
    requested_moves[2] = new Move('K', 1UL << 60, 1UL << 53, 0, NOTCASTLE); // King NE
    success_count += find_moves_RegularCases_FindAllMoves(
        28, new Board("8/8/8/8/8/8/8/R3K2r w a - 0 1"), requested_moves, 3
    );

    /* KING FINAL CELL IS ATTACKED */

    // Black - Most right position - King final cell is attacked
    requested_moves[0] = new Move('k', 1UL << 6, 1UL << 7, 0, NOTCASTLE);  // King E
    requested_moves[1] = new Move('k', 1UL << 6, 1UL << 15, 0, NOTCASTLE); // King SE
    requested_moves[2] = new Move('k', 1UL << 6, 1UL << 13, 0, NOTCASTLE); // King SW
    requested_moves[3] = new Move('r', 1UL << 5, 1UL << 0);                // Rook
    requested_moves[4] = new Move('r', 1UL << 5, 1UL << 1);                // Rook
    requested_moves[5] = new Move('r', 1UL << 5, 1UL << 2);                // Rook
    requested_moves[6] = new Move('r', 1UL << 5, 1UL << 3);                // Rook
    requested_moves[7] = new Move('r', 1UL << 5, 1UL << 4);                // Rook
    requested_moves[8] = new Move('r', 1UL << 5, 1UL << 13);               // Rook capture
    requested_moves[9] = new Move('r', 1UL << 5, 1UL << 21);               // Rook capture
    success_count += find_moves_RegularCases_FindAllMoves(
        29, new Board("5rk1/8/2R2P2/8/8/8/8/8 b f - 0 1"), requested_moves, 10
    );
    // Black - Most left position - King final cell is attacked
    requested_moves[0] = new Move('k', 1UL << 1, 1UL << 9, 0, NOTCASTLE); // King S
    requested_moves[1] = new Move('r', 1UL << 0, 1UL << 8);               // Rook
    requested_moves[2] = new Move('r', 1UL << 0, 1UL << 16);              // Rook capture
    requested_moves[3] = new Move('r', 1UL << 2, 1UL << 10);              // Rook
    requested_moves[4] = new Move('r', 1UL << 2, 1UL << 18);              // Rook capture
    requested_moves[5] = new Move('r', 1UL << 2, 1UL << 3);               // Rook
    requested_moves[6] = new Move('r', 1UL << 2, 1UL << 4);               // Rook
    requested_moves[7] = new Move('r', 1UL << 2, 1UL << 5);               // Rook
    requested_moves[8] = new Move('r', 1UL << 2, 1UL << 6);               // Rook
    requested_moves[9] = new Move('r', 1UL << 2, 1UL << 7);               // Rook
    success_count += find_moves_RegularCases_FindAllMoves(
        30, new Board("rkr5/8/R1R3R1/8/8/8/8/8 b ac - 0 1"), requested_moves, 10
    );
    // White - Most right position - King final cell is attacked
    // White - Most left position - King final cell is attacked
    requested_moves[0] = new Move('K', 1UL << 57, 1UL << 48, 0, NOTCASTLE); // King NW
    requested_moves[1] = new Move('K', 1UL << 57, 1UL << 50, 0, NOTCASTLE); // King NE
    requested_moves[2] = new Move('R', 1UL << 56, 1UL << 48);               // Rook 1
    requested_moves[3] = new Move('R', 1UL << 56, 1UL << 40);               // Rook capture
    requested_moves[4] = new Move('R', 1UL << 58, 1UL << 50);               // Rook 3
    requested_moves[5] = new Move('R', 1UL << 58, 1UL << 42);               // Rook capture
    requested_moves[6] = new Move('R', 1UL << 58, 1UL << 59);               // Rook
    requested_moves[7] = new Move('R', 1UL << 58, 1UL << 60);               // Rook
    requested_moves[8] = new Move('R', 1UL << 58, 1UL << 61);               // Rook
    requested_moves[9] = new Move('R', 1UL << 58, 1UL << 62);               // Rook
    requested_moves[10] = new Move('R', 1UL << 58, 1UL << 63);              // Rook
    success_count += find_moves_RegularCases_FindAllMoves(
        31, new Board("8/8/8/8/8/p1p3r1/8/RKR5 w AC - 0 1"), requested_moves, 11
    );

    /* PATH IS ATTACKED */

    // Black - King path is attacked
    requested_moves[0] = new Move('k', 1UL << 4, 1UL << 12, 0, NOTCASTLE); // King S
    requested_moves[1] = new Move('r', 1UL << 0, 1UL << 1);                // Rook 1
    requested_moves[2] = new Move('r', 1UL << 0, 1UL << 2);                // Rook 2
    requested_moves[3] = new Move('r', 1UL << 0, 1UL << 3);                // Rook 3
    requested_moves[4] = new Move('r', 1UL << 0, 1UL << 8);                // Rook 4
    requested_moves[5] = new Move('r', 1UL << 0, 1UL << 16);               // Rook capture
    requested_moves[6] = new Move('r', 1UL << 7, 1UL << 6);                // Rook 6
    requested_moves[7] = new Move('r', 1UL << 7, 1UL << 5);                // Rook 5
    requested_moves[8] = new Move('r', 1UL << 7, 1UL << 15);               // Rook 6
    requested_moves[9] = new Move('r', 1UL << 7, 1UL << 23);               // Rook capture
    success_count += find_moves_RegularCases_FindAllMoves(
        32, new Board("r3k2r/8/P2R1R1P/8/8/8/8/8 b ah - 0 1"), requested_moves, 10
    );
    // White - King path is attacked
    requested_moves[0] = new Move('K', 1UL << 60, 1UL << 52, 0, NOTCASTLE); // King N
    requested_moves[1] = new Move('R', 1UL << 56, 1UL << 57);               // Rook 1
    requested_moves[2] = new Move('R', 1UL << 56, 1UL << 58);               // Rook 2
    requested_moves[3] = new Move('R', 1UL << 56, 1UL << 59);               // Rook 3
    requested_moves[4] = new Move('R', 1UL << 56, 1UL << 48);               // Rook 4
    requested_moves[5] = new Move('R', 1UL << 56, 1UL << 40);               // Rook capture
    requested_moves[6] = new Move('R', 1UL << 63, 1UL << 62);               // Rook 1
    requested_moves[7] = new Move('R', 1UL << 63, 1UL << 61);               // Rook 2
    requested_moves[8] = new Move('R', 1UL << 63, 1UL << 55);               // Rook 3
    requested_moves[9] = new Move('R', 1UL << 63, 1UL << 47);               // Rook capture
    success_count += find_moves_RegularCases_FindAllMoves(
        33, new Board("8/8/8/8/8/p2r1r1p/8/R3K2R w AH - 0 1"), requested_moves, 10
    );

    /* PIECE ON PATH */

    // Black - Piece on king path
    requested_moves[0] = new Move('k', 1UL << 4, 1UL << 13, 0, NOTCASTLE); // King SE
    requested_moves[1] = new Move('k', 1UL << 4, 1UL << 12, 0, NOTCASTLE); // King S
    requested_moves[2] = new Move('k', 1UL << 4, 1UL << 11, 0, NOTCASTLE); // King SW
    requested_moves[3] = new Move('r', 1UL << 0, 1UL << 1);                // Rook 1
    requested_moves[4] = new Move('r', 1UL << 0, 1UL << 2);                // Rook 2
    requested_moves[5] = new Move('r', 1UL << 0, 1UL << 8);                // Rook 4
    requested_moves[6] = new Move('r', 1UL << 0, 1UL << 16);               // Rook capture
    requested_moves[7] = new Move('r', 1UL << 7, 1UL << 6);                // Rook 6
    requested_moves[8] = new Move('r', 1UL << 7, 1UL << 15);               // Rook 6
    requested_moves[9] = new Move('r', 1UL << 7, 1UL << 23);               // Rook capture
    requested_moves[10] = new Move('p', 1UL << 3, 1UL << 11);              // Pawn
    requested_moves[11] = new Move('p', 1UL << 5, 1UL << 13);              // Pawn
    success_count += find_moves_RegularCases_FindAllMoves(
        34, new Board("r2pkp1r/8/P6P/8/8/8/8/8 b ah - 0 1"), requested_moves, 12
    );
    // Black - Piece on rook path
    requested_moves[0] = new Move('k', 1UL << 4, 1UL << 5, 0, NOTCASTLE);   // King E
    requested_moves[1] = new Move('k', 1UL << 4, 1UL << 13, 0, NOTCASTLE);  // King SE
    requested_moves[2] = new Move('k', 1UL << 4, 1UL << 12, 0, NOTCASTLE);  // King S
    requested_moves[3] = new Move('k', 1UL << 4, 1UL << 11, 0, NOTCASTLE);  // King SW
    requested_moves[4] = new Move('k', 1UL << 4, 1UL << 3, 0, NOTCASTLE);   // King W
    requested_moves[5] = new Move('r', 1UL << 0, 1UL << 8);                 // Rook 4
    requested_moves[6] = new Move('r', 1UL << 0, 1UL << 16);                // Rook capture
    requested_moves[7] = new Move('r', 1UL << 7, 1UL << 5);                 // Rook 6
    requested_moves[8] = new Move('r', 1UL << 7, 1UL << 6);                 // Rook 6
    requested_moves[9] = new Move('r', 1UL << 7, 1UL << 15);                // Rook 6
    requested_moves[10] = new Move('r', 1UL << 7, 1UL << 23);               // Rook capture
    requested_moves[11] = new Move('p', 1UL << 1, 1UL << 9);                // Pawn
    requested_moves[12] = new Move('k', 1UL << 4, 1UL << 7, 0, BLACKRIGHT); // King castle !
    success_count += find_moves_RegularCases_FindAllMoves(
        35, new Board("rp2k2r/8/P6P/8/8/8/8/8 b ah - 0 1"), requested_moves, 13
    );
    // White - Piece on king path
    requested_moves[0] = new Move('K', 1UL << 60, 1UL << 52, 0, NOTCASTLE); // King N
    requested_moves[1] = new Move('K', 1UL << 60, 1UL << 53, 0, NOTCASTLE); // King NE
    requested_moves[2] = new Move('K', 1UL << 60, 1UL << 61, 0, NOTCASTLE); // King E
    requested_moves[3] = new Move('K', 1UL << 60, 1UL << 59, 0, NOTCASTLE); // King W
    requested_moves[4] = new Move('K', 1UL << 60, 1UL << 51, 0, NOTCASTLE); // King NW
    requested_moves[5] = new Move('R', 1UL << 56, 1UL << 57);               // Rook 1
    requested_moves[6] = new Move('R', 1UL << 56, 1UL << 58);               // Rook 2
    requested_moves[7] = new Move('R', 1UL << 56, 1UL << 59);               // Rook 3
    requested_moves[8] = new Move('R', 1UL << 56, 1UL << 48);               // Rook 4
    requested_moves[9] = new Move('R', 1UL << 56, 1UL << 40);               // Rook capture
    requested_moves[10] = new Move('R', 1UL << 63, 1UL << 62);              // Rook 1
    requested_moves[11] = new Move('R', 1UL << 63, 1UL << 61);              // Rook 2
    requested_moves[12] = new Move('R', 1UL << 63, 1UL << 55);              // Rook 3
    requested_moves[13] = new Move('R', 1UL << 63, 1UL << 47);              // Rook capture
    success_count += find_moves_RegularCases_FindAllMoves(
        36, new Board("8/8/8/8/8/p6p/8/R2pKp1R w AH - 0 1"), requested_moves, 14
    );
    // White - Piece on rook path
    requested_moves[0] = new Move('K', 1UL << 60, 1UL << 52, 0, NOTCASTLE);   // King N
    requested_moves[1] = new Move('K', 1UL << 60, 1UL << 53, 0, NOTCASTLE);   // King NE
    requested_moves[2] = new Move('K', 1UL << 60, 1UL << 61, 0, NOTCASTLE);   // King E
    requested_moves[3] = new Move('K', 1UL << 60, 1UL << 59, 0, NOTCASTLE);   // King W
    requested_moves[4] = new Move('K', 1UL << 60, 1UL << 51, 0, NOTCASTLE);   // King NW
    requested_moves[5] = new Move('R', 1UL << 56, 1UL << 57);                 // Rook 1
    requested_moves[6] = new Move('R', 1UL << 56, 1UL << 48);                 // Rook 4
    requested_moves[7] = new Move('R', 1UL << 56, 1UL << 40);                 // Rook capture
    requested_moves[8] = new Move('R', 1UL << 63, 1UL << 62);                 // Rook 1
    requested_moves[9] = new Move('R', 1UL << 63, 1UL << 61);                 // Rook 2
    requested_moves[10] = new Move('R', 1UL << 63, 1UL << 55);                // Rook 3
    requested_moves[11] = new Move('R', 1UL << 63, 1UL << 47);                // Rook capture
    requested_moves[12] = new Move('K', 1UL << 60, 1UL << 63, 0, WHITERIGHT); // King W
    success_count += find_moves_RegularCases_FindAllMoves(
        36, new Board("8/8/8/8/8/p6p/8/Rp2K2R w AH - 0 1"), requested_moves, 13
    );

    // King doesn't move during the castle
    requested_moves[0] = new Move('k', 1UL << 2, 1UL << 1, 0, BLACKLEFT); // King castle !
    requested_moves[1] = new Move('k', 1UL << 2, 1UL << 9, 0, NOTCASTLE); // King capture
    requested_moves[2] = new Move('r', 1UL << 1, 1UL << 0);               // Rook
    requested_moves[3] = new Move('r', 1UL << 1, 1UL << 9);               // Rook capture
    success_count += find_moves_RegularCases_FindAllMoves(
        37, new Board("1rk5/1R6/2N5/8/8/8/8/8 b b - 0 1"), requested_moves, 4
    );

    // King path is attacked (Castling left but king going right)
    requested_moves[0] = new Move('k', 1UL << 1, 1UL << 8, 0, NOTCASTLE); // King capture
    requested_moves[1] = new Move('r', 1UL << 0, 1UL << 8);               // Rook capture
    success_count += find_moves_RegularCases_FindAllMoves(
        38, new Board("rk6/R7/1N6/8/8/8/8/8 b a - 2 18"), requested_moves, 2
    );

    // Rook is pinned by an enemy sliding piece
    requested_moves[0] = new Move('r', 1UL << 1, 1UL << 0);               // Rook capture
    requested_moves[1] = new Move('k', 1UL << 2, 1UL << 3, 0, NOTCASTLE); // King moves
    success_count += find_moves_RegularCases_FindAllMoves(
        39, new Board("Qrk4/1Q6/8/8/8/8/8/8 b b - 1 20"), requested_moves, 2
    );

    return success_count;
}

#pragma endregion find_moves

#pragma region create_fen

int create_fen_unittest(int testIndex, Board *board, string requested_fen)
{
    string fen = board->create_fen();

    if (fen != requested_fen)
    {
        cerr << "\n---------- Board - create_fen_unittest() - Test " << testIndex
             << " - !!! FAILURE !!! ----------" << endl;
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
        7, new Board("r1bqkbnr/8/8/8/8/8/8/RNBQKB1R w - - 0 1", true),
        "r1bqkbnr/8/8/8/8/8/8/RNBQKB1R w - - 0 1"
    );

    // Pieces test 2. Edge test
    success_count += create_fen_unittest(
        1, new Board("1nb2bn1/8/8/3pP3/8/8/8/1NB2BN1 w - - 0 1", true),
        "1nb2bn1/8/8/3pP3/8/8/8/1NB2BN1 w - - 0 1"
    );

    // Empty board test + castling test
    success_count += create_fen_unittest(
        2, new Board("8/8/8/8/8/8/8/8 b AHah - 1 2", true), "8/8/8/8/8/8/8/8 b AHah - 1 2"
    );

    // Not full castling rights test
    success_count += create_fen_unittest(
        3, new Board("8/8/8/8/8/8/8/8 w Ac - 0 1", true), "8/8/8/8/8/8/8/8 w Ac - 0 1"
    );

    // En passant test
    success_count += create_fen_unittest(
        4, new Board("8/8/8/8/8/8/8/8 w - h7 0 1", true), "8/8/8/8/8/8/8/8 w - h7 0 1"
    );

    // Half turn rule test
    success_count += create_fen_unittest(
        5, new Board("8/8/8/8/8/8/8/8 w - - 12 1", true), "8/8/8/8/8/8/8/8 w - - 12 1"
    );

    // Game turn test
    success_count += create_fen_unittest(
        6, new Board("8/8/8/8/8/8/8/8 w - - 0 101", true), "8/8/8/8/8/8/8/8 w - - 0 101"
    );

    // Empty board test + castling test with standard rules
    success_count += create_fen_unittest(
        8, new Board("8/8/8/8/8/8/8/8 b KQkq - 1 2", false), "8/8/8/8/8/8/8/8 b KQkq - 1 2"
    );

    // Not full castling rights test
    success_count += create_fen_unittest(
        9, new Board("8/8/8/8/8/8/8/8 w Kq - 0 1", false), "8/8/8/8/8/8/8/8 w Kq - 0 1"
    );

    return success_count;
}

#pragma endregion create_fen

#pragma region get_game_state

int get_game_state_unittest(
    int testIndex, Board *board, float requested_game_state, bool lazy_threefold = false
)
{
    float game_state = board->get_game_state(lazy_threefold);

    if (game_state != requested_game_state)
    {
        cerr << "\n---------- Board - get_game_state_unittest() - Test " << testIndex
             << " - !!! FAILURE !!! ----------" << endl;
        cerr << "- Board : " << endl;
        board->log();

        cerr << "\n- Actual get_game_state    : " << game_state << endl;
        cerr << "- Requested get_game_state : " << requested_game_state << endl;

        return 0;
    }

    return 1;
}

int get_game_state_testLauncher()
{
    int success_count = 0;

    // 0 - Fifty-Move rule
    success_count += get_game_state_unittest(
        0, new Board("8/3k4/3p4/8/3P4/3K4/8/8 w - - 98 110"), GAME_CONTINUE
    );

    // 1 - Fifty-Move rule
    success_count +=
        get_game_state_unittest(1, new Board("8/3k4/3p4/8/3P4/3K4/8/8 w - - 99 110"), DRAW);

    // 2 - Game turn max reached - 126 move proc the end game
    success_count +=
        get_game_state_unittest(2, new Board("8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 126"), DRAW);

    // 3 - White wins
    success_count +=
        get_game_state_unittest(3, new Board("k6R/7R/8/8/8/8/8/8 b - - 0 1"), WHITE_WIN);

    // 33 - Black wins
    success_count +=
        get_game_state_unittest(4, new Board("8/8/8/8/8/8/7r/K6r w - - 0 1"), BLACK_WIN);

    // 4 - Stalemate - White turn
    success_count += get_game_state_unittest(5, new Board("6r1/8/8/8/8/8/r7/7K w - - 0 1"), DRAW);

    // 44 - Stalemate - Black turn
    success_count += get_game_state_unittest(6, new Board("k7/7R/8/8/8/8/8/1R6 b - - 0 1"), DRAW);

    // Insufficient material part: Do some move before so the implementation verify if a
    // capture was made before

    // Insufficient material: King vs king
    Board *board = new Board("8/8/6k1/8/8/8/3K4/8 b - - 0 115");
    success_count += get_game_state_unittest(70, board, DRAW);

    // Insufficient material: King vs king
    board = new Board("8/3K4/3q4/8/8/3k4/8/8 w - - 0 1");
    board->apply_move(Move("d7d6"));
    success_count += get_game_state_unittest(7, board, DRAW);

    // Same, but with an extra piece on the board (Game continue)
    board = new Board("8/3K4/3q4/8/8/3k4/8/2p5 w - - 0 1");
    board->apply_move(Move("d7d6"));
    success_count += get_game_state_unittest(8, board, GAME_CONTINUE);

    // Insufficient material: King+knight vs king
    board = new Board("8/3K4/3q4/8/3n4/3k4/8/8 w - - 0 1");
    board->apply_move(Move("d7d6"));
    success_count += get_game_state_unittest(9, board, DRAW);

    // Insufficient material: King+bishop vs king
    board = new Board("3q4/3K4/8/3B4/8/3k4/8/8 w - - 0 1");
    board->apply_move(Move("d7d8"));
    success_count += get_game_state_unittest(10, board, DRAW);

    // Insufficient material: King+bishop vs king+bishop if both bishops are on the same square
    // color.
    board = new Board("3q4/3K4/8/3B4/4b3/3k4/8/8 w - - 0 1");
    board->apply_move(Move("d7d8"));
    success_count += get_game_state_unittest(11, board, DRAW);

    // Insufficient material: Game continue (Bishop vs Knight)
    board = new Board("3Q4/3k4/8/6K1/3B4/8/8/5n2 b - - 0 115");
    board->apply_move(Move("d7d8"));
    success_count += get_game_state_unittest(12, board, GAME_CONTINUE);

    // Insufficient material: Game continue (Knight vs Bishop)
    board = new Board("3Q4/3k4/8/6K1/3N4/8/8/5b2 b - - 0 115");
    board->apply_move(Move("d7d8"));
    success_count += get_game_state_unittest(13, board, GAME_CONTINUE);

    // Insufficient material: Game continue (2 bishops on different square color)
    board = new Board("8/8/3K4/3B4/3b4/3Q4/3k4/8 b - - 0 1");
    board->apply_move(Move("d2d3"));
    success_count += get_game_state_unittest(14, board, GAME_CONTINUE);

    // Insufficient material: Game continue (2 knights)
    board = new Board("8/8/3K4/3N4/3n4/3Q4/3k4/8 b - - 0 1");
    board->apply_move(Move("d2d3"));
    success_count += get_game_state_unittest(15, board, GAME_CONTINUE);

    // Insufficient material: Game continue (Queen)
    board = new Board("8/3K4/3q4/3Q4/8/3k4/8/8 w - - 0 1");
    board->apply_move(Move("d7d6"));
    success_count += get_game_state_unittest(16, board, GAME_CONTINUE);

    // Insufficient material: Game continue (Rook)
    board = new Board("8/3K4/3q4/8/3r4/3k4/8/8 w - - 0 1");
    board->apply_move(Move("d7d6"));
    success_count += get_game_state_unittest(17, board, GAME_CONTINUE);

    // Insufficient material: Game continue (Rook) (No move before)
    board = new Board("8/8/3K4/8/3r4/3k4/8/8 b - - 0 1");
    success_count += get_game_state_unittest(170, board, GAME_CONTINUE);

    // Insufficient material: Game continue (Rook)
    board = new Board("3K4/8/8/3B4/4b3/3k4/8/8 b - - 0 1");
    success_count += get_game_state_unittest(171, board, DRAW);

    // --- Threefold Repetition part ---

    board = new Board("8/3K4/3Q4/8/8/3q4/3k4/8 w - - 0 1");

    // Threefold Repetition rule fails (Only 1 repetition)
    board->apply_move(Move("d6e6")); // White move right
    board->apply_move(Move("d3e3")); // Black move right
    board->apply_move(Move("e6d6")); // White move left
    board->apply_move(Move("e3d3")); // Black move left
    success_count += get_game_state_unittest(18, board, GAME_CONTINUE);

    // Threefold Repetition rule succeed -> 2 repetitions
    board->apply_move(Move("d6e6")); // White move right
    board->apply_move(Move("d3e3")); // Black move right
    board->apply_move(Move("e6d6")); // White move left
    board->apply_move(Move("e3d3")); // Black move left
    success_count += get_game_state_unittest(19, board, DRAW);

    board = new Board("8/3K4/3Q4/8/8/3q4/3k4/8 w - - 5 5");

    // Threefold Repetition rule fails (Only 1 repetition)
    board->apply_move(Move("d6e6")); // White move right
    board->apply_move(Move("d3e3")); // Black move right
    board->apply_move(Move("e6d6")); // White move left
    board->apply_move(Move("e3d3")); // Black move left
    success_count += get_game_state_unittest(180, board, GAME_CONTINUE);

    // Threefold Repetition rule succeed -> 2 repetitions
    board->apply_move(Move("d6e6")); // White move right
    board->apply_move(Move("d3e3")); // Black move right
    board->apply_move(Move("e6d6")); // White move left
    board->apply_move(Move("e3d3")); // Black move left
    success_count += get_game_state_unittest(190, board, DRAW);

    board = new Board("8/3K4/3Q4/8/8/3q4/3k4/8 w - - 10 10");

    // Threefold Repetition rule fails (Only 1 repetition)
    board->apply_move(Move("d6e6")); // White move right
    board->apply_move(Move("d3e3")); // Black move right
    board->apply_move(Move("e6d6")); // White move left
    board->apply_move(Move("e3d3")); // Black move left
    success_count += get_game_state_unittest(1800, board, GAME_CONTINUE);

    // Threefold Repetition rule succeed -> 2 repetitions
    board->apply_move(Move("d6e6")); // White move right
    board->apply_move(Move("d3e3")); // Black move right
    board->apply_move(Move("e6d6")); // White move left
    board->apply_move(Move("e3d3")); // Black move left
    success_count += get_game_state_unittest(1900, board, DRAW);

    // Make sure the lazy evaluation is working too
    success_count += get_game_state_unittest(200, board, DRAW, true);

    // CG int test
    success_count += get_game_state_unittest(
        20, new Board("1rk5/1pnrb2p/2p1b1P1/Q2p1p2/P2P1P2/3n1BP1/1q6/NKR3B1 w b - 6 25"), BLACK_WIN
    );

    // Test game_state with previous moves
    board = new Board("b4r2/r3np2/2p1k2b/p1P1p1Q1/K1P1P2p/3R1B2/1q3P2/1N1R4 b - - 0 48");
    board->apply_move(Move("b2b4"));
    success_count += get_game_state_unittest(21, board, BLACK_WIN);

    // Test coding game special rule where the game is a draw if it exceeds 125 turns
    success_count +=
        get_game_state_unittest(22, new Board("k3NN2/8/8/3b4/8/4K3/8/5n2 w - - 22 128"), DRAW);

    // Test without coding game special rule where the game is a draw if it exceeds 125 turns
    success_count += get_game_state_unittest(
        23, new Board("k3NN2/8/8/3b4/8/4K3/8/5n2 w - - 22 128", true, false), GAME_CONTINUE
    );

    // Threefold Repetition rule test - Bug 06/07/2025 - With weveral moves before
    board = new Board("8/5q2/6r1/3k3K/8/8/2p5/8 w - - 0 60");
    board->apply_move(Move("h5h4"));
    board->apply_move(Move("c2c1n"));
    board->apply_move(Move("h4h5"));
    board->apply_move(Move("f7e8"));
    board->apply_move(Move("h5h4"));
    board->apply_move(Move("e8a8"));
    board->apply_move(Move("h4h5"));
    board->apply_move(Move("a8b8"));
    board->apply_move(Move("h5g6"));
    board->apply_move(Move("b8a8"));
    board->apply_move(Move("g6f7"));
    board->apply_move(Move("a8b8"));

    board->apply_move(Move("f7e7")); // First seen

    board->apply_move(Move("b8a8"));
    board->apply_move(Move("e7d7"));
    board->apply_move(Move("a8b8"));
    board->apply_move(Move("d7e7")); // Second seen

    board->apply_move(Move("b8a8"));
    board->apply_move(Move("e7f7"));
    success_count += get_game_state_unittest(24, board, GAME_CONTINUE);

    // Threefold Repetition rule test - Bug 06/07/2025 - With half move = 0
    board = new Board("1q6/8/6K1/3k4/8/8/8/2n5 b - - 0 60");

    board->apply_move(Move("b8a8"));
    board->apply_move(Move("g6f7"));
    board->apply_move(Move("a8b8"));

    board->apply_move(Move("f7e7")); // First seen

    board->apply_move(Move("b8a8"));
    board->apply_move(Move("e7d7"));
    board->apply_move(Move("a8b8"));
    board->apply_move(Move("d7e7")); // Second seen

    board->apply_move(Move("b8a8"));
    board->apply_move(Move("e7f7"));
    success_count += get_game_state_unittest(25, board, GAME_CONTINUE);

    return success_count;
}

#pragma endregion get_game_state

#pragma region is_check

int is_check_unittest(int testIndex, Board *board, bool requested_is_check)
{
    bool is_check = board->get_check_state();

    if (is_check != requested_is_check)
    {
        cerr << "\n---------- Board - is_check_unittest() - Test " << testIndex
             << " - !!! FAILURE !!! ----------" << endl;
        cerr << "- Board : " << endl;
        board->log();

        cerr << "\n- Actual is_check    : " << is_check << endl;
        cerr << "- Requested is_check : " << requested_is_check << endl;

        return 0;
    }

    return 1;
}

int is_check_testLauncher()
{
    int success_count = 0;

    // Check by rook
    success_count += is_check_unittest(1, new Board("4r3/8/8/8/8/8/8/4K3 w - - 0 1"), true);
    success_count += is_check_unittest(2, new Board("4k3/8/8/8/8/8/8/4R3 b - - 0 1"), true);

    // Check by bishop
    success_count += is_check_unittest(3, new Board("8/8/1b6/8/8/4K3/8/8 w - - 0 1"), true);
    success_count += is_check_unittest(4, new Board("8/8/1k6/8/8/4B3/8/8 b - - 0 1"), true);

    // Check by knight
    success_count += is_check_unittest(5, new Board("8/8/8/8/8/3K4/1n6/8 w - - 0 1"), true);
    success_count += is_check_unittest(6, new Board("8/8/8/8/2N5/8/1k6/8 b - - 0 1"), true);

    // Check by pawn
    success_count += is_check_unittest(7, new Board("8/8/1p6/2K5/8/8/8/8 w - - 0 1"), true);
    success_count += is_check_unittest(8, new Board("8/8/8/8/8/2k5/1P6/8 b - - 0 1"), true);

    // Check obstrued by piece - Rook
    success_count += is_check_unittest(9, new Board("4r3/8/8/4p3/8/8/8/4K3 w - - 0 1"), false);
    success_count += is_check_unittest(10, new Board("4k3/8/8/4P3/8/8/8/4R3 b - - 0 1"), false);

    // Check obstrued by piece - Bishop
    success_count += is_check_unittest(11, new Board("8/8/1b6/8/3P4/4K3/8/8 w - - 0 1"), false);
    success_count += is_check_unittest(12, new Board("8/8/1k6/8/3p4/4B3/8/8 b - - 0 1"), false);

    // Check by pawn 2
    success_count +=
        is_check_unittest(71, new Board("8/6B1/8/2Pp3P/4k3/3p2p1/2p5/2RK2R1 w - - 0 84"), true);

    return success_count;
}

#pragma endregion is_check

#pragma region clone

int clone_unittest(int testIndex, Board *board)
{
    Board clone = board->clone();

    if (!(*board == &clone))
    {
        cerr << "\n---------- Board - clone_unittest() - Test " << testIndex
             << " - !!! FAILURE !!! ----------" << endl;
        cerr << "- Board : " << endl;
        board->log();

        cerr << "\n- Actual clone : " << endl;
        clone.log();

        return 0;
    }

    return 1;
}

int clone_testLauncher()
{
    int success_count = 0;

    // Empty board - Chess960 rules
    success_count += clone_unittest(1, new Board("8/8/8/8/8/8/8/8 w - - 0 1", true));
    // Empty board - Standard rules
    success_count += clone_unittest(2, new Board("8/8/8/8/8/8/8/8 w - - 0 1", false));

    // With pieces - Chess960 rules
    success_count += clone_unittest(
        3, new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b AHah b2 10 100", true)
    );
    // With pieces - Standard rules
    success_count += clone_unittest(
        4, new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b AHah b2 10 100", false)
    );

    return success_count;
}

#pragma endregion clone

int mainTestBoard()
{
    int successCount = 0;

    // Use '#' to block moves
    // Use 't' and 'T' to simulate opponent pieces

    successCount += create_fen_testLauncher();
    successCount += apply_move_testLauncher();
    successCount += apply_move_testLauncher_uci();
    successCount += get_game_state_testLauncher();
    successCount += is_check_testLauncher();

    successCount += find_pawn_moves_testLauncher();
    successCount += find_knight_moves_testLauncher();
    successCount += find_bishop_moves_testLauncher();
    successCount += find_rook_moves_testLauncher();
    successCount += find_queen_moves_testLauncher();
    successCount += find_king_moves_testLauncher();
    successCount += find_moves_ckecks_testLauncher();
    successCount += find_moves_not_unpinning_testLauncher();
    successCount += find_moves_castles_testLauncher();

    successCount += clone_testLauncher();

    return successCount;
}
