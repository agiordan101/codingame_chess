#include "Board.hpp"

#pragma region apply_move

int apply_move_validMove_ApplyIt(int testIndex, Board *initial_board, Board *requested_board, Move* move)
{
    Board *test_board = initial_board->clone();
    
    test_board->apply_move(*move);

    if (!(*test_board == requested_board))
    {
        cerr << "\n---------- Board - apply_move_validMove_ApplyIt() - Test " << testIndex << " - !!! FAILURE !!! ----------" << endl;
        cerr << "\n- Initial board : " << endl;
        initial_board->log();

        cerr << "- Move applied : " << endl;
        move->log();

        cerr << "\n- Final board : " << endl;
        test_board->log();
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
        101,
        new Board("8/p7/8/8/8/8/8/8 b - - 5 1"),
        new Board("8/8/p7/8/8/8/8/8 w - - 0 2"),
        new Move("a7a6")

    );
    // 2 advance black pawn that create a "en passant"
    successCount += apply_move_validMove_ApplyIt(
        102,
        new Board("8/1p6/8/2P5/8/8/8/8 b - - 0 1"),
        new Board("8/8/8/1pP5/8/8/8/8 w - b6 0 2"),
        new Move("b7b5")
        
    );
    // 2 "en passant" in a row
    successCount += apply_move_validMove_ApplyIt(
        103,
        new Board("8/1p6/8/2P5/4P3/8/8/8 b - e4 0 1"),
        new Board("8/8/8/1pP5/4P3/8/8/8 w - b6 0 2"),
        new Move("b7b5")
    );

    // Regular White moves - (Don't reset the half turn rule)
    successCount += apply_move_validMove_ApplyIt(
        104,
        new Board("8/8/8/8/8/8/8/3Q4 w - - 5 1"),
        new Board("8/8/8/8/6Q1/8/8/8 b - - 6 1"),
        new Move("d1g4")
    );
    // 2 advance white pawn that create a "en passant"
    successCount += apply_move_validMove_ApplyIt(
        105,
        new Board("8/8/8/8/3p4/8/4P3/8 w - - 0 1"),
        new Board("8/8/8/8/3pP3/8/8/8 b - e3 0 1"),
        new Move("e2e4")
    );

    // Black Chess960 Castle left
    successCount += apply_move_validMove_ApplyIt(
        106,
        new Board("r3k3/8/8/8/8/8/8/8 b a - 0 1"),
        new Board("2kr4/8/8/8/8/8/8/8 w - - 1 2"),
        new Move("e8a8")
    );
    // Black Chess960 Castle right
    successCount += apply_move_validMove_ApplyIt(
        107,
        new Board("1k5r/8/8/8/8/8/8/8 b h - 0 1"),
        new Board("5rk1/8/8/8/8/8/8/8 w - - 1 2"),
        new Move("b8h8")
    );
    // White Chess960 Castle left
    successCount += apply_move_validMove_ApplyIt(
        108,
        new Board("8/8/8/8/8/8/8/R5K1 w A - 0 1"),
        new Board("8/8/8/8/8/8/8/2KR4 b - - 1 1"),
        new Move("g1a1")
    );
    // White Chess960 Castle right
    successCount += apply_move_validMove_ApplyIt(
        109,
        new Board("8/8/8/8/8/8/8/4K2R w H - 0 1"),
        new Board("8/8/8/8/8/8/8/5RK1 b - - 1 1"),
        new Move("e1h1")
    );
    // White king takes black rook, not castling
    successCount += apply_move_validMove_ApplyIt(
        110,
        new Board("8/8/8/8/8/8/8/1Kr5 w - - 2 78"),
        new Board("8/8/8/8/8/8/8/2K5 b - - 0 78"),
        new Move("b1c1")
    );

    // Black Standard Castle left
    successCount += apply_move_validMove_ApplyIt(
        111,
        new Board("r3k3/8/8/8/8/8/8/8 b a - 0 1", false),
        new Board("2kr4/8/8/8/8/8/8/8 w - - 1 2", false),
        new Move("e8a8")
    );
    // Black Standard Castle right
    successCount += apply_move_validMove_ApplyIt(
        112,
        new Board("1k5r/8/8/8/8/8/8/8 b h - 0 1", false),
        new Board("5rk1/8/8/8/8/8/8/8 w - - 1 2", false),
        new Move("b8h8")
    );
    // White Standard Castle left
    successCount += apply_move_validMove_ApplyIt(
        113,
        new Board("8/8/8/8/8/8/8/R5K1 w A - 0 1", false),
        new Board("8/8/8/8/8/8/8/2KR4 b - - 1 1", false),
        new Move("g1a1")
    );
    // White Standard Castle right
    successCount += apply_move_validMove_ApplyIt(
        114,
        new Board("8/8/8/8/8/8/8/4K2R w H - 0 1", false),
        new Board("8/8/8/8/8/8/8/5RK1 b - - 1 1", false),
        new Move("e1h1")
    );

    // Castles rights test - King moves
    successCount += apply_move_validMove_ApplyIt(
        115,
        new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"),
        new Board("8/8/8/8/8/8/4K3/R6R b - - 1 1"),
        new Move("e1e2")
    );
    // Castles rights test - Rook moves
    successCount += apply_move_validMove_ApplyIt(
        116,
        new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"),
        new Board("8/8/8/8/8/8/R7/4K2R b H - 1 1"),
        new Move("a1a2")
    );
    // Castles rights test - Rook moves
    successCount += apply_move_validMove_ApplyIt(
        117,
        new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"),
        new Board("8/8/8/8/8/8/7R/R3K21 b A - 1 1"),
        new Move("h1h2")
    );

    // Promotion White
    successCount += apply_move_validMove_ApplyIt(
        119,
        new Board("8/3P4/8/8/8/8/8/8 w - - 0 1"),
        new Board("3N4/8/8/8/8/8/8/8 b - - 0 1"),
        new Move("d7d8n")
    );

    // Promotion Black
    successCount += apply_move_validMove_ApplyIt(
        121,
        new Board("8/8/8/8/8/8/3p4/8 b - - 0 1"),
        new Board("8/8/8/8/8/8/8/3b4 w - - 0 2"),
        new Move("d2d1b")
    );

    // En passant White
    successCount += apply_move_validMove_ApplyIt(
        123,
        new Board("8/8/8/4pP2/8/8/8/8 w - e6 0 1"),
        new Board("8/8/4P3/8/8/8/8/8 b - - 0 1"),
        new Move("f5e6")
    );
    // En passant Black
    successCount += apply_move_validMove_ApplyIt(
        125,
        new Board("8/8/8/8/2pP4/8/8/8 b - d3 0 1"),
        new Board("8/8/8/8/8/3p4/8/8 w - - 0 2"),
        new Move("c4d3")
    );

    // Reset the half turn rule when a piece is captured
    successCount += apply_move_validMove_ApplyIt(
        126,
        new Board("8/8/4r3/8/8/8/4B3/8 b - - 5 1"),
        new Board("8/8/8/8/8/8/4r3/8 w - - 0 2"),
        new Move("e6e2")
    );

    // White captures a piece
    successCount += apply_move_validMove_ApplyIt(
        127,
        new Board("8/8/8/6r1/8/4B3/8/8 w - - 5 1"),
        new Board("8/8/8/6B1/8/8/8/8 b - - 0 1"),
        new Move("e3g5")
    );
    // Black captures a piece
    successCount += apply_move_validMove_ApplyIt(
        128,
        new Board("8/8/8/6b1/8/4R3/8/8 b - - 5 1"),
        new Board("8/8/8/8/8/4b3/8/8 w - - 0 2"),
        new Move("g5e3")
    );

    // White promotes to Queen while capturing a piece
    successCount += apply_move_validMove_ApplyIt(
        129,
        new Board("4q3/3P4/8/8/8/8/8/8 w - - 5 1"),
        new Board("4Q3/8/8/8/8/8/8/8 b - - 0 1"),
        new Move("d7e8q")
    );
    // Black promotes to Queen while capturing a piece
    successCount += apply_move_validMove_ApplyIt(
        130,
        new Board("8/8/8/8/8/8/3p4/4Q3 b - - 5 1"),
        new Board("8/8/8/8/8/8/8/4q3 w - - 0 2"),
        new Move("d2e1q")
    );

    return successCount;
}

# if BITBOARD_IMPLEMENTATION == 1

