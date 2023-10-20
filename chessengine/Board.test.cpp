#include "Board.hpp"

// Board   *constructor_unittest()
// {
//     string fen_board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
//     string color = "w";
//     string castling = "ahAH";
//     string en_passant = "e2e4";
//     int half_move_clock = 0;
//     int full_move = 1;

//     cerr << "\n[UNITTEST] Board - constructor_unittest() is starting " << endl;
//     cerr << "\n[UNITTEST] Board - FEN: " << fen_board << endl;
//     Board *board = new Board(fen_board, color, castling, en_passant, half_move_clock, full_move);
//     board->log();
//     board->show_board();

//     return board;
// }

int apply_move_validMove_ApplyIt(int testIndex, Board *board, string initial_board, string requested_board, int param_src_x, int param_src_y, int param_dst_x, int param_dst_y, bool param_castle, int param_promotion, bool param_en_passant)
{
    board->reset_board(initial_board);
    // board->show_board();

    board->apply_move(param_src_x, param_src_y, param_dst_x, param_dst_y, param_castle, param_promotion, param_en_passant);
    // board->show_board();

    if (!(*board == requested_board))
    {
        cerr << "[UNITTEST] Board - apply_move_validMove_ApplyIt() - Test " << testIndex << " - !!! FAILURE !!!" << endl;
        cerr << "[UNITTEST] Board - apply_move_validMove_ApplyIt() - Test " << testIndex << " - FEN board requested: " << requested_board << endl;
        board->show_board();
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
        0, 1, 0, 2, false, 0, false
    );
    successCount += apply_move_validMove_ApplyIt(
        2,
        board,
        "8/1p6/8/8/8/8/8/8",
        "8/8/8/1p6/8/8/8/8",
        1, 1, 1, 3, false, 0, false
    );
    // Regular White moves
    successCount += apply_move_validMove_ApplyIt(
        3,
        board,
        "8/8/8/8/8/8/4P3/8",
        "8/8/8/8/8/4P3/8/8",
        4, 6, 4, 5, false, 0, false
    );
    successCount += apply_move_validMove_ApplyIt(
        4,
        board,
        "8/8/8/8/8/8/8/3Q4",
        "8/8/8/8/6Q1/8/8/8",
        3, 7, 6, 4, false, 0, false
    );

    // Black Castle
    //cerr << "\n[UNITTEST] Board - apply_move_testLauncher() - Castles Black side " << endl;
    successCount += apply_move_validMove_ApplyIt(
        5,
        board,
        "r3k3/8/8/8/8/8/8/8",
        "2kr4/8/8/8/8/8/8/8",
        4, 0, 2, 0, true, 0, false
    );
    successCount += apply_move_validMove_ApplyIt(
        6,
        board,
        "4k2r/8/8/8/8/8/8/8",
        "5rk1/8/8/8/8/8/8/8",
        4, 0, 6, 0, true, 0, false
    );

    // White Castle
    //cerr << "\n[UNITTEST] Board - apply_move_testLauncher() - Castles White side " << endl;
    successCount += apply_move_validMove_ApplyIt(
        7,
        board,
        "8/8/8/8/8/8/8/R3K3",
        "8/8/8/8/8/8/8/2KR4",
        4, 7, 2, 7, true, 0, false
    );
    successCount += apply_move_validMove_ApplyIt(
        8,
        board,
        "8/8/8/8/8/8/8/4K2R",
        "8/8/8/8/8/8/8/5RK1",
        4, 7, 6, 7, true, 0, false
    );

    // Queen promotion White
    //cerr << "\n[UNITTEST] Board - apply_move_testLauncher() - White promotion" << endl;
    successCount += apply_move_validMove_ApplyIt(
        9,
        board,
        "8/5P2/8/8/8/8/8/8",
        "5Q2/8/8/8/8/8/8/8",
        5, 1, 5, 0, false, 'q', false
    );
    successCount += apply_move_validMove_ApplyIt(
        10,
        board,
        "8/3P4/8/8/8/8/8/8",
        "3N4/8/8/8/8/8/8/8",
        3, 1, 3, 0, false, 'N', false
    );

    // Queen promotion Black
    //cerr << "\n[UNITTEST] Board - apply_move_testLauncher() - Black promotion" << endl;
    successCount += apply_move_validMove_ApplyIt(
        11,
        board,
        "8/8/8/8/8/8/5p2/8",
        "8/8/8/8/8/8/8/5r2",
        5, 6, 5, 7, false, 'r', false
    );
    successCount += apply_move_validMove_ApplyIt(
        12,
        board,
        "8/8/8/8/8/8/3p4/8",
        "8/8/8/8/8/8/8/3b4",
        3, 6, 3, 7, false, 'B', false
    );

    // En passant White
    //cerr << "\n[UNITTEST] Board - apply_move_testLauncher() - White en passant" << endl;
    successCount += apply_move_validMove_ApplyIt(
        13,
        board,
        "8/8/8/4pP2/8/8/8/8",
        "8/8/4P3/8/8/8/8/8",
        5, 3, 4, 2, false, 0, true
    );
    successCount += apply_move_validMove_ApplyIt(
        14,
        board,
        "8/8/8/8/2pP4/8/8/8",
        "8/8/8/8/8/3p4/8/8",
        2, 4, 3, 5, false, 0, true
    );

    return successCount;
}

int mainTestBoard()
{
    int successCount = 0;
    string fen_board = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    string color = "w";
    string castling = "ahAH";
    string en_passant = "e2e4";
    int half_move_clock = 0;
    int full_move = 1;
    
    Board *board = new Board(fen_board, color, castling, en_passant, half_move_clock, full_move);

    successCount += apply_move_testLauncher(board);

    return successCount;
}
