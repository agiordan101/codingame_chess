#include "Move.hpp"

#pragma region to_uci

int to_uci_unittest(int test_index, Move *move, string requested_uci)
{
    string final_uci = move->to_uci();

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

    success_count += to_uci_unittest(
        1,
        new Move(0, 0, 0, 1, false, false, false),
        "a1a2"
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