int apply_move_testLauncher()
{
    int successCount = 0;

    // Regular Black moves - (Reset the half turn rule due to a pawn advance)
    successCount += apply_move_validMove_ApplyIt(
        1,
        new Board("8/p7/8/8/8/8/8/8 b - - 5 1"),
        new Board("8/8/p7/8/8/8/8/8 w - - 0 2"),
        new Move('p', 1UL << 8, 1UL << 16)
    );
    // 2 advance black pawn that create a "en passant"
    successCount += apply_move_validMove_ApplyIt(
        2,
        new Board("8/1p6/8/2P5/8/8/8/8 b - - 0 1"),
        new Board("8/8/8/1pP5/8/8/8/8 w - b6 0 2"),
        new Move('p', 1UL << 9, 1UL << 25)
    );
    // 2 "en passant" in a row
    successCount += apply_move_validMove_ApplyIt(
        3,
        new Board("8/1p6/8/2P5/4P3/8/8/8 b - e4 0 1"),
        new Board("8/8/8/1pP5/4P3/8/8/8 w - b6 0 2"),
        new Move('p', 1UL << 9, 1UL << 25)
    );

    // Regular White moves - (Don't reset the half turn rule)
    successCount += apply_move_validMove_ApplyIt(
        4,
        new Board("8/8/8/8/8/8/8/3Q4 w - - 5 1"),
        new Board("8/8/8/8/6Q1/8/8/8 b - - 6 1"),
        new Move('Q', 1UL << 59, 1UL << 38)
    );
    // 2 advance white pawn that create a "en passant"
    successCount += apply_move_validMove_ApplyIt(
        5,
        new Board("8/8/8/8/3p4/8/4P3/8 w - - 0 1"),
        new Board("8/8/8/8/3pP3/8/8/8 b - e3 0 1"),
        new Move('P', 1UL << 52, 1UL << 36)
    );

    // Black Chess960 Castle left
    successCount += apply_move_validMove_ApplyIt(
        6,
        new Board("r3k3/8/8/8/8/8/8/8 b a - 0 1"),
        new Board("2kr4/8/8/8/8/8/8/8 w - - 1 2"),
        new Move('k', 1UL << 4, 1UL << 0, 0, BLACKLEFT)
    );
    // Black Chess960 Castle right
    successCount += apply_move_validMove_ApplyIt(
        7,
        new Board("1k5r/8/8/8/8/8/8/8 b h - 0 1"),
        new Board("5rk1/8/8/8/8/8/8/8 w - - 1 2"),
        new Move('k', 1UL << 1, 1UL << 7, 0, BLACKRIGHT)
    );
    // White Chess960 Castle left
    successCount += apply_move_validMove_ApplyIt(
        8,
        new Board("8/8/8/8/8/8/8/R5K1 w A - 0 1"),
        new Board("8/8/8/8/8/8/8/2KR4 b - - 1 1"),
        new Move('K', 1UL << 62, 1UL << 56, 0, WHITELEFT)
    );
    // White Chess960 Castle right
    successCount += apply_move_validMove_ApplyIt(
        9,
        new Board("8/8/8/8/8/8/8/4K2R w H - 0 1"),
        new Board("8/8/8/8/8/8/8/5RK1 b - - 1 1"),
        new Move('K', 1UL << 60, 1UL << 63, 0, WHITERIGHT)
    );
    // White king takes black rook, not castling
    successCount += apply_move_validMove_ApplyIt(
        10,
        new Board("8/8/8/8/8/8/8/1Kr5 w - - 2 78"),
        new Board("8/8/8/8/8/8/8/2K5 b - - 0 78"),
        new Move('K', 1UL << 57, 1UL << 58, 0, NOTCASTLE)
    );

    // Black Standard Castle left
    successCount += apply_move_validMove_ApplyIt(
        11,
        new Board("r3k3/8/8/8/8/8/8/8 b a - 0 1", false),
        new Board("2kr4/8/8/8/8/8/8/8 w - - 1 2", false),
        new Move('k', 1UL << 4, 1UL << 0, 0, BLACKLEFT)
    );
    // Black Standard Castle right
    successCount += apply_move_validMove_ApplyIt(
        12,
        new Board("1k5r/8/8/8/8/8/8/8 b h - 0 1", false),
        new Board("5rk1/8/8/8/8/8/8/8 w - - 1 2", false),
        new Move('k', 1UL << 1, 1UL << 7, 0, BLACKRIGHT)
    );
    // White Standard Castle left
    successCount += apply_move_validMove_ApplyIt(
        13,
        new Board("8/8/8/8/8/8/8/R5K1 w A - 0 1", false),
        new Board("8/8/8/8/8/8/8/2KR4 b - - 1 1", false),
        new Move('K', 1UL << 62, 1UL << 56, 0, WHITELEFT)
    );
    // White Standard Castle right
    successCount += apply_move_validMove_ApplyIt(
        14,
        new Board("8/8/8/8/8/8/8/4K2R w H - 0 1", false),
        new Board("8/8/8/8/8/8/8/5RK1 b - - 1 1", false),
        new Move('K', 1UL << 60, 1UL << 63, 0, WHITERIGHT)
    );

    // Castles rights test - King moves
    successCount += apply_move_validMove_ApplyIt(
        15,
        new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"),
        new Board("8/8/8/8/8/8/4K3/R6R b - - 1 1"),
        new Move('K', 1UL << 60, 1UL << 52, 0, NOTCASTLE)
    );
    // Castles rights test - Rook moves
    successCount += apply_move_validMove_ApplyIt(
        16,
        new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"),
        new Board("8/8/8/8/8/8/R7/4K2R b H - 1 1"),
        new Move('R', 1UL << 56, 1UL << 48)
    );
    // Castles rights test - Rook moves
    successCount += apply_move_validMove_ApplyIt(
        17,
        new Board("8/8/8/8/8/8/8/R3K2R w AH - 0 1"),
        new Board("8/8/8/8/8/8/7R/R3K21 b A - 1 1"),
        new Move('R', 1UL << 63, 1UL << 55)
    );

    // Promotion White
    successCount += apply_move_validMove_ApplyIt(
        18,
        new Board("8/5P2/8/8/8/8/8/8 w - - 0 1"),
        new Board("5Q2/8/8/8/8/8/8/8 b - - 0 1"),
        new Move('P', 1UL << 13, 1UL << 5, 'Q')
    );

    // Promotion Black
    successCount += apply_move_validMove_ApplyIt(
        20,
        new Board("8/8/8/8/8/8/5p2/8 b - - 0 1"),
        new Board("8/8/8/8/8/8/8/5r2 w - - 0 2"),
        new Move('p', 1UL << 53, 1UL << 61, 'r')
    );

    // En passant White
    successCount += apply_move_validMove_ApplyIt(
        22,
        new Board("8/8/8/4pP2/8/8/8/8 w - e6 0 1"),
        new Board("8/8/4P3/8/8/8/8/8 b - - 0 1"),
        new Move('P', 1UL << 29, 1UL << 20)
    );
    // En passant Black
    successCount += apply_move_validMove_ApplyIt(
        24,
        new Board("8/8/8/8/2pP4/8/8/8 b - d3 0 1"),
        new Board("8/8/8/8/8/3p4/8/8 w - - 0 2"),
        new Move('p', 1UL << 34, 1UL << 43)
    );

    // Reset the half turn rule when a piece is captured
    successCount += apply_move_validMove_ApplyIt(
        26,
        new Board("8/8/4r3/8/8/8/4B3/8 b - - 5 1"),
        new Board("8/8/8/8/8/8/4r3/8 w - - 0 2"),
        new Move('r', 1UL << 20, 1UL << 52)
    );

    // White captures a piece
    successCount += apply_move_validMove_ApplyIt(
        27,
        new Board("8/8/8/6r1/8/4B3/8/8 w - - 5 1"),
        new Board("8/8/8/6B1/8/8/8/8 b - - 0 1"),
        new Move('B', 1UL << 44, 1UL << 30)
    );
    // Black captures a piece
    successCount += apply_move_validMove_ApplyIt(
        28,
        new Board("8/8/8/6b1/8/4R3/8/8 b - - 5 1"),
        new Board("8/8/8/8/8/4b3/8/8 w - - 0 2"),
        new Move('b', 1UL << 30, 1UL << 44)
    );

    // White promotes to Queen while capturing a piece
    successCount += apply_move_validMove_ApplyIt(
        29,
        new Board("4q3/3P4/8/8/8/8/8/8 w - - 5 1"),
        new Board("4Q3/8/8/8/8/8/8/8 b - - 0 1"),
        new Move('P', 1UL << 11, 1UL << 4, 'Q')
    );
    // Black promotes to Queen while capturing a piece
    successCount += apply_move_validMove_ApplyIt(
        30,
        new Board("8/8/8/8/8/8/3p4/4Q3 b - - 5 1"),
        new Board("8/8/8/8/8/8/8/4q3 w - - 0 2"),
        new Move('p', 1UL << 51, 1UL << 60, 'q')
    );

    return successCount;
}

