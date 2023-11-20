#include "Move.hpp"

#pragma region Move

int Move_unittest(int test_index, string uci, char src_piece, char dst_piece, bool chess960_rule, Move *requested_move)
{
    Move final_move = Move(uci, src_piece, dst_piece, chess960_rule);

    if (final_move == requested_move)
        return 1;

    cerr << "\n---------- Move - Move_unittest() - Test " << test_index << " - !!! FAILURE !!! ----------" << endl;
    cerr << "- Move() parameters : " << endl;
    cerr << "uci : " << uci << endl;
    cerr << "src_piece : " << src_piece << endl;
    cerr << "dst_piece : " << dst_piece << endl;
    cerr << "chess960_rule : " << chess960_rule << endl;

    cerr << "- Final move : " << endl;
    final_move.log();
    cerr << "- Requested move : " << endl;
    requested_move->log();

    return 0;
}

int Move_unittestLauncher()
{
    int success_count = 0;

    success_count += Move_unittest(
        1,
        "a8b7", 'B', 0, true
        , new Move(0, 0, 1, 1, false, false)
    );
    success_count += Move_unittest(
        2,
        "c5d6", 'B', 0, true
        , new Move(2, 3, 3, 2, false, false)
    );
    success_count += Move_unittest(
        3,
        "e4f3", 'b', 0, true
        , new Move(4, 4, 5, 5, false, false)
    );
    success_count += Move_unittest(
        4,
        "g1h2", 'b', 0, true
        , new Move(6, 7, 7, 6, false, false)
    );

    // Promotions
    success_count += Move_unittest(
        5,
        "a7a8R", 'P', 0, true
        , new Move(0, 1, 0, 0, false, 'R')
    );
    success_count += Move_unittest(
        6,
        "h7h8b", 'p', 0, true
        , new Move(7, 1, 7, 0, false, 'b')
    );

    // Same move with 2 different rules :
    string castle_uci = "e1c1";

    //  - Regular chess castle rule
    success_count += Move_unittest(
        7,
        castle_uci, 'k', 'r', false
        , new Move(4, 7, 0, 7, true, false)
    );
    //  - Chess960 castle rule 
    success_count += Move_unittest(
        8,
        castle_uci, 'K', 'R', true
        , new Move(4, 7, 2, 7, true, false)
    );

    // Same classic king moves with both rules
    string king_uci = "e1d1";

    //  - Regular chess castle rule
    success_count += Move_unittest(
        9,
        king_uci, 'k', 0, false
        , new Move(4, 7, 3, 7, false, false)
    );
    //  - Chess960 castle rule 
    success_count += Move_unittest(
        10,
        king_uci, 'K', 0, true
        , new Move(4, 7, 3, 7, false, false)
    );

    return success_count;
}

#pragma endregion Move

#pragma region to_uci

int to_uci_unittest(int test_index, Move *move, bool chess960_rule, string requested_uci)
{
    string final_uci = move->to_uci(chess960_rule);

    if (final_uci == requested_uci)
        return 1;

    cerr << "\n---------- Move - to_uci_unittest() - Test " << test_index << " - !!! FAILURE !!! ----------" << endl;
    cerr << "- Move : " << endl;
    move->log();

    cerr << "\n- Final UCI : " << final_uci << endl;
    cerr << "- Requested UCI : " << requested_uci << endl;
    return 0;
}

int to_uci_unittestLauncher()
{
    int success_count = 0;

    // All columns and lines
    success_count += to_uci_unittest(
        1,
        new Move(0, 6, 0, 1, false, false),
        true,
        "a2a7"
    );
    success_count += to_uci_unittest(
        2,
        new Move(1, 2, 6, 5, false, false),
        true,
        "b6g3"
    );
    success_count += to_uci_unittest(
        3,
        new Move(3, 0, 4, 7, false, false),
        true,
        "d8e1"
    );
    success_count += to_uci_unittest(
        4,
        new Move(2, 4, 5, 3, false, false),
        true,
        "c4f5"
    );

    // Promotions
    success_count += to_uci_unittest(
        5,
        new Move(6, 1, 6, 0, false, 'q'),
        true,
        "g7g8q"
    );
    success_count += to_uci_unittest(
        6,
        new Move(6, 1, 6, 0, false, 'N'),
        true,
        "g7g8n"
    );

    // Same castling with 2 different rules :
    Move *castling_move = new Move(4, 7, 7, 7, true, false);

    //  - Regular chess castle rule
    success_count += to_uci_unittest(
        7,
        castling_move,
        false,
        "e1g1"
    );
    //  - Chess960 castle rule 
    success_count += to_uci_unittest(
        8,
        castling_move,
        true,
        "e1h1"
    );

    // Same classic king move with 2 different rules :
    Move *king_move = new Move(4, 7, 5, 7, false, false);

    //  - Regular chess castle rule
    success_count += to_uci_unittest(
        9,
        king_move,
        false,
        "e1f1"
    );
    //  - Chess960 castle rule 
    success_count += to_uci_unittest(
        10,
        king_move,
        true,
        "e1f1"
    );

    return success_count;
}

#pragma endregion to_uci

int unittests_Move()
{
    int success_count = 0;

    success_count += to_uci_unittestLauncher();
    success_count += Move_unittestLauncher();

    return success_count;
}
