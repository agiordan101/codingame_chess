#include "Move.hpp"

#pragma region Move

int Move_unittest(int test_index, string uci, Move *requested_move)
{
    Move final_move = Move(uci);

    if (final_move == requested_move)
        return 1;

    cerr << "\n---------- Move - Move_unittest() - Test " << test_index << " - !!! FAILURE !!! ----------" << endl;
    cerr << "- Move() parameters : " << endl;
    cerr << "uci : " << uci << endl;

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
        "a8b7",
        new Move(0, 0, 1, 1, 0)
    );
    success_count += Move_unittest(
        2,
        "c5d6",
        new Move(2, 3, 3, 2, 0)
    );
    success_count += Move_unittest(
        3,
        "e4f3",
        new Move(4, 4, 5, 5, 0)
    );
    success_count += Move_unittest(
        4,
        "g1h2",
        new Move(6, 7, 7, 6, 0)
    );

    // Promotions
    success_count += Move_unittest(
        5,
        "a7a8R",
        new Move(0, 1, 0, 0, 'R')
    );
    success_count += Move_unittest(
        6,
        "h7h8b",
        new Move(7, 1, 7, 0, 'b')
    );

    // (Castling)
    success_count += Move_unittest(
        7,
        "e1a1",
        new Move(4, 7, 0, 7, 0)
    );

    return success_count;
}

#pragma endregion Move

#pragma region to_uci

int to_uci_unittest(int test_index, Move *move, bool regular_rules_castling, string requested_uci)
{
    string final_uci = move->to_uci(regular_rules_castling);

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
        new Move(0, 6, 0, 1, 0),
        false,
        "a2a7"
    );
    success_count += to_uci_unittest(
        2,
        new Move(1, 2, 6, 5, 0),
        false,
        "b6g3"
    );
    success_count += to_uci_unittest(
        3,
        new Move(3, 0, 4, 7, 0),
        false,
        "d8e1"
    );
    success_count += to_uci_unittest(
        4,
        new Move(2, 4, 5, 3, 0),
        false,
        "c4f5"
    );

    // Promotions
    success_count += to_uci_unittest(
        5,
        new Move(6, 1, 6, 0, 'q'),
        false,
        "g7g8q"
    );
    success_count += to_uci_unittest(
        6,
        new Move(6, 1, 6, 0, 'N'),
        false,
        "g7g8n"
    );

    // Same right castle with 2 different rules :
    Move *castling_move = new Move(4, 7, 7, 7, 0);

    //  - Regular chess castle rule
    success_count += to_uci_unittest(
        7,
        castling_move,
        true,
        "e1g1"
    );
    //  - Chess960 castle rule 
    success_count += to_uci_unittest(
        8,
        castling_move,
        false,
        "e1h1"
    );

    // Same left castle with 2 different rules :
    castling_move = new Move(4, 7, 0, 7, 0);

    //  - Regular chess castle rule
    success_count += to_uci_unittest(
        9,
        castling_move,
        true,
        "e1c1"
    );
    //  - Chess960 castle rule 
    success_count += to_uci_unittest(
        10,
        castling_move,
        false,
        "e1a1"
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