# else

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
    // 2 "en passant" in a row
    successCount += apply_move_validMove_ApplyIt(
        22,
        new Board("8/1p6/8/2P5/4P3/8/8/8 b - e4 0 1"),
        new Board("8/8/8/1pP5/4P3/8/8/8 w - b6 0 2"),
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
    // White king takes black rook, not castling
    successCount += apply_move_validMove_ApplyIt(
        88,
        new Board("8/8/1r6/K7/8/8/8/8 w - - 2 78"),
        new Board("8/8/1K6/8/8/8/8/8 b - - 0 78"),
        new Move(0, 3, 1, 2, 0)
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

    // Promotion White
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
        new Move("d7d8n")
    );

    // Promotion Black
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
        new Move("d2d1b")
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

# endif

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
    vector<Move> moves_found = board->get_available_moves();

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

# if BITBOARD_IMPLEMENTATION == 1

int find_pawn_moves_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[20];

    // All White possible moves
    requested_moves[0] = new Move('P', 1UL << 49, 1UL << 41); // Advance 1 (Against enemy)
    requested_moves[1] = new Move('P', 1UL << 49, 1UL << 33); // Advance 2 (Against enemy)
    requested_moves[2] = new Move('P', 1UL << 49, 1UL << 42); // Capture right
    requested_moves[3] = new Move('P', 1UL << 51, 1UL << 42); // Capture left
    requested_moves[4] = new Move('P', 1UL << 51, 1UL << 43); // Advance 1 (Against enemy)
    requested_moves[5] = new Move('P', 1UL << 53, 1UL << 45); // Advance 1 (Against ally)
    requested_moves[6] = new Move('P', 1UL << 53, 1UL << 37); // Advance 2 (Against ally)
    requested_moves[7] = new Move('P', 1UL << 46, 1UL << 38); // Advance 1 (By the 1st blocker)
    requested_moves[8] = new Move('P', 1UL << 55, 1UL << 47); // Advance 1 (Against ally)
    requested_moves[9] = new Move('P', 1UL << 39, 1UL << 31); // Advance 1 (By the 2nd blocker)
    requested_moves[10] = new Move('P', 1UL << 8, 1UL, 'N'); // Promotion
    requested_moves[11] = new Move('P', 1UL << 8, 1UL, 'B'); // Promotion
    requested_moves[12] = new Move('P', 1UL << 8, 1UL, 'R'); // Promotion
    requested_moves[13] = new Move('P', 1UL << 8, 1UL, 'Q'); // Promotion
    requested_moves[14] = new Move('P', 1UL << 20, 1UL << 13); // En passant right
    requested_moves[15] = new Move('P', 1UL << 22, 1UL << 13); // En passant left
    requested_moves[16] = new Move('P', 1UL << 20, 1UL << 12); // Advance 1
    requested_moves[17] = new Move('P', 1UL << 22, 1UL << 14); // Advance 1
    success_count += find_moves_RegularCases_FindAllMoves(
        1,
        new Board("8/P7/4PpP1/8/3q3P/2q3P1/1PPP1PPP/4q3 w - f7 0 1"),
        requested_moves,
        18
    );

    // All Black possible moves
    requested_moves[0] = new Move('p', 1UL << 9, 1UL << 17); // Advance 1 (Against enemy)
    requested_moves[1] = new Move('p', 1UL << 9, 1UL << 25); // Advance 2 (Against enemy)
    requested_moves[2] = new Move('p', 1UL << 9, 1UL << 18); // Capture right
    requested_moves[3] = new Move('p', 1UL << 11, 1UL << 18); // Capture left
    requested_moves[4] = new Move('p', 1UL << 11, 1UL << 19); // Advance 1 (Against enemy)
    requested_moves[5] = new Move('p', 1UL << 13, 1UL << 21); // Advance 1 (Against ally)
    requested_moves[6] = new Move('p', 1UL << 13, 1UL << 29); // Advance 2 (Against ally)
    requested_moves[7] = new Move('p', 1UL << 22, 1UL << 30); // Advance 1 (By the 1st blocker)
    requested_moves[8] = new Move('p', 1UL << 15, 1UL << 23); // Advance 1 (Against ally)
    requested_moves[9] = new Move('p', 1UL << 31, 1UL << 39); // Advance 1 (By the 2nd blocker)
    requested_moves[10] = new Move('p', 1UL << 48, 1UL << 56, 'N'); // Promotion
    requested_moves[11] = new Move('p', 1UL << 48, 1UL << 56, 'B'); // Promotion
    requested_moves[12] = new Move('p', 1UL << 48, 1UL << 56, 'R'); // Promotion
    requested_moves[13] = new Move('p', 1UL << 48, 1UL << 56, 'Q'); // Promotion
    requested_moves[14] = new Move('p', 1UL << 44, 1UL << 53); // En passant right
    requested_moves[15] = new Move('p', 1UL << 46, 1UL << 53); // En passant left
    requested_moves[16] = new Move('p', 1UL << 44, 1UL << 52); // Advance 1
    requested_moves[17] = new Move('p', 1UL << 46, 1UL << 54); // Advance 1
    success_count += find_moves_RegularCases_FindAllMoves(
        2,
        new Board("4Q3/1ppp1ppp/2Q3p1/3Q3p/8/4pPp1/p7/8 b - f2 0 1"),
        requested_moves,
        18
    );

    return success_count;
}
int find_knight_moves_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[20];

    // Left up corner
    requested_moves[0] = new Move('N', 1UL, 1UL << 10);
    requested_moves[1] = new Move('N', 1UL, 1UL << 17);
    success_count += find_moves_RegularCases_FindAllMoves(
        3,
        new Board("N7/8/8/8/8/8/8/8 w - - 0 1"),
        requested_moves,
        2
    );

    // Right up corner
    requested_moves[0] = new Move('N', 1UL << 7, 1UL << 13);
    requested_moves[1] = new Move('N', 1UL << 7, 1UL << 22);
    success_count += find_moves_RegularCases_FindAllMoves(
        4,
        new Board("7N/8/8/8/8/8/8/8 w - - 0 1"),
        requested_moves,
        2
    );

    // Left down corner
    requested_moves[0] = new Move('n', 1UL << 56, 1UL << 41);
    requested_moves[1] = new Move('n', 1UL << 56, 1UL << 50);
    success_count += find_moves_RegularCases_FindAllMoves(
        5,
        new Board("8/8/8/8/8/8/8/n7 b - - 0 1"),
        requested_moves,
        2
    );

    // Right down corner
    requested_moves[0] = new Move('n', 1UL << 63, 1UL << 46);
    requested_moves[1] = new Move('n', 1UL << 63, 1UL << 53);
    success_count += find_moves_RegularCases_FindAllMoves(
        6,
        new Board("8/8/8/8/8/8/8/7n b - - 0 1"),
        requested_moves,
        2
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
        6,
        new Board("8/8/8/3N4/8/2q1P3/8/8 w - - 0 1"),
        requested_moves,
        8
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
        7,
        new Board("8/8/8/3n4/8/2Q1p3/8/8 b - - 0 1"),
        requested_moves,
        8
    );

    return success_count;
}
int find_bishop_moves_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[20];

    // White - All move types
    requested_moves[0] = new Move('B', 1UL << 27, 1UL << 18);
    requested_moves[1] = new Move('B', 1UL << 27, 1UL << 9);
    requested_moves[2] = new Move('B', 1UL << 27, 1UL << 0);
    requested_moves[3] = new Move('B', 1UL << 27, 1UL << 20);
    requested_moves[4] = new Move('B', 1UL << 27, 1UL << 13); // Following cell is blocked by ally pawn
    requested_moves[5] = new Move('B', 1UL << 27, 1UL << 36);
    requested_moves[6] = new Move('B', 1UL << 27, 1UL << 45);
    requested_moves[7] = new Move('B', 1UL << 27, 1UL << 54);
    requested_moves[8] = new Move('B', 1UL << 27, 1UL << 63);
    requested_moves[9] = new Move('B', 1UL << 27, 1UL << 34);
    requested_moves[10] = new Move('B', 1UL << 27, 1UL << 41); // Capture enemy pawn
    success_count += find_moves_RegularCases_FindAllMoves(
        8,
        new Board("6P1/8/8/3B4/8/1p6/8/8 w - - 0 1"),
        requested_moves,
        11
    );

    // Black - All move types
    requested_moves[0] = new Move('b', 1UL << 27, 1UL << 18);
    requested_moves[1] = new Move('b', 1UL << 27, 1UL << 9);
    requested_moves[2] = new Move('b', 1UL << 27, 1UL << 0);
    requested_moves[3] = new Move('b', 1UL << 27, 1UL << 20);
    requested_moves[4] = new Move('b', 1UL << 27, 1UL << 13); // Following cell is blocked by ally pawn
    requested_moves[5] = new Move('b', 1UL << 27, 1UL << 36);
    requested_moves[6] = new Move('b', 1UL << 27, 1UL << 45);
    requested_moves[7] = new Move('b', 1UL << 27, 1UL << 54);
    requested_moves[8] = new Move('b', 1UL << 27, 1UL << 63);
    requested_moves[9] = new Move('b', 1UL << 27, 1UL << 34);
    requested_moves[10] = new Move('b', 1UL << 27, 1UL << 41); // Capture enemy pawn
    requested_moves[11] = new Move('p', 1UL << 6, 1UL << 14); // Blocker pawn move
    success_count += find_moves_RegularCases_FindAllMoves(
        9,
        new Board("6p1/8/8/3b4/8/1P6/8/8 b - - 0 1"),
        requested_moves,
        12
    );

    return success_count;
}
int find_rook_moves_testLauncher()
{
    int success_count = 0;
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
        10,
        new Board("3P4/8/8/3R2p1/8/8/8/8 w - - 0 1"),
        requested_moves,
        12
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
        10,
        new Board("3p4/8/8/3r2P1/8/8/8/8 b - - 0 1"),
        requested_moves,
        13
    );

    return success_count;
}
int find_queen_moves_testLauncher()
{
    int success_count = 0;
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
        10,
        new Board("1P6/8/8/1Q4p1/8/8/8/8 w - - 0 1"),
        requested_moves,
        21
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
        10,
        new Board("1p6/8/8/1q4P1/8/8/8/8 b - - 0 1"),
        requested_moves,
        22
    );

    return success_count;
}
int find_king_moves_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[10];

    // White - All move types (King cannot move on attacked squares)
    requested_moves[0] = new Move('K', 1UL << 51, 1UL << 43, 0, NOTCASTLE); // N
    requested_moves[1] = new Move('K', 1UL << 51, 1UL << 44, 0, NOTCASTLE); // NE
    requested_moves[2] = new Move('K', 1UL << 51, 1UL << 60, 0, NOTCASTLE); // SE
    requested_moves[3] = new Move('K', 1UL << 51, 1UL << 58, 0, NOTCASTLE); // SW
    requested_moves[4] = new Move('K', 1UL << 51, 1UL << 50, 0, NOTCASTLE); // W
    requested_moves[5] = new Move('K', 1UL << 51, 1UL << 42, 0, NOTCASTLE); // NW
    requested_moves[6] = new Move('P', 1UL << 52, 1UL << 44); // Pawn move
    requested_moves[7] = new Move('P', 1UL << 52, 1UL << 36); // Pawn move
    success_count += find_moves_RegularCases_FindAllMoves(
        11,
        new Board("8/8/8/8/8/8/2pKP3/8 w - - 0 1"),
        requested_moves,
        8
    );

    // Black - All move types (King cannot move on attacked squares)
    requested_moves[0] = new Move('k', 1UL << 9, 1UL << 2, 0, NOTCASTLE); // NE
    requested_moves[1] = new Move('k', 1UL << 9, 1UL << 10, 0, NOTCASTLE); // E
    requested_moves[2] = new Move('k', 1UL << 9, 1UL << 18, 0, NOTCASTLE); // SE
    requested_moves[3] = new Move('k', 1UL << 9, 1UL << 17, 0, NOTCASTLE); // S
    requested_moves[4] = new Move('k', 1UL << 9, 1UL << 16, 0, NOTCASTLE); // SW
    requested_moves[5] = new Move('k', 1UL << 9, 1UL << 0, 0, NOTCASTLE); // NW
    requested_moves[6] = new Move('p', 1UL << 8, 1UL << 16); // Pawn move
    requested_moves[7] = new Move('p', 1UL << 8, 1UL << 24); // Pawn move
    success_count += find_moves_RegularCases_FindAllMoves(
        12,
        new Board("8/pkP5/8/8/8/8/8/8 b - - 0 1"),
        requested_moves,
        8
    );

    return success_count;
}

