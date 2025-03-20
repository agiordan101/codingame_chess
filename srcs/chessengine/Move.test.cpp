#include "Move.hpp"

#pragma region Move

int Move_unittest(int test_index, string uci, Move *requested_move)
{
    Move final_move = Move(uci);

    if (final_move == requested_move)
        return 1;

    cerr << "\n---------- Move - Move_unittest() - Test " << test_index
         << " - !!! FAILURE !!! ----------" << endl;
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

    // All columns and lines
    success_count += Move_unittest(1, "a8b7", new Move('p', 1UL << 0, 1UL << 9, 0, NOINFO));
    success_count += Move_unittest(2, "c5d6", new Move('p', 1UL << 26, 1UL << 19, 0, NOINFO));
    success_count += Move_unittest(3, "e4f3", new Move('p', 1UL << 36, 1UL << 45, 0, NOINFO));
    success_count += Move_unittest(4, "g1h2", new Move('p', 1UL << 62, 1UL << 55, 0, NOINFO));

    // Promotions
    success_count += Move_unittest(5, "a7a8r", new Move('P', 1UL << 8, 1UL << 0, 'r', NOINFO));
    success_count += Move_unittest(6, "h7h8b", new Move('p', 1UL << 15, 1UL << 7, 'b', NOINFO));

    // White castle - Left side
    success_count += Move_unittest(7, "e1a1", new Move('K', 1UL << 60, 1UL << 56, 0, NOINFO));
    // White castle - Right side
    success_count += Move_unittest(8, "e1h1", new Move('K', 1UL << 60, 1UL << 63, 0, NOINFO));
    // Black castle - Left side
    success_count += Move_unittest(9, "e8a8", new Move('k', 1UL << 4, 1UL << 0, 0, NOINFO));
    // Black castle - Right side
    success_count += Move_unittest(10, "e8h8", new Move('k', 1UL << 4, 1UL << 7, 0, NOINFO));

    return success_count;
}

#pragma endregion Move

#pragma region to_uci

int to_uci_unittest(
    int test_index, Move *move, bool chess960_rules, bool castling, string requested_uci
)
{
    string final_uci = move->to_uci(chess960_rules, castling);

    if (final_uci == requested_uci)
        return 1;

    cerr << "\n---------- Move - to_uci_unittest() - Test " << test_index
         << " - !!! FAILURE !!! ----------" << endl;
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
    success_count +=
        to_uci_unittest(1, new Move('p', 1UL << 48, 1UL << 8, 0, NOTCASTLE), false, false, "a2a7");
    success_count +=
        to_uci_unittest(2, new Move('p', 1UL << 17, 1UL << 46, 0, NOTCASTLE), false, false, "b6g3");
    success_count +=
        to_uci_unittest(3, new Move('p', 1UL << 3, 1UL << 60, 0, NOTCASTLE), true, false, "d8e1");
    success_count +=
        to_uci_unittest(4, new Move('p', 1UL << 34, 1UL << 29, 0, NOTCASTLE), true, false, "c4f5");

    // Promotions
    success_count += to_uci_unittest(
        5, new Move('p', 1UL << 14, 1UL << 6, 'q', NOTCASTLE), false, false, "g7g8q"
    );
    success_count += to_uci_unittest(
        6, new Move('P', 1UL << 14, 1UL << 6, 'n', NOTCASTLE), true, false, "g7g8n"
    );

    //  - White castle - Right - Regular chess rule
    Move *castling_move = new Move('K', 1UL << 60, 1UL << 63, 0, WHITERIGHT);
    success_count += to_uci_unittest(7, castling_move, false, true, "e1g1");
    //  - White castle - Right - Chess960 rule
    castling_move = new Move('K', 1UL << 60, 1UL << 63, 0, WHITERIGHT);
    success_count += to_uci_unittest(8, castling_move, true, true, "e1h1");

    //  - White castle - Left - Regular chess rule
    castling_move = new Move('K', 1UL << 60, 1UL << 56, 0, WHITELEFT);
    success_count += to_uci_unittest(9, castling_move, false, true, "e1c1");
    //  - White castle - Left - Chess960 rule
    castling_move = new Move('K', 1UL << 60, 1UL << 56, 0, WHITELEFT);
    success_count += to_uci_unittest(10, castling_move, true, true, "e1a1");

    //  - Black castle - Right - Regular chess rule
    castling_move = new Move('k', 1UL << 4, 1UL << 7, 0, BLACKRIGHT);
    success_count += to_uci_unittest(11, castling_move, false, true, "e8g8");

    //  - Black castle - Right - Chess960 rule
    castling_move = new Move('k', 1UL << 4, 1UL << 7, 0, BLACKRIGHT);
    success_count += to_uci_unittest(12, castling_move, true, true, "e8h8");

    //  - Black castle - Left - Regular chess rule
    castling_move = new Move('k', 1UL << 4, 1UL << 0, 0, BLACKLEFT);
    success_count += to_uci_unittest(13, castling_move, false, true, "e8c8");

    //  - Black castle - Left - Chess960 rule
    castling_move = new Move('k', 1UL << 4, 1UL << 0, 0, BLACKLEFT);
    success_count += to_uci_unittest(14, castling_move, true, true, "e8a8");

    return success_count;
}

