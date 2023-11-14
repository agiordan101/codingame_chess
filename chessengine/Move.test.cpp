#include "Move.hpp"

#pragma region to_uci

int to_uci_unittest(int test_index, Move *move, bool chess960_rule, string requested_uci)
{
    string final_uci = move->to_uci(chess960_rule);

    if (final_uci != requested_uci)
    {
        cerr << "\n---------- Move - to_uci_unittest() - Test " << test_index << " - !!! FAILURE !!! ----------" << endl;
        cerr << "- Move : " << endl;
        move->log();

        cerr << "\n- Final UCI : " << final_uci << endl;
        cerr << "- Requested UCI : " << requested_uci << endl;
        return 0;
    }

    return 1;
}

int to_uci_unittestLauncher()
{
    int success_count = 0;

    // All columns and lines
    success_count += to_uci_unittest(
        1,
        new Move(0, 6, 0, 1, false, false, false),
        true,
        "a2a7"
    );
    success_count += to_uci_unittest(
        2,
        new Move(1, 2, 6, 5, false, false, false),
        true,
        "b6g3"
    );
    success_count += to_uci_unittest(
        3,
        new Move(3, 0, 4, 7, false, false, false),
        true,
        "d8e1"
    );
    success_count += to_uci_unittest(
        4,
        new Move(2, 4, 5, 3, false, false, false),
        true,
        "c4f5"
    );

    // Promotions
    success_count += to_uci_unittest(
        5,
        new Move(6, 1, 6, 0, false, 'q', false),
        true,
        "g7g8q"
    );
    success_count += to_uci_unittest(
        6,
        new Move(6, 1, 6, 0, false, 'N', false),
        true,
        "g7g8n"
    );

    // Regular chess castle rule 
    success_count += to_uci_unittest(
        6,
        new Move(4, 7, 7, 7, true, false, false),
        false,
        "e1g1"
    );

    // Chess960 castle rule 
    success_count += to_uci_unittest(
        6,
        new Move(4, 7, 7, 7, true, false, false),
        true,
        "e1h1"
    );

    return success_count;
}

#pragma endregion to_uci

int unittests_Move()
{
    int success_count = 0;

    success_count += to_uci_unittestLauncher();

    return success_count;
}