int find_moves_ckecks_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[15];

    // Pieces cannot move if they aren't unchecking the king - White
    requested_moves[0] = new Move('K', 1UL << 17, 1UL << 8, 0, NOTCASTLE); // King NW
    requested_moves[1] = new Move('K', 1UL << 17, 1UL << 26, 0, NOTCASTLE); // King SE
    requested_moves[2] = new Move('K', 1UL << 17, 1UL << 25, 0, NOTCASTLE); // King S
    requested_moves[3] = new Move('K', 1UL << 17, 1UL << 24, 0, NOTCASTLE); // King SW
    requested_moves[4] = new Move('N', 1UL << 9, 1UL << 19); // Knight block
    requested_moves[5] = new Move('Q', 1UL << 5, 1UL << 19); // Queen block
    requested_moves[6] = new Move('Q', 1UL << 5, 1UL << 21); // Queen block 2
    requested_moves[7] = new Move('B', 1UL << 27, 1UL << 18); // Bishop block
    requested_moves[8] = new Move('B', 1UL << 27, 1UL << 20); // Bishop block
    requested_moves[9] = new Move('P', 1UL << 29, 1UL << 21); // Pawn block
    requested_moves[10] = new Move('P', 1UL << 29, 1UL << 22); // Pawn capture
    success_count += find_moves_RegularCases_FindAllMoves(
        13,
        new Board("5Q2/1N4r1/1K4r1/3B1Pr1/7R/8/8/8 w - - 0 1"),
        requested_moves,
        11
    );
    
    // Pieces cannot move if they aren't unchecking the king - Black
    requested_moves[0] = new Move('k', 1UL << 17, 1UL << 8, 0, NOTCASTLE); // King NW
    requested_moves[1] = new Move('k', 1UL << 17, 1UL << 26, 0, NOTCASTLE); // King SE
    requested_moves[2] = new Move('k', 1UL << 17, 1UL << 25, 0, NOTCASTLE); // King S
    requested_moves[3] = new Move('k', 1UL << 17, 1UL << 24, 0, NOTCASTLE); // King SW
    requested_moves[4] = new Move('n', 1UL << 9, 1UL << 19); // Knight block
    requested_moves[5] = new Move('q', 1UL << 5, 1UL << 19); // Queen block
    requested_moves[6] = new Move('q', 1UL << 5, 1UL << 21); // Queen block 2
    requested_moves[7] = new Move('b', 1UL << 27, 1UL << 18); // Bishop block
    requested_moves[8] = new Move('b', 1UL << 27, 1UL << 20); // Bishop block
    requested_moves[9] = new Move('p', 1UL << 15, 1UL << 22); // Pawn capture
    success_count += find_moves_RegularCases_FindAllMoves(
        14,
        new Board("5q2/1n4Rp/1k4R1/3b2R1/7r/8/8/8 b - - 0 1"),
        requested_moves,
        10
    );

    // Only the king can move if there are 2 checks - White
    requested_moves[0] = new Move('K', 1UL << 17, 1UL << 25, 0, NOTCASTLE); // King S
    requested_moves[1] = new Move('K', 1UL << 17, 1UL << 24, 0, NOTCASTLE); // King SW
    success_count += find_moves_RegularCases_FindAllMoves(
        15,
        new Board("5Q2/1N4r1/1K4r1/3B1Pr1/3b3R/8/8/8 w - - 0 1"),
        requested_moves,
        2
    );
    
    // Only the king can move if there are 2 checks - Black
    requested_moves[0] = new Move('k', 1UL << 17, 1UL << 25, 0, NOTCASTLE); // King S
    requested_moves[1] = new Move('k', 1UL << 17, 1UL << 24, 0, NOTCASTLE); // King SW
    success_count += find_moves_RegularCases_FindAllMoves(
        16,
        new Board("5q2/1n4Rp/1k4R1/3b2R1/3B3r/8/8/8 b - - 0 1"),
        requested_moves,
        2
    );

    return success_count;
}

int find_moves_not_unpinning_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[10];

    // Only the king can move if there are 2 checks - White
    requested_moves[0] = new Move('K', 1UL << 17, 1UL << 16, 0, NOTCASTLE); // King W
    requested_moves[1] = new Move('K', 1UL << 17, 1UL << 24, 0, NOTCASTLE); // King SW
    requested_moves[2] = new Move('B', 1UL << 10, 1UL << 3);  // Pinned bishop captures
    requested_moves[3] = new Move('Q', 1UL << 26, 1UL << 35); // Pinned queen captures
    requested_moves[4] = new Move('R', 1UL << 25, 1UL << 33); // Pinned rook captures
    success_count += find_moves_RegularCases_FindAllMoves(
        17,
        new Board("1q1b4/1NB5/1KPr4/1RQ5/1r1b4/8/8/8 w - - 0 1"),
        requested_moves,
        5
    );

    // Only the king can move if there are 2 checks - Black
    requested_moves[0] = new Move('k', 1UL << 17, 1UL << 16, 0, NOTCASTLE); // King W
    requested_moves[1] = new Move('k', 1UL << 17, 1UL << 24, 0, NOTCASTLE); // King SW
    requested_moves[2] = new Move('b', 1UL << 10, 1UL << 3);  // Pinned bishop captures
    requested_moves[3] = new Move('q', 1UL << 26, 1UL << 35); // Pinned queen captures
    requested_moves[4] = new Move('r', 1UL << 25, 1UL << 33); // Pinned rook captures
    success_count += find_moves_RegularCases_FindAllMoves(
        18,
        new Board("1Q1B4/1nb5/1kpR4/1rq5/1R1B4/8/8/8 b - - 0 1"),
        requested_moves,
        5
    );

    return success_count;
}

int find_moves_not_illegal_ones_testLauncher()
{
    return 0;
}

int find_moves_castles_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[20];

    // Black
    requested_moves[0] = new Move('k', 1UL << 4, 1UL << 5, 0, NOTCASTLE); // King E
    requested_moves[1] = new Move('k', 1UL << 4, 1UL << 13, 0, NOTCASTLE); // King SE
    requested_moves[2] = new Move('k', 1UL << 4, 1UL << 12, 0, NOTCASTLE); // King S
    requested_moves[3] = new Move('k', 1UL << 4, 1UL << 11, 0, NOTCASTLE); // King SW
    requested_moves[4] = new Move('k', 1UL << 4, 1UL << 3, 0, NOTCASTLE); // King W
    requested_moves[5] = new Move('k', 1UL << 4, 1UL << 0, 0, BLACKLEFT);
    requested_moves[6] = new Move('r', 1UL << 0, 1UL << 1); // Rook 1
    requested_moves[7] = new Move('r', 1UL << 0, 1UL << 2); // Rook 2
    requested_moves[8] = new Move('r', 1UL << 0, 1UL << 3); // Rook 3
    requested_moves[9] = new Move('r', 1UL << 0, 1UL << 8); // Rook 4
    requested_moves[10] = new Move('r', 1UL << 0, 1UL << 16); // Rook capture
    requested_moves[11] = new Move('k', 1UL << 4, 1UL << 7, 0, BLACKRIGHT);
    requested_moves[12] = new Move('r', 1UL << 7, 1UL << 6); // Rook 6
    requested_moves[13] = new Move('r', 1UL << 7, 1UL << 5); // Rook 5
    requested_moves[14] = new Move('r', 1UL << 7, 1UL << 15); // Rook 6
    requested_moves[15] = new Move('r', 1UL << 7, 1UL << 23); // Rook capture
    success_count += find_moves_RegularCases_FindAllMoves(
        19,
        new Board("r3k2r/8/P6P/8/8/8/8/8 b ah - 0 1", true),
        requested_moves,
        16
    );

    // White
    requested_moves[0] = new Move('K', 1UL << 60, 1UL << 61, 0, NOTCASTLE); // King E
    requested_moves[1] = new Move('K', 1UL << 60, 1UL << 59, 0, NOTCASTLE); // King W
    requested_moves[2] = new Move('K', 1UL << 60, 1UL << 51, 0, NOTCASTLE); // King NW
    requested_moves[3] = new Move('K', 1UL << 60, 1UL << 52, 0, NOTCASTLE); // King N
    requested_moves[4] = new Move('K', 1UL << 60, 1UL << 53, 0, NOTCASTLE); // King NE
    requested_moves[5] = new Move('K', 1UL << 60, 1UL << 56, 0, WHITELEFT);
    requested_moves[6] = new Move('R', 1UL << 56, 1UL << 57); // Rook 1
    requested_moves[7] = new Move('R', 1UL << 56, 1UL << 58); // Rook 2
    requested_moves[8] = new Move('R', 1UL << 56, 1UL << 59); // Rook 3
    requested_moves[9] = new Move('R', 1UL << 56, 1UL << 48); // Rook 4
    requested_moves[10] = new Move('R', 1UL << 56, 1UL << 40); // Rook capture
    requested_moves[11] = new Move('K', 1UL << 60, 1UL << 63, 0, WHITERIGHT);
    requested_moves[12] = new Move('R', 1UL << 63, 1UL << 62); // Rook 1
    requested_moves[13] = new Move('R', 1UL << 63, 1UL << 61); // Rook 2
    requested_moves[14] = new Move('R', 1UL << 63, 1UL << 55); // Rook 3
    requested_moves[15] = new Move('R', 1UL << 63, 1UL << 47); // Rook capture
    success_count += find_moves_RegularCases_FindAllMoves(
        20,
        new Board("8/8/8/8/8/p6p/8/R3K2R w AH - 0 1", true),
        requested_moves,
        16
    );

    return success_count;
}

