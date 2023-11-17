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

    // Regular Black moves
    successCount += apply_move_validMove_ApplyIt(
        1,
        new Board("8/p7/8/8/8/8/8/8 b - - 0 1"),
        new Board("8/8/p7/8/8/8/8/8 w - - 1 2"),
        new Move(0, 1, 0, 2, false, 0, false)
    );
    // 2 advance black pawn that create a "en passant"
    successCount += apply_move_validMove_ApplyIt(
        2,
        new Board("8/1p6/8/2P5/8/8/8/8 b - - 0 1"),
        new Board("8/8/8/1pP5/8/8/8/8 w - b6 1 2"),
        new Move(1, 1, 1, 3, false, 0, false)
    );

    // Regular White moves
    // 2 advance white pawn that create a "en passant"
    successCount += apply_move_validMove_ApplyIt(
        3,
        new Board("8/8/8/8/3p4/8/4P3/8 w - - 0 1"),
        new Board("8/8/8/8/3pP3/8/8/8 b - e3 1 1"),
        new Move(4, 6, 4, 4, false, 0, false)
    );
    successCount += apply_move_validMove_ApplyIt(
        4,
        new Board("8/8/8/8/8/8/8/3Q4 w - - 0 1"),
        new Board("8/8/8/8/6Q1/8/8/8 b - - 1 1"),
        new Move(3, 7, 6, 4, false, 0, false)
    );

    // Black Castle
    successCount += apply_move_validMove_ApplyIt(
        5,
        new Board("r3k3/8/8/8/8/8/8/8 b a - 0 1"),
        new Board("2kr4/8/8/8/8/8/8/8 w - - 1 2"),
        new Move(4, 0, 0, 0, true, 0, false)
    );
    successCount += apply_move_validMove_ApplyIt(
        6,
        new Board("4k2r/8/8/8/8/8/8/8 b h - 0 1"),
        new Board("5rk1/8/8/8/8/8/8/8 w - - 1 2"),
        new Move(4, 0, 7, 0, true, 0, false)
    );

    // White Castle
    successCount += apply_move_validMove_ApplyIt(
        7,
        new Board("8/8/8/8/8/8/8/R3K3 w A - 0 1"),
        new Board("8/8/8/8/8/8/8/2KR4 b - - 1 1"),
        new Move(4, 7, 0, 7, true, 0, false)
    );
    successCount += apply_move_validMove_ApplyIt(
        8,
        new Board("8/8/8/8/8/8/8/4K2R w H - 0 1"),
        new Board("8/8/8/8/8/8/8/5RK1 b - - 1 1"),
        new Move(4, 7, 7, 7, true, 0, false)
    );

    // Queen promotion White
    successCount += apply_move_validMove_ApplyIt(
        9,
        new Board("8/5P2/8/8/8/8/8/8 w - - 0 1"),
        new Board("5Q2/8/8/8/8/8/8/8 b - - 1 1"),
        new Move(5, 1, 5, 0, false, 'Q', false)
    );
    successCount += apply_move_validMove_ApplyIt(
        10,
        new Board("8/3P4/8/8/8/8/8/8 w - - 0 1"),
        new Board("3N4/8/8/8/8/8/8/8 b - - 1 1"),
        new Move(3, 1, 3, 0, false, 'N', false)
    );

    // Queen promotion Black
    //cerr << "\n[UNITTEST] Board - apply_move_testLauncher() - Black promotion" << endl;
    successCount += apply_move_validMove_ApplyIt(
        11,
        new Board("8/8/8/8/8/8/5p2/8 b - - 0 1"),
        new Board("8/8/8/8/8/8/8/5r2 w - - 1 2"),
        new Move(5, 6, 5, 7, false, 'r', false)
    );
    successCount += apply_move_validMove_ApplyIt(
        12,
        new Board("8/8/8/8/8/8/3p4/8 b - - 0 1"),
        new Board("8/8/8/8/8/8/8/3b4 w - - 1 2"),
        new Move(3, 6, 3, 7, false, 'b', false)
    );

    // En passant White
    //cerr << "\n[UNITTEST] Board - apply_move_testLauncher() - White en passant" << endl;
    successCount += apply_move_validMove_ApplyIt(
        13,
        new Board("8/8/8/4pP2/8/8/8/8 w - b3 0 1"),
        new Board("8/8/4P3/8/8/8/8/8 b - - 1 1"),
        new Move(5, 3, 4, 2, false, 0, true)
    );
    successCount += apply_move_validMove_ApplyIt(
        14,
        new Board("8/8/8/8/2pP4/8/8/8 w - d3 0 1"),
        new Board("8/8/8/8/8/3p4/8/8 b - - 1 1"),
        new Move(2, 4, 3, 5, false, 0, true)
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

bool is_move_in_movelst(Move move, Move *movelst, int move_count)
{
    for (int i = 0; i < move_count; i++)
    {
        if (move == &movelst[i])
            return true;
    }

    return false;
}

int find_moves_RegularCases_FindAllMoves(int testIndex, Board *board, Move *requested_moves, int requested_moves_count)
{
    // Act
    vector<Move> moves_found = board->find_moves();

    // Assert all requested moves were found by the engine
    bool success = true;
    for (int i = 0; i < requested_moves_count; i++)
    {
        if (!is_move_in_movelst(&(requested_moves[i]), moves_found))
        {
            if (success)
            {
                cerr << "\n---------- Board - find_moves_RegularCases_FindAllMoves() - Test " << testIndex << " - !!! FAILURE !!! ----------" << endl;
                board->log();
            }

            cerr << "- This requested move wasn't found by the engine : " << endl;
            requested_moves[i].log();
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

    return success ? 1 : 0;
}

int find_moves_testLauncher()
{
    int success_count = 0;
    Move requested_moves[10] = {};

    // 666 - No piece -> No moves
    success_count += find_moves_RegularCases_FindAllMoves(
        666,
        new Board("8/8/8/8/8/8/8/8 w - - 0 1"),
        requested_moves,
        0
    );

    // 0 - Pawn tests - 2 Regular moves + 2 double advances + 2 captures (6) (2 blocked due to 2 opponent pieces also blocked)
    requested_moves[0] = Move(0, 2, 1, 1, false, false, false); // White left pawn captures
    requested_moves[1] = Move(1, 3, 1, 2, false, false, false); // White left pawn advance 1
    requested_moves[2] = Move(6, 6, 6, 5, false, false, false); // White right pawn advance 1 but not 2
    requested_moves[3] = Move(6, 6, 5, 5, false, false, false); // White right pawn captures
    requested_moves[4] = Move(7, 6, 7, 5, false, false, false); // White right pawn advance 1
    requested_moves[5] = Move(7, 6, 7, 4, false, false, false); // White right pawn advance 2
    success_count += find_moves_RegularCases_FindAllMoves(
        0,
        new Board("8/ppp5/P7/1P6/6p1/5p2/5PPP/8 w - - 0 1"),
        requested_moves,
        6
    );

    // 1 - Pawn tests - 2 Regular moves + 2 double advances + 2 captures (6) (2 blocked due to 2 opponent pieces also blocked)
    requested_moves[0] = Move(1, 1, 1, 2, false, false, false); // Black left pawn advance 1 but not 2
    requested_moves[1] = Move(1, 1, 0, 2, false, false, false); // Black left pawn captures
    requested_moves[2] = Move(2, 1, 2, 2, false, false, false); // Black left pawn advance 1
    requested_moves[3] = Move(2, 1, 2, 3, false, false, false); // Black left pawn advance 2
    requested_moves[4] = Move(5, 5, 6, 6, false, false, false); // Black right pawn captures
    requested_moves[5] = Move(6, 4, 6, 5, false, false, false); // Black right pawn advance 1
    success_count += find_moves_RegularCases_FindAllMoves(
        1,
        new Board("8/ppp5/P7/1P6/6p1/5p2/5PPP/8 b - - 0 1"),
        requested_moves,
        6
    );

    // 2 - Pawn tests - Return all Captures left and right (And no capture in the wrong way)
    requested_moves[0] = Move(0, 2, 1, 1, false, false, false); // White right capture
    requested_moves[1] = Move(2, 2, 1, 1, false, false, false); // White left capture
    success_count += find_moves_RegularCases_FindAllMoves(
        2,
        new Board("8/1p6/P1P5/1p6/8/8/8/8 w - - 0 1"),
        requested_moves,
        2
    );

    // 3 - Pawn tests - Return all Captures left and right (And no capture in the wrong way)
    requested_moves[0] = Move(1, 1, 0, 2, false, false, false); // Black left capture
    requested_moves[1] = Move(1, 1, 2, 2, false, false, false); // Black right capture
    success_count += find_moves_RegularCases_FindAllMoves(
        2,
        new Board("8/1p6/P1P5/1p6/8/8/8/8 b - - 0 1"),
        requested_moves,
        2
    );

    // 4 - Pawn tests - Return "en passant" black capture left -> b6
    requested_moves[0] = Move(2, 3, 1, 2, false, false, false); // Black en passant
    success_count += find_moves_RegularCases_FindAllMoves(
        2,
        new Board("8/8/8/1pP5/8/8/8/8 b - b6 0 1"),
        requested_moves,
        1
    );

    // 4 - Pawn tests - Return "en passant" black capture right -> d6
    requested_moves[0] = Move(2, 3, 3, 2, false, false, false); // White en passant
    success_count += find_moves_RegularCases_FindAllMoves(
        2,
        new Board("8/8/8/2Pp4/8/8/8/8 w - d6 0 1"),
        requested_moves,
        1
    );

    // For both sides - Pawn tests - Return "en passant" white capture left
    // For both sides - Pawn tests - Return "en passant" white capture right
    // For both sides - Pawn tests - Return all possible  
    // For both sides - Pawn tests - Return all promotions (32)

    // For both sides - Knight tests - 1 in the center Return all Regular move (8) (With obstacles)
    // For both sides - Knight tests - 4 on edges      Return all Regular move (6/8) (2 blocked due to 2 opponent pieces)

    // For both sides - Bishop tests - Return all Regular moves. 1 line should eat an opponent piece. Some lines are obstrued
    // For both sides - Rook tests - Return all Regular moves. 1 line should eat an opponent piece. Some lines are obstrued
    // For both sides - Queen tests - Return all Regular moves. Some line should eat an opponent piece. Some lines are obstrued

    // For both sides - King tests - Return all Regular moves. Some line should eat an opponent piece. Some lines are obstrued

    // For both sides - Check tests - King cannot move on a threated cell (By all other pieces)
    // For both sides - Check tests - Pieces cannot moves if the king get checked
    // For both sides - Other tests - No move found (Stale mate)

    return success_count;
}

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

    return success_count;
}

#pragma endregion create_fen

int mainTestBoard()
{
    int successCount = 0;

    successCount += apply_move_testLauncher();
    // successCount += find_moves_testLauncher();
    successCount += create_fen_testLauncher();

    return successCount;
}
