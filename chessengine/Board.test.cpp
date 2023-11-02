#include "Board.hpp"

#pragma region apply_move

int apply_move_validMove_ApplyIt(int testIndex, Board *board, string initial_fen, string requested_fen, Move* move)
{
    board->reset_board(initial_fen);
    // board->show_board();

    board->apply_move(*move);
    // board->show_board();

    if (!(*board == requested_fen))
    {
        cerr << "\n---------- Board - apply_move_validMove_ApplyIt() - Test " << testIndex << " - !!! FAILURE !!! ----------" << endl;
        cerr << "- Move applied : " << endl;
        move->log();
        cerr << "\n- Final board : " << endl;
        board->log();
        cerr << "\n- Requested FEN : " << requested_fen << endl;
        return 0;
    }

    return 1;
}

int apply_move_testLauncher(Board *board)
{
    int successCount = 0;

    //cerr << "\n[UNITTEST] Board - apply_move_testLauncher() - Regular moves both sides " << endl;
    // Regular Black moves
    successCount += apply_move_validMove_ApplyIt(
        1,
        board,
        "8/p7/8/8/8/8/8/8",
        "8/8/p7/8/8/8/8/8",
        new Move(0, 1, 0, 2, false, 0, false)
    );
    successCount += apply_move_validMove_ApplyIt(
        2,
        board,
        "8/1p6/8/8/8/8/8/8",
        "8/8/8/1p6/8/8/8/8",
        new Move(1, 1, 1, 3, false, 0, false)
    );
    // Regular White moves
    successCount += apply_move_validMove_ApplyIt(
        3,
        board,
        "8/8/8/8/8/8/4P3/8",
        "8/8/8/8/8/4P3/8/8",
        new Move(4, 6, 4, 5, false, 0, false)
    );
    successCount += apply_move_validMove_ApplyIt(
        4,
        board,
        "8/8/8/8/8/8/8/3Q4",
        "8/8/8/8/6Q1/8/8/8",
        new Move(3, 7, 6, 4, false, 0, false)
    );

    // Black Castle
    //cerr << "\n[UNITTEST] Board - apply_move_testLauncher() - Castles Black side " << endl;
    successCount += apply_move_validMove_ApplyIt(
        5,
        board,
        "r3k3/8/8/8/8/8/8/8",
        "2kr4/8/8/8/8/8/8/8",
        new Move(4, 0, 2, 0, true, 0, false)
    );
    successCount += apply_move_validMove_ApplyIt(
        6,
        board,
        "4k2r/8/8/8/8/8/8/8",
        "5rk1/8/8/8/8/8/8/8",
        new Move(4, 0, 6, 0, true, 0, false)
    );

    // White Castle
    //cerr << "\n[UNITTEST] Board - apply_move_testLauncher() - Castles White side " << endl;
    successCount += apply_move_validMove_ApplyIt(
        7,
        board,
        "8/8/8/8/8/8/8/R3K3",
        "8/8/8/8/8/8/8/2KR4",
        new Move(4, 7, 2, 7, true, 0, false)
    );
    successCount += apply_move_validMove_ApplyIt(
        8,
        board,
        "8/8/8/8/8/8/8/4K2R",
        "8/8/8/8/8/8/8/5RK1",
        new Move(4, 7, 6, 7, true, 0, false)
    );

    // Queen promotion White
    //cerr << "\n[UNITTEST] Board - apply_move_testLauncher() - White promotion" << endl;
    successCount += apply_move_validMove_ApplyIt(
        9,
        board,
        "8/5P2/8/8/8/8/8/8",
        "5Q2/8/8/8/8/8/8/8",
        new Move(5, 1, 5, 0, false, 'q', false)
    );
    successCount += apply_move_validMove_ApplyIt(
        10,
        board,
        "8/3P4/8/8/8/8/8/8",
        "3N4/8/8/8/8/8/8/8",
        new Move(3, 1, 3, 0, false, 'N', false)
    );

    // Queen promotion Black
    //cerr << "\n[UNITTEST] Board - apply_move_testLauncher() - Black promotion" << endl;
    successCount += apply_move_validMove_ApplyIt(
        11,
        board,
        "8/8/8/8/8/8/5p2/8",
        "8/8/8/8/8/8/8/5r2",
        new Move(5, 6, 5, 7, false, 'r', false)
    );
    successCount += apply_move_validMove_ApplyIt(
        12,
        board,
        "8/8/8/8/8/8/3p4/8",
        "8/8/8/8/8/8/8/3b4",
        new Move(3, 6, 3, 7, false, 'B', false)
    );

    // En passant White
    //cerr << "\n[UNITTEST] Board - apply_move_testLauncher() - White en passant" << endl;
    successCount += apply_move_validMove_ApplyIt(
        13,
        board,
        "8/8/8/4pP2/8/8/8/8",
        "8/8/4P3/8/8/8/8/8",
        new Move(5, 3, 4, 2, false, 0, true)
    );
    successCount += apply_move_validMove_ApplyIt(
        14,
        board,
        "8/8/8/8/2pP4/8/8/8",
        "8/8/8/8/8/3p4/8/8",
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
        if (*move == move_f)
            return true;
    }

    return false;
}