# else

int find_pawn_moves_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[15];

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

    // Promotions - While taking a piece - White
    requested_moves[0] = new Move(1, 1, 0, 0, 'N');
    requested_moves[1] = new Move(1, 1, 0, 0, 'B');
    requested_moves[2] = new Move(1, 1, 0, 0, 'R');
    requested_moves[3] = new Move(1, 1, 0, 0, 'Q');
    requested_moves[4] = new Move(1, 1, 1, 0, 'N');
    requested_moves[5] = new Move(1, 1, 1, 0, 'B');
    requested_moves[6] = new Move(1, 1, 1, 0, 'R');
    requested_moves[7] = new Move(1, 1, 1, 0, 'Q');
    requested_moves[8] = new Move(1, 1, 2, 0, 'N');
    requested_moves[9] = new Move(1, 1, 2, 0, 'B');
    requested_moves[10] = new Move(1, 1, 2, 0, 'R');
    requested_moves[11] = new Move(1, 1, 2, 0, 'Q');
    success_count += find_moves_RegularCases_FindAllMoves(
        10,
        new Board("t1t5/1P6/8/8/8/8/8/8 w - - 0 1"),
        requested_moves,
        12
    );

    // Promotions - While taking a piece - Black
    requested_moves[0] = new Move(3, 6, 2, 7, 'n');
    requested_moves[1] = new Move(3, 6, 2, 7, 'b');
    requested_moves[2] = new Move(3, 6, 2, 7, 'r');
    requested_moves[3] = new Move(3, 6, 2, 7, 'q');
    requested_moves[4] = new Move(3, 6, 3, 7, 'n');
    requested_moves[5] = new Move(3, 6, 3, 7, 'b');
    requested_moves[6] = new Move(3, 6, 3, 7, 'r');
    requested_moves[7] = new Move(3, 6, 3, 7, 'q');
    requested_moves[8] = new Move(3, 6, 4, 7, 'n');
    requested_moves[9] = new Move(3, 6, 4, 7, 'b');
    requested_moves[10] = new Move(3, 6, 4, 7, 'r');
    requested_moves[11] = new Move(3, 6, 4, 7, 'q');
    success_count += find_moves_RegularCases_FindAllMoves(
        11,
        new Board("8/8/8/8/8/8/3p4/2T1T3 b - - 0 1"),
        requested_moves,
        12
    );

    // No piece -> No moves
    success_count += find_moves_RegularCases_FindAllMoves(
        12,
        new Board("8/8/8/8/8/8/8/8 w - - 0 1"),
        requested_moves,
        0
    );

    // Find "en_passant" move to remove check
    requested_moves[0] = new Move(2, 3, 3, 2, 0);
    success_count += find_moves_RegularCases_FindAllMoves(
        13,
        new Board("8/8/2#5/1#Pp4/2K5/1#6/3q4/8 w - d6 0 30"),
        requested_moves,
        1
    );

    return success_count;
}

int find_knight_moves_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[10];

    // Regular move - White
    requested_moves[0] = new Move(2, 2, 0, 1, 0); // 2 left 1 up
    requested_moves[1] = new Move(2, 2, 0, 3, 0); // 2 left 1 down
    requested_moves[2] = new Move(2, 2, 4, 1, 0); // 2 right 1 up
    requested_moves[3] = new Move(2, 2, 4, 3, 0); // 2 right 1 down
    requested_moves[4] = new Move(2, 2, 1, 0, 0); // 2 up 1 left
    requested_moves[5] = new Move(2, 2, 3, 0, 0); // 2 up 1 right
    requested_moves[6] = new Move(2, 2, 1, 4, 0); // 2 down 1 left
    requested_moves[7] = new Move(2, 2, 3, 4, 0); // 2 down 1 right
    success_count += find_moves_RegularCases_FindAllMoves(
        11,
        new Board("8/8/2N5/8/8/8/8/8 w - - 0 1"),
        requested_moves,
        8
    );

    // Regular move - Black
    requested_moves[0] = new Move(5, 5, 3, 4, 0); // 2 left 1 up
    requested_moves[1] = new Move(5, 5, 3, 6, 0); // 2 left 1 down
    requested_moves[2] = new Move(5, 5, 7, 4, 0); // 2 right 1 up
    requested_moves[3] = new Move(5, 5, 7, 6, 0); // 2 right 1 down
    requested_moves[4] = new Move(5, 5, 4, 3, 0); // 2 up 1 left
    requested_moves[5] = new Move(5, 5, 6, 3, 0); // 2 up 1 right
    requested_moves[6] = new Move(5, 5, 4, 7, 0); // 2 down 1 left
    requested_moves[7] = new Move(5, 5, 6, 7, 0); // 2 down 1 right
    success_count += find_moves_RegularCases_FindAllMoves(
        12,
        new Board("8/8/8/8/8/5n2/8/8 b - - 0 1"),
        requested_moves,
        8
    );

    // Captures - White
    requested_moves[0] = new Move(2, 2, 0, 1, 0); // 2 left 1 up
    requested_moves[1] = new Move(2, 2, 0, 3, 0); // 2 left 1 down
    requested_moves[2] = new Move(2, 2, 4, 1, 0); // 2 right 1 up
    requested_moves[3] = new Move(2, 2, 4, 3, 0); // 2 right 1 down
    requested_moves[4] = new Move(2, 2, 1, 0, 0); // 2 up 1 left
    requested_moves[5] = new Move(2, 2, 3, 0, 0); // 2 up 1 right
    requested_moves[6] = new Move(2, 2, 1, 4, 0); // 2 down 1 left
    requested_moves[7] = new Move(2, 2, 3, 4, 0); // 2 down 1 right
    success_count += find_moves_RegularCases_FindAllMoves(
        13,
        new Board("1p1p4/p3p3/2N5/p3p3/1p1p4/8/8/8 w - - 0 1"),
        requested_moves,
        8
    );

    // Captures - Black
    requested_moves[0] = new Move(5, 5, 3, 4, 0); // 2 left 1 up
    requested_moves[1] = new Move(5, 5, 3, 6, 0); // 2 left 1 down
    requested_moves[2] = new Move(5, 5, 7, 4, 0); // 2 right 1 up
    requested_moves[3] = new Move(5, 5, 7, 6, 0); // 2 right 1 down
    requested_moves[4] = new Move(5, 5, 4, 3, 0); // 2 up 1 left
    requested_moves[5] = new Move(5, 5, 6, 3, 0); // 2 up 1 right
    requested_moves[6] = new Move(5, 5, 4, 7, 0); // 2 down 1 left
    requested_moves[7] = new Move(5, 5, 6, 7, 0); // 2 down 1 right
    success_count += find_moves_RegularCases_FindAllMoves(
        14,
        new Board("8/8/8/1P1P4/P3P3/5n2/P3P3/1P1P4 b - - 0 1"),
        requested_moves,
        8
    );

    // No moves - Left up corner - White
    success_count += find_moves_RegularCases_FindAllMoves(
        15,
        new Board("N1#5/1#P5/1P6/8/8/8/8/8 w - - 0 1"),
        requested_moves,
        0
    );
    // No moves - Left up corner - Black
    success_count += find_moves_RegularCases_FindAllMoves(
        16,
        new Board("n7/2p5/1p#5/1#6/8/8/8/8 b - - 0 1"),
        requested_moves,
        0
    );

    // No moves - Right up corner - White
    success_count += find_moves_RegularCases_FindAllMoves(
        17,
        new Board("5#1N/5P#1/6P1/8/8/8/8/8 w - - 0 1"),
        requested_moves,
        0
    );
    // No moves - Right up corner - Black
    success_count += find_moves_RegularCases_FindAllMoves(
        18,
        new Board("7n/5p2/5#p1/6#1/8/8/8/8 b - - 0 1"),
        requested_moves,
        0
    );

    // No moves - Left down corner - White
    success_count += find_moves_RegularCases_FindAllMoves(
        19,
        new Board("8/8/8/8/1#6/1P#5/2P5/N7 w - - 0 1"),
        requested_moves,
        0
    );
    // No moves - Left down corner - Black
    success_count += find_moves_RegularCases_FindAllMoves(
        20,
        new Board("8/8/8/8/8/1p6/1#p5/n1#5 b - - 0 1"),
        requested_moves,
        0
    );

    // No moves - Right down corner - White
    success_count += find_moves_RegularCases_FindAllMoves(
        21,
        new Board("8/8/8/8/6#1/5#P1/5P2/7N w - - 0 1"),
        requested_moves,
        0
    );
    // No moves - Right down corner - Black
    success_count += find_moves_RegularCases_FindAllMoves(
        22,
        new Board("8/8/8/8/8/6p1/5p#1/5#1n b - - 0 1"),
        requested_moves,
        0
    );

    return success_count;
}