#pragma endregion to_uci

#pragma region equaloperator

int equaloperator_unittest(int test_index, Move *move1, Move *move2, bool expected_result)
{
    bool final_result = *move1 == move2;

    if (final_result == expected_result)
        return 1;

    cerr << "\n---------- Move - equaloperator_unittest() - Test " << test_index
         << " - !!! FAILURE !!! ----------" << endl;
    cerr << "- Move 1 : " << move1->to_uci() << endl;
    move1->log();
    cerr << "- Move 2 : " << move2->to_uci() << endl;
    move2->log();

    cerr << "\n- Final result :    " << final_result << endl;
    cerr << "- Expected result : " << expected_result << endl;
    return 0;
}

int equaloperator_unittestLauncher()
{
    int success_count = 0;

    success_count += equaloperator_unittest(
        1, new Move('p', 1UL << 0, 1UL << 9), new Move('p', 1UL << 0, 1UL << 9), true
    );
    success_count += equaloperator_unittest(
        2, new Move('p', 1UL << 0, 1UL << 9), new Move('p', 1UL << 0, 1UL << 8), false
    );

    success_count += equaloperator_unittest(3, new Move("a1b7"), new Move("a1b7"), true);
    success_count += equaloperator_unittest(4, new Move("a1b7"), new Move("a1b8"), false);

    // Promotions
    success_count += equaloperator_unittest(
        5, new Move('P', 1UL << 8, 1UL << 0, 'r'), new Move('P', 1UL << 8, 1UL << 0, 'r'), true
    );
    success_count += equaloperator_unittest(
        6, new Move('P', 1UL << 8, 1UL << 0, 'r'), new Move('P', 1UL << 8, 1UL << 0, 'q'), false
    );

    success_count += equaloperator_unittest(7, new Move("a1b7r"), new Move("a1b7r"), true);
    success_count += equaloperator_unittest(8, new Move("a1b7r"), new Move("a1b8q"), false);

    return success_count;
}

#pragma endregion equaloperator

#pragma region compare_move_vector

int compare_move_vector_unittest(
    int test_index, vector<Move> movelst1, vector<Move> movelst2, bool expected_result
)
{
    bool final_result = Move::compare_move_vector(movelst1, movelst2);

    if (final_result == expected_result)
        return 1;

    cerr << "\n---------- Move - compare_move_vector_unittest() - Test " << test_index
         << " - !!! FAILURE !!! ----------" << endl;
    cerr << "- Move vector 1 : " << endl;
    for (Move move : movelst1)
        cerr << move.to_uci() << endl;
    cerr << "- Move vector 2 : " << endl;
    for (Move move : movelst2)
        cerr << move.to_uci() << endl;

    cerr << "\n- Final result :    " << final_result << endl;
    cerr << "- Expected result : " << expected_result << endl;
    return 0;
}

#pragma endregion compare_move_vector

int unittests_Move()
{
    int success_count = 0;

    success_count += to_uci_unittestLauncher();
    success_count += Move_unittestLauncher();
    success_count += equaloperator_unittestLauncher();

    return success_count;
}