bool is_move_in_movelst(Move move, Move *movelst, int move_count)
{
    for (int i = 0; i < move_count; i++)
    {
        if (move == movelst[i])
            return true;
    }

    return false;
}

int find_moves_RegularCases_FindAllMoves(int testIndex, Board *board, string initial_fen, Move *requested_moves, int requested_moves_count)
{
    // Arrange
    board->reset_board(initial_fen);

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

int find_moves_testLauncher(Board *board)
{
    int success_count = 0;
    Move requested_moves[10] = {};

    // 0 - Pawn tests - 2 Regular moves + 2 double advances + 2 captures (6) (2 blocked due to 2 opponent pieces also blocked)
    // Set turn to white
    requested_moves[0] = Move(0, 2, 1, 1, false, false, false); // White left pawn captures
    requested_moves[1] = Move(1, 3, 1, 2, false, false, false); // White left pawn advance 1
    requested_moves[2] = Move(6, 6, 6, 5, false, false, false); // White right pawn advance 1 but not 2
    requested_moves[3] = Move(6, 6, 5, 5, false, false, false); // White right pawn captures
    requested_moves[4] = Move(7, 6, 7, 5, false, false, false); // White right pawn advance 1
    requested_moves[5] = Move(7, 6, 7, 4, false, false, false); // White right pawn advance 2
    success_count += find_moves_RegularCases_FindAllMoves(
        0,
        board,
        "8/ppp5/P7/1P6/6p1/5p2/5PPP/8",
        requested_moves,
        6
    );

    // 1 - Pawn tests - 2 Regular moves + 2 double advances + 2 captures (6) (2 blocked due to 2 opponent pieces also blocked)
    // Set turn to black
    requested_moves[0] = Move(1, 1, 1, 2, false, false, false); // Black left pawn advance 1 but not 2
    requested_moves[1] = Move(1, 1, 0, 2, false, false, false); // Black left pawn captures
    requested_moves[2] = Move(2, 1, 2, 2, false, false, false); // Black left pawn advance 1
    requested_moves[3] = Move(2, 1, 2, 3, false, false, false); // Black left pawn advance 2
    requested_moves[4] = Move(5, 5, 6, 6, false, false, false); // Black right pawn captures
    requested_moves[5] = Move(6, 4, 6, 5, false, false, false); // Black right pawn advance 1
    success_count += find_moves_RegularCases_FindAllMoves(
        1,
        board,
        "8/ppp5/P7/1P6/6p1/5p2/5PPP/8",
        requested_moves,
        6
    );

    // 2 - Pawn tests - Return all Captures left and right (And no capture in the wrong way)
    // Set turn to white
    requested_moves[0] = Move(0, 2, 1, 1, false, false, false); // White right capture
    requested_moves[1] = Move(2, 2, 1, 1, false, false, false); // White left capture
    success_count += find_moves_RegularCases_FindAllMoves(
        2,
        board,
        "8/1p6/P1P5/1p6/8/8/8/8",
        requested_moves,
        2
    );


    // 3 - Pawn tests - Return all Captures left and right (And no capture in the wrong way)
    // Set turn to black
    requested_moves[0] = Move(1, 1, 0, 2, false, false, false); // Black left capture
    requested_moves[1] = Move(1, 1, 2, 2, false, false, false); // Black right capture
    success_count += find_moves_RegularCases_FindAllMoves(
        2,
        board,
        "8/1p6/P1P5/1p6/8/8/8/8",
        requested_moves,
        2
    );

    // 4 - Pawn tests - Return "en passant" black capture left
    // Set turn to black
    board->en_passant_x = 1;
    board->en_passant_y = 2;
    requested_moves[0] = Move(2, 3, 1, 2, false, false, false); // Black en passant
    success_count += find_moves_RegularCases_FindAllMoves(
        2,
        board,
        "8/8/8/1pP5/8/8/8/8",
        requested_moves,
        1
    );

    // 4 - Pawn tests - Return "en passant" black capture right
    // Set turn to white
    board->en_passant_x = 3;
    board->en_passant_y = 2;
    requested_moves[0] = Move(2, 3, 3, 2, false, false, false); // White en passant
    success_count += find_moves_RegularCases_FindAllMoves(
        2,
        board,
        "8/8/8/2Pp4/8/8/8/8",
        requested_moves,
        1
    );

    // Reset en_passant
    board->en_passant_x = -1;
    board->en_passant_y = -1;

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

    // No piece -> No moves
    success_count += find_moves_RegularCases_FindAllMoves(
        0,
        board,
        "8/8/8/8/8/8/8/8",
        requested_moves,
        0
    );

    return success_count;
}

#pragma endregion find_moves

int mainTestBoard()
{
    // Default random values
    int successCount = 0;
    string fen_board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    string color = "w";
    string castling = "ahAH";
    string en_passant = "-";
    int half_move_clock = 0;
    int full_move = 1;
    
    Board *board = new Board(fen_board, color, castling, en_passant, half_move_clock, full_move);

    successCount += apply_move_testLauncher(board);
    successCount += find_moves_testLauncher(board);

    return successCount;
}