int find_bishop_moves_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[10];

    // Regular move & blocked - White
    requested_moves[0] = new Move(3, 3, 2, 2, 0); // Left up 1
    requested_moves[1] = new Move(3, 3, 1, 1, 0); // Left up 2
    requested_moves[2] = new Move(3, 3, 0, 0, 0); // Left up 3
    requested_moves[3] = new Move(3, 3, 4, 2, 0); // Right up 1
    requested_moves[4] = new Move(3, 3, 5, 1, 0); // Right up 2
    requested_moves[5] = new Move(3, 3, 4, 4, 0); // Right down 1
    success_count += find_moves_RegularCases_FindAllMoves(
        23,
        new Board("6T1/8/8/3B4/2T5/5T2/8/8 w - - 0 1"),
        requested_moves,
        6
    );

    // Regular move & blocked - Black
    requested_moves[0] = new Move(4, 3, 3, 2, 0); // Left up 1
    requested_moves[1] = new Move(4, 3, 2, 1, 0); // Left up 2
    requested_moves[2] = new Move(4, 3, 1, 0, 0); // Left up 3
    requested_moves[3] = new Move(4, 3, 5, 2, 0); // Right up 1
    requested_moves[4] = new Move(4, 3, 6, 1, 0); // Right up 2
    requested_moves[5] = new Move(4, 3, 5, 4, 0); // Right down 1
    success_count += find_moves_RegularCases_FindAllMoves(
        24,
        new Board("7t/8/8/4b3/3t4/6t1/8/8 b - - 0 1"),
        requested_moves,
        6
    );

    // Captures - White
    requested_moves[0] = new Move(0, 7, 1, 6, 0); // Right up
    success_count += find_moves_RegularCases_FindAllMoves(
        25,
        new Board("8/8/8/8/8/8/1t6/B7 w - - 0 1"),
        requested_moves,
        1
    );

    // Captures - Black
    requested_moves[0] = new Move(7, 0, 6, 1, 0); // Left down
    success_count += find_moves_RegularCases_FindAllMoves(
        26,
        new Board("7b/6T1/8/8/8/8/8/8 b - - 0 1"),
        requested_moves,
        1
    );

    return success_count;
}

int find_rook_moves_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[15];

    // Regular moves
    requested_moves[0] = new Move(3, 3, 3, 2, 0); // Up 1
    requested_moves[1] = new Move(3, 3, 3, 1, 0); // Up 2
    requested_moves[2] = new Move(3, 3, 3, 0, 0); // Up 3
    requested_moves[3] = new Move(3, 3, 2, 3, 0); // Left 1
    requested_moves[4] = new Move(3, 3, 1, 3, 0); // Left 2
    requested_moves[5] = new Move(3, 3, 0, 3, 0); // Left 3
    requested_moves[6] = new Move(3, 3, 4, 3, 0); // Right 1
    requested_moves[7] = new Move(3, 3, 5, 3, 0); // Right 2
    requested_moves[8] = new Move(3, 3, 6, 3, 0); // Right 3
    requested_moves[9] = new Move(3, 3, 7, 3, 0); // Right 4
    requested_moves[10] = new Move(3, 3, 3, 4, 0); // Down 1
    requested_moves[11] = new Move(3, 3, 3, 5, 0); // Down 2
    requested_moves[12] = new Move(3, 3, 3, 6, 0); // Down 3
    requested_moves[13] = new Move(3, 3, 3, 7, 0); // Down 4
    success_count += find_moves_RegularCases_FindAllMoves(
        27,
        new Board("8/8/8/3R4/8/8/8/8 w - - 0 1"),
        requested_moves,
        14
    );
    success_count += find_moves_RegularCases_FindAllMoves(
        28,
        new Board("8/8/8/3r4/8/8/8/8 b - - 0 1"),
        requested_moves,
        14
    );

    // Captures
    requested_moves[0] = new Move(3, 3, 3, 2, 0); // Up 1
    requested_moves[1] = new Move(3, 3, 2, 3, 0); // Left 1
    requested_moves[2] = new Move(3, 3, 4, 3, 0); // Right 1
    requested_moves[3] = new Move(3, 3, 3, 4, 0); // Down 1
    success_count += find_moves_RegularCases_FindAllMoves(
        29,
        new Board("8/8/3t4/2tRt3/3t4/8/8/8 w - - 0 1"),
        requested_moves,
        4
    );
    success_count += find_moves_RegularCases_FindAllMoves(
        30,
        new Board("8/8/3T4/2TrT3/3T4/8/8/8 b - - 0 1"),
        requested_moves,
        4
    );

    // Blocked
    success_count += find_moves_RegularCases_FindAllMoves(
        31,
        new Board("8/8/3T4/2TRT3/3T4/8/8/8 w - - 0 1"),
        requested_moves,
        0
    );
    success_count += find_moves_RegularCases_FindAllMoves(
        32,
        new Board("8/8/3t4/2trt3/3t4/8/8/8 b - - 0 1"),
        requested_moves,
        0
    );

    return success_count;    
}

int find_queen_moves_testLauncher()
{
    return 0;
}

int find_king_moves_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[10];

    // Regular moves
    requested_moves[0] = new Move(3, 3, 2, 2, 0); // Up left
    requested_moves[1] = new Move(3, 3, 3, 2, 0); // Up
    requested_moves[2] = new Move(3, 3, 4, 2, 0); // Up right
    requested_moves[3] = new Move(3, 3, 2, 3, 0); // Left
    requested_moves[4] = new Move(3, 3, 4, 3, 0); // Right
    requested_moves[5] = new Move(3, 3, 2, 4, 0); // Down left
    requested_moves[6] = new Move(3, 3, 3, 4, 0); // Down
    requested_moves[7] = new Move(3, 3, 4, 4, 0); // Down right
    success_count += find_moves_RegularCases_FindAllMoves(
        33,
        new Board("8/8/8/3K4/8/8/8/8 w - - 0 1"),
        requested_moves,
        8
    );
    success_count += find_moves_RegularCases_FindAllMoves(
        34,
        new Board("8/8/8/3k4/8/8/8/8 b - - 0 1"),
        requested_moves,
        8
    );

    // Castles - White
    requested_moves[0] = new Move(1, 7, 0, 7, 0); // Castle left
    requested_moves[1] = new Move(1, 7, 7, 7, 0); // Castle right
    requested_moves[2] = new Move(7, 7, 6, 7, 0); // Rook moves 1
    requested_moves[3] = new Move(7, 7, 5, 7, 0); // Rook moves 2
    requested_moves[4] = new Move(7, 7, 4, 7, 0); // Rook moves 3
    requested_moves[5] = new Move(7, 7, 3, 7, 0); // Rook moves 4
    requested_moves[6] = new Move(7, 7, 2, 7, 0); // Rook moves 5
    requested_moves[7] = new Move(1, 7, 2, 7, 0); // King moves
    success_count += find_moves_RegularCases_FindAllMoves(
        35,
        new Board("8/8/8/8/8/8/###4#/RK5R w AH - 0 1"),
        requested_moves,
        8
    );

    // Castles - Black
    requested_moves[0] = new Move(4, 0, 2, 0, 0); // Castle left
    requested_moves[1] = new Move(4, 0, 6, 0, 0); // Castle right
    requested_moves[2] = new Move(4, 0, 3, 0, 0); // King moves left
    requested_moves[3] = new Move(4, 0, 5, 0, 0); // King moves right
    requested_moves[4] = new Move(2, 0, 3, 0, 0); // Rook moves right
    requested_moves[5] = new Move(6, 0, 5, 0, 0); // Rook moves left
    success_count += find_moves_RegularCases_FindAllMoves(
        36,
        new Board("1#r1k1r#/########/8/8/8/8/8/8 b cg - 0 1"),
        requested_moves,
        6
    );

    // No castles - Blocked on the way 1
    requested_moves[0] = new Move(2, 7, 3, 7, 0); // King moves right
    requested_moves[1] = new Move(6, 7, 5, 7, 0); // Rook moves left 1
    requested_moves[2] = new Move(6, 7, 4, 7, 0); // Rook moves left 2
    requested_moves[3] = new Move(6, 7, 3, 7, 0); // Rook moves left 3
    success_count += find_moves_RegularCases_FindAllMoves(
        37,
        new Board("8/8/8/8/4r3/8/####1###/##K3R# w G - 0 1"),
        requested_moves,
        4
    );
    // No castles - Blocked on the way 2
    requested_moves[0] = new Move(2, 7, 3, 7, 0); // Rook moves left 1
    requested_moves[1] = new Move(2, 7, 4, 7, 0); // Rook moves left 2
    requested_moves[2] = new Move(2, 7, 5, 7, 0); // Rook moves left 3
    requested_moves[3] = new Move(2, 7, 6, 7, 0); // Rook moves left 4
    success_count += find_moves_RegularCases_FindAllMoves(
        38,
        new Board("8/8/8/8/4r3/8/###5/#KR4# w C - 0 1"),
        requested_moves,
        4
    );
    // No castles - Blocked on the way 3
    requested_moves[0] = new Move(5, 0, 2, 0, 0); // Rook moves left 3
    requested_moves[1] = new Move(5, 0, 3, 0, 0); // Rook moves left 2
    requested_moves[2] = new Move(5, 0, 4, 0, 0); // Rook moves left 1
    success_count += find_moves_RegularCases_FindAllMoves(
        39,
        new Board("##3rk#/5###/8/8/8/3R4/8/8 b f - 0 1"),
        requested_moves,
        3
    );

    // No castles - King under check
    requested_moves[0] = new Move(3, 7, 4, 7, 0); // King moves right
    success_count += find_moves_RegularCases_FindAllMoves(
        40,
        new Board("8/8/8/8/8/3r4/2#1####/2#K2R# w G - 0 1"),
        requested_moves,
        1
    );

    // No castles - No problem if the rook is under check
    requested_moves[0] = new Move(4, 7, 5, 7, 0); // King moves right
    requested_moves[1] = new Move(7, 7, 6, 7, 0); // Rook moves left 1
    requested_moves[2] = new Move(7, 7, 5, 7, 0); // Rook moves left 2
    requested_moves[3] = new Move(7, 7, 7, 6, 0); // Rook takes
    requested_moves[4] = new Move(4, 7, 7, 7, 0); // Castle
    success_count += find_moves_RegularCases_FindAllMoves(
        41,
        new Board("8/8/8/8/8/8/2#####r/3#K2R w H - 0 1"),
        requested_moves,
        5
    );

    // No castles - Pieces are on the destination - 1
    requested_moves[0] = new Move(6, 7, 7, 7, 0); // King moves right
    requested_moves[1] = new Move(5, 7, 4, 7, 0); // Rook moves left 1
    requested_moves[2] = new Move(5, 7, 3, 7, 0); // Rook moves left 2
    success_count += find_moves_RegularCases_FindAllMoves(
        42,
        new Board("8/8/8/8/8/8/5###/2#2RK1 w F - 0 1"),
        requested_moves,
        3
    );
    // No castles - Pieces are on the destination - 2
    requested_moves[0] = new Move(1, 7, 0, 7, 0); // King moves left
    requested_moves[1] = new Move(2, 7, 3, 7, 0); // Rook moves right 1
    requested_moves[2] = new Move(2, 7, 4, 7, 0); // Rook moves right 2
    requested_moves[3] = new Move(2, 7, 5, 7, 0); // Rook moves right 2
    success_count += find_moves_RegularCases_FindAllMoves(
        43,
        new Board("8/8/8/8/8/8/###5/1KR3#1 w F - 0 1"),
        requested_moves,
        4
    );

    // No castles - The destination is under check
    requested_moves[0] = new Move(2, 7, 3, 7, 0); // Rook moves left 1
    requested_moves[1] = new Move(2, 7, 4, 7, 0); // Rook moves left 2
    requested_moves[2] = new Move(2, 7, 5, 7, 0); // Rook moves left 3
    requested_moves[3] = new Move(2, 7, 6, 7, 0); // Rook moves left 4
    success_count += find_moves_RegularCases_FindAllMoves(
        44,
        new Board("8/8/8/8/6r1/8/###5/#KR4# w C - 0 1"),
        requested_moves,
        4
    );

    // No castles - The other rook block the trajectory
    requested_moves[0] = new Move(2, 7, 3, 7, 0); // Rook moves
    requested_moves[1] = new Move(2, 7, 4, 7, 0); // Rook moves
    success_count += find_moves_RegularCases_FindAllMoves(
        45,
        new Board("8/8/8/8/8/8/###5/RKR2#2 w A - 0 1"),
        requested_moves,
        2
    );

    // No castles - Blocked
    success_count += find_moves_RegularCases_FindAllMoves(
        46,
        new Board("bbqrkrnn/pppppppp/########/8/P7/8/1PPPPPPP/BBQRKRNN b DFdf a3 0 1"),
        requested_moves,
        0
    );

    // No castles - Extremety 1
    success_count += find_moves_RegularCases_FindAllMoves(
        47,
        new Board("8/8/8/8/8/8/###5/RK#T4 w A - 1 6"),
        requested_moves,
        0
    );

    // No castles - Extremety 2
    success_count += find_moves_RegularCases_FindAllMoves(
        48,
        new Board("8/8/8/8/8/8/5###/5TKR w H - 1 6"),
        requested_moves,
        0
    );

    // No castles - Extremety 2
    success_count += find_moves_RegularCases_FindAllMoves(
        49,
        new Board("8/8/8/8/8/8/########/4BRKR w FH - 3 11"),
        requested_moves,
        0
    );

    return success_count;
}

