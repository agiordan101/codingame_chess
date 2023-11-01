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

int find_moves_RegularCases_FindAllMoves(int testIndex, Board *board, string initial_fen, Move *requested_moves, int move_count)
{
    // Arrange
    board->reset_board(initial_fen);

    // Act
    vector<Move> moves_found = board->find_moves();

    // Assert
    bool success = true;
    for (int i = 0; i < move_count; i++)
    {
        // Assert the requested move was found by the engine
        bool isfound = false;
        for (Move move_f : moves_found)
        {
            if (requested_moves[i] == move_f)
                isfound = true;
        }

        if (!isfound)
        {
            if (success)
                cerr << "\n---------- Board - find_moves_RegularCases_FindAllMoves() - Test " << testIndex << " - !!! FAILURE !!! ----------" << endl;

            cerr << "- This requested move wasn't found by the engine : " << endl;
            requested_moves[i].log();
            success = false;
        }
    }

    return success ? 1 : 0;
}

int find_moves_testLauncher(Board *board)
{
    int success_count = 0;
    Move requested_moves[2] = {};

    // No piece -> No moves
    success_count += find_moves_RegularCases_FindAllMoves(
        0,
        board,
        "8/8/8/8/8/8/8/8",
        requested_moves,
        0
    );

    // Pawn tests - White move - Regular
    // requested_moves[0] = Move(0, 0, 0, 0, false, false, false);
    // success_count += find_moves_RegularCases_FindAllMoves(
    //     0,
    //     board,
    //     "8/p7/8/8/8/8/8/8",
    //     requested_moves,
    //     1
    // );

    // Pawn tests - Black move - Regular
    // Pawn tests - White move - 2 cells advance
    // Pawn tests - Black move - 2 cells advance

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
    string en_passant = "e2e4";
    int half_move_clock = 0;
    int full_move = 1;
    
    Board *board = new Board(fen_board, color, castling, en_passant, half_move_clock, full_move);

    successCount += apply_move_testLauncher(board);
    successCount += find_moves_testLauncher(board);

    return successCount;
}