int find_moves_not_illegal_ones_testLauncher()
{
    int success_count = 0;
    Move *requested_moves[10];

    // Check - Prevent moves that don't remove the check - Obstruct the trajectory
    requested_moves[0] = new Move(0, 4, 0, 5, 0); // King escape - Down
    requested_moves[1] = new Move(0, 4, 1, 5, 0); // King escape - Down right
    requested_moves[2] = new Move(1, 1, 1, 4, 0); // Rook saves
    requested_moves[3] = new Move(2, 2, 4, 4, 0); // Bishop saves
    requested_moves[4] = new Move(4, 5, 4, 4, 0); // Pawn saves
    requested_moves[5] = new Move(6, 6, 6, 4, 0); // Queen saves - Vertically
    requested_moves[6] = new Move(6, 6, 4, 4, 0); // Queen saves - Diagonally
    success_count += find_moves_RegularCases_FindAllMoves(
        50,
        new Board("8/1R6/2B5/7r/K6r/4P3/6Q1/8 w - - 0 1"),
        requested_moves,
        7
    );

    // Check - Eat the piece that puts the king in check
    requested_moves[0] = new Move(3, 0, 3, 2, 0); // Queen eats
    requested_moves[1] = new Move(7, 2, 3, 2, 0); // Rook eats
    requested_moves[2] = new Move(2, 1, 3, 2, 0); // Pawn eats
    requested_moves[3] = new Move(6, 5, 3, 2, 0); // Bishop eats
    success_count += find_moves_RegularCases_FindAllMoves(
        51,
        new Board("3q4/2p5/3R3r/8/2#1#3/2#k#1b1/2###3/8 b - - 0 1"),
        requested_moves,
        4
    );

    // Pinned pieces cannot move (= if the move puts the king in check)
    requested_moves[0] = new Move(4, 3, 4, 2, 0); // Queen takes
    requested_moves[1] = new Move(3, 4, 2, 4, 0); // Rook takes
    success_count += find_moves_RegularCases_FindAllMoves(
        52,
        new Board("8/8/4q3/4Q3/2qRKPq1/4BN2/4q1b1/8 w - - 0 1"),
        requested_moves,
        2
    );

    // 2 pinned pieces cannot move at the same time (Whitte pawn can't take the en-passant)
    requested_moves[0] = new Move(0, 3, 0, 2, 0); // Kink moves up
    requested_moves[1] = new Move(0, 3, 1, 2, 0); // Kink moves up right
    requested_moves[2] = new Move(0, 3, 0, 4, 0); // Kink moves down
    requested_moves[3] = new Move(1, 3, 1, 2, 0); // Pawn move up
    success_count += find_moves_RegularCases_FindAllMoves(
        53,
        new Board("8/8/8/KPp4r/8/8/8/8/8 w - c6 0 1"),
        requested_moves,
        4
    );

    return success_count;
}

int find_moves_ckecks_testLauncher()
{
    return 0;
}
int find_moves_not_unpinning_testLauncher()
{
    return 0;
}
int find_moves_castles_testLauncher()
{
    return 0;
}

# endif

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
        7,
        new Board("r1bqkbnr/8/8/8/8/8/8/RNBQKB1R w - - 0 1", true),
        "r1bqkbnr/8/8/8/8/8/8/RNBQKB1R w - - 0 1"
    );

    // Pieces test 2. Edge test
    success_count += create_fen_unittest(
        1,
        new Board("1nb2bn1/8/8/3pP3/8/8/8/1NB2BN1 w - - 0 1", true),
        "1nb2bn1/8/8/3pP3/8/8/8/1NB2BN1 w - - 0 1"
    );

    // Empty board test + castling test
    success_count += create_fen_unittest(
        2,
        new Board("8/8/8/8/8/8/8/8 b AHah - 1 2", true),
        "8/8/8/8/8/8/8/8 b AHah - 1 2"
    );

    // Not full castling rights test
    success_count += create_fen_unittest(
        3,
        new Board("8/8/8/8/8/8/8/8 w Ac - 0 1", true),
        "8/8/8/8/8/8/8/8 w Ac - 0 1"
    );

    // En passant test
    success_count += create_fen_unittest(
        4,
        new Board("8/8/8/8/8/8/8/8 w - h7 0 1", true),
        "8/8/8/8/8/8/8/8 w - h7 0 1"
    );

    // Half turn rule test
    success_count += create_fen_unittest(
        5,
        new Board("8/8/8/8/8/8/8/8 w - - 12 1", true),
        "8/8/8/8/8/8/8/8 w - - 12 1"
    );

    // Game turn test
    success_count += create_fen_unittest(
        6,
        new Board("8/8/8/8/8/8/8/8 w - - 0 101", true),
        "8/8/8/8/8/8/8/8 w - - 0 101"
    );

    // Empty board test + castling test with standard rules
    success_count += create_fen_unittest(
        8,
        new Board("8/8/8/8/8/8/8/8 b KQkq - 1 2", false),
        "8/8/8/8/8/8/8/8 b KQkq - 1 2"
    );

    // Not full castling rights test
    success_count += create_fen_unittest(
        9,
        new Board("8/8/8/8/8/8/8/8 w Kq - 0 1", false),
        "8/8/8/8/8/8/8/8 w Kq - 0 1"
    );

    return success_count;
}

#pragma endregion create_fen

#pragma region get_game_state

int get_game_state_unittest(int testIndex, Board *board, float requested_game_state)
{
    float game_state = board->get_game_state();

    if (game_state != requested_game_state)
    {
        cerr << "\n---------- Board - get_game_state_unittest() - Test " << testIndex << " - !!! FAILURE !!! ----------" << endl;
        cerr << "- Board : " << endl;
        board->log();

        cerr << "\n- Actual get_game_state    : " << game_state << endl;
        cerr << "- Requested get_game_state : " << requested_game_state << endl;

        return 0;
    }

    return 1;
}

# if BITBOARD_IMPLEMENTATION == 1

int get_game_state_testLauncher()
{
    return 0;
}

# else

int get_game_state_testLauncher()
{
    int success_count = 0;
    vector<Move> moves_exists;
    vector<Move> moves_empty;

    moves_exists.push_back(Move(0, 0, 0, 0, 0));

    // 0 - Fifty-Move rule
    success_count += get_game_state_unittest(
        0,
        new Board("8/3k4/3p4/8/3P4/3K4/8/8 w - - 99 1"),
        GAME_CONTINUE
    );

    // 1 - Fifty-Move rule
    success_count += get_game_state_unittest(
        1,
        new Board("8/3k4/3p4/8/3P4/3K4/8/8 w - - 100 1"),
        DRAW
    );

    // 2 - Game turn max reached - 126 move proc the end game
    success_count += get_game_state_unittest(
        2,
        new Board("8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 126"),
        DRAW
    );

    // 3 - White wins
    success_count += get_game_state_unittest(
        3,
        new Board("k6R/7R/8/8/8/8/8/8 b - - 0 0"),
        WHITE_WIN
    );

    // 33 - Black wins
    success_count += get_game_state_unittest(
        33,
        new Board("8/8/8/8/8/8/7r/K6r w - - 0 0"),
        BLACK_WIN
    );

    // 4 - Stalemate - White turn
    success_count += get_game_state_unittest(
        4,
        new Board("6r1/8/8/8/8/8/r7/7K w - - 0 0"),
        DRAW
    );

    // 44 - Stalemate - Black turn
    success_count += get_game_state_unittest(
        44,
        new Board("k7/7R/8/8/8/8/8/1R6 b - - 0 0"),
        DRAW
    );

    // Insufficient material: King vs king
    success_count += get_game_state_unittest(
        5,
        new Board("8/8/3K4/8/8/3k4/8/8 w - - 0 0"),
        DRAW
    );
    // Same, but with an extra piece on the board (Game continue)
    success_count += get_game_state_unittest(
        51,
        new Board("8/8/3K4/8/8/3k4/8/2p5 w - - 0 0"),
        GAME_CONTINUE
    );

    // Insufficient material: King+knight vs king
    success_count += get_game_state_unittest(
        6,
        new Board("8/8/3K4/8/3n4/3k4/8/8 w - - 0 0"),
        DRAW
    );

    // Insufficient material: King+bishop vs king
    success_count += get_game_state_unittest(
        7,
        new Board("8/8/3K4/3B4/8/3k4/8/8 w - - 0 0"),
        DRAW
    );

    // Insufficient material: King+bishop vs king+bishop if both bishops are on the same square color.
    success_count += get_game_state_unittest(
        8,
        new Board("8/8/3K4/3B4/4b3/3k4/8/8 w - - 0 0"),
        DRAW
    );

    // Game continue (Bishop vs Knight)
    success_count += get_game_state_unittest(
        81,
        new Board("3k4/8/8/6K1/3B4/8/8/5n2 b - - 0 115"),
        GAME_CONTINUE
    );
    
    // Game continue (Knight vs Bishop)
    success_count += get_game_state_unittest(
        82,
        new Board("3k4/8/8/6K1/3N4/8/8/5b2 b - - 0 115"),
        GAME_CONTINUE
    );

    // Game continue (2 bishops on different square color)
    success_count += get_game_state_unittest(
        9,
        new Board("8/8/3K4/3B4/3b4/3k4/8/8 w - - 0 0"),
        GAME_CONTINUE
    );

    // Game continue (2 knights)
    success_count += get_game_state_unittest(
        9,
        new Board("8/8/3K4/3N4/3n4/3k4/8/8 w - - 0 0"),
        GAME_CONTINUE
    );

    // Game continue (Queen)
    success_count += get_game_state_unittest(
        9,
        new Board("8/8/3K4/3Q4/8/3k4/8/8 w - - 0 0"),
        GAME_CONTINUE
    );

    // Game continue (Rook)
    success_count += get_game_state_unittest(
        9,
        new Board("8/8/3K4/8/3r4/3k4/8/8 w - - 0 0"),
        GAME_CONTINUE
    );

    Board *board = new Board("8/3K4/3Q4/8/8/3q4/3k4/8 w - - 0 0");

    // Threefold Repetition rule fails (Only 1 repetition)
    board->apply_move(Move(3, 2, 4, 2, 0)); // White move right
    board->apply_move(Move(3, 5, 4, 5, 0)); // Black move right
    board->apply_move(Move(4, 2, 3, 2, 0)); // White move left
    board->apply_move(Move(4, 5, 3, 5, 0)); // Black move left
    success_count += get_game_state_unittest(
        9,
        board,
        GAME_CONTINUE
    );

    // Threefold Repetition rule succeed -> 2 repetitions
    board->apply_move(Move(3, 2, 4, 2, 0)); // White move right
    board->apply_move(Move(3, 5, 4, 5, 0)); // Black move right
    board->apply_move(Move(4, 2, 3, 2, 0)); // White move left
    board->apply_move(Move(4, 5, 3, 5, 0)); // Black move left
    success_count += get_game_state_unittest(
        9,
        board,
        DRAW
    );

    // CG int test
    success_count += get_game_state_unittest(
        10,
        new Board("1rk5/1pnrb2p/2p1b1P1/Q2p1p2/P2P1P2/3n1BP1/1q6/NKR3B1 w b - 6 25"),
        BLACK_WIN
    );

    // Test game_state with previous moves
    board = new Board("b4r2/r3np2/2p1k2b/p1P1p1Q1/K1P1P2p/3R1B2/1q3P2/1N1R4 b - - 0 48");
    board->apply_move(Move("b2b4"));
    success_count += get_game_state_unittest(
        11,
        board,
        BLACK_WIN
    );

    return success_count;
}

# endif

#pragma endregion get_game_state

#pragma region is_check

int is_check_unittest(int testIndex, Board *board, bool requested_is_check)
{
    bool is_check = board->get_check_state();

    if (is_check != requested_is_check)
    {
        cerr << "\n---------- Board - is_check_unittest() - Test " << testIndex << " - !!! FAILURE !!! ----------" << endl;
        cerr << "- Board : " << endl;
        board->log();

        cerr << "\n- Actual is_check    : " << is_check << endl;
        cerr << "- Requested is_check : " << requested_is_check << endl;

        return 0;
    }

    return 1;
}

# if BITBOARD_IMPLEMENTATION == 1

int is_check_testLauncher()
{
    return 0;
}

# else

int is_check_testLauncher()
{
    int success_count = 0;

    // Check by rook
    success_count += is_check_unittest(
        1,
        new Board("4r3/8/8/8/8/8/8/4K3 w - - 0 1"),
        true
    );
    success_count += is_check_unittest(
        2,
        new Board("4k3/8/8/8/8/8/8/4R3 b - - 0 1"),
        true
    );

    // Check by bishop
    success_count += is_check_unittest(
        3,
        new Board("8/8/1b6/8/8/4K3/8/8 w - - 0 1"),
        true
    );
    success_count += is_check_unittest(
        4,
        new Board("8/8/1k6/8/8/4B3/8/8 b - - 0 1"),
        true
    );

    // Check by knight
    success_count += is_check_unittest(
        5,
        new Board("8/8/8/8/8/3K4/1n6/8 w - - 0 1"),
        true
    );
    success_count += is_check_unittest(
        6,
        new Board("8/8/8/8/2N5/8/1k6/8 b - - 0 1"),
        true
    );

    // Check by pawn
    success_count += is_check_unittest(
        7,
        new Board("8/8/1p6/2K5/8/8/8/8 w - - 0 1"),
        true
    );
    success_count += is_check_unittest(
        8,
        new Board("8/8/8/8/8/2k5/1P6/8 b - - 0 1"),
        true
    );

    // Check obstrued by piece - Rook
    success_count += is_check_unittest(
        9,
        new Board("4r3/8/8/4t3/8/8/8/4K3 w - - 0 1"),
        false
    );
    success_count += is_check_unittest(
        10,
        new Board("4k3/8/8/4T3/8/8/8/4R3 b - - 0 1"),
        false
    );

    // Check obstrued by piece - Bishop
    success_count += is_check_unittest(
        11,
        new Board("8/8/1b6/8/3T4/4K3/8/8 w - - 0 1"),
        false
    );
    success_count += is_check_unittest(
        12,
        new Board("8/8/1k6/8/3t4/4B3/8/8 b - - 0 1"),
        false
    );

    return success_count;
}

# endif

#pragma endregion is_check

#pragma region clone

int clone_unittest(int testIndex, Board *board)
{
    Board *clone = board->clone();

    if (!(*board == clone))
    {
        cerr << "\n---------- Board - clone_unittest() - Test " << testIndex << " - !!! FAILURE !!! ----------" << endl;
        cerr << "- Board : " << endl;
        board->log();

        cerr << "\n- Actual clone : " << endl;
        clone->log();

        return 0;
    }

    return 1;
}

int clone_testLauncher()
{
    int success_count = 0;

    // Empty board - Chess960 rules
    success_count += clone_unittest(
        1,
        new Board("8/8/8/8/8/8/8/8 w - - 0 1", true)
    );
    // Empty board - Standard rules
    success_count += clone_unittest(
        2,
        new Board("8/8/8/8/8/8/8/8 w - - 0 1", false)
    );

    // With pieces - Chess960 rules
    success_count += clone_unittest(
        3,
        new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b AHah b2 10 100", true)
    );
    // With pieces - Standard rules
    success_count += clone_unittest(
        4,
        new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b AHah b2 10 100", false)
    );

    return success_count;
}

#pragma endregion clone

int mainTestBoard()
{
    int successCount = 0;

    // Use '#' to block moves
    // Use 't' and 'T' to simulate opponent pieces

    // successCount += create_fen_testLauncher();
    // successCount += apply_move_testLauncher();
    // successCount += apply_move_testLauncher_uci();
    // successCount += get_game_state_testLauncher();
    // successCount += is_check_testLauncher();

    successCount += find_pawn_moves_testLauncher();
    successCount += find_knight_moves_testLauncher();
    successCount += find_bishop_moves_testLauncher();
    successCount += find_rook_moves_testLauncher();
    successCount += find_queen_moves_testLauncher();
    successCount += find_king_moves_testLauncher();
    successCount += find_moves_ckecks_testLauncher();
    successCount += find_moves_not_unpinning_testLauncher();
    successCount += find_moves_not_illegal_ones_testLauncher();
    successCount += find_moves_castles_testLauncher();

    // successCount += clone_testLauncher();

    return successCount;
}
