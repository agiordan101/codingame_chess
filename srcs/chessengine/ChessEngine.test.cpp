#include "ChessEngine.hpp"

#pragma region algebraic_to_coord

int algebraic_to_coord_unittest(int test_index, string algebraic, int requested_x, int requested_y)
{
    int final_x;
    int final_y;
    algebraic_to_coord(algebraic, &final_x, &final_y);

    if (final_x != requested_x || final_y != requested_y)
    {
        cerr << "\n---------- ChessEngine - algebraic_to_coord() - Test " << test_index
             << " - !!! FAILURE !!! ----------" << endl;
        cerr << "- Initial algebraic : " << algebraic << endl;
        cerr << "- Final coords     : x=" << final_x << " y=" << final_y << endl;
        cerr << "- Requested coords : x=" << requested_x << " y=" << requested_y << endl;
        return 0;
    }

    return 1;
}

int algebraic_to_coord_unittestLauncher()
{
    int success_count = 0;

    success_count += algebraic_to_coord_unittest(1, string("a8"), 0, 0);
    success_count += algebraic_to_coord_unittest(2, string("b7"), 1, 1);
    success_count += algebraic_to_coord_unittest(3, string("c3"), 2, 5);
    success_count += algebraic_to_coord_unittest(4, string("d5"), 3, 3);
    success_count += algebraic_to_coord_unittest(5, string("e4"), 4, 4);
    success_count += algebraic_to_coord_unittest(6, string("f6"), 5, 2);
    success_count += algebraic_to_coord_unittest(7, string("g2"), 6, 6);
    success_count += algebraic_to_coord_unittest(8, string("h1"), 7, 7);

    return success_count;
}

#pragma endregion algebraic_to_coord

#pragma region coord_to_algebraic

int coord_to_algebraic_unittest(int test_index, int x, int y, string requested_algebraic)
{
    string final_algebraic = coord_to_algebraic(x, y);

    if (final_algebraic != requested_algebraic)
    {
        cerr << "\n---------- ChessEngine - coord_to_algebraic() - Test " << test_index
             << " - !!! FAILURE !!! ----------" << endl;
        cerr << "- Initial coords : x=" << x << " y=" << y << endl;
        cerr << "- Final algebraic : " << final_algebraic << endl;
        cerr << "- Requested algebraic : " << requested_algebraic << endl;
        return 0;
    }

    return 1;
}

int coord_to_algebraic_unittestLauncher()
{
    int success_count = 0;

    success_count += coord_to_algebraic_unittest(1, 0, 7, string("a1"));
    success_count += coord_to_algebraic_unittest(2, 1, 6, string("b2"));
    success_count += coord_to_algebraic_unittest(3, 2, 2, string("c6"));
    success_count += coord_to_algebraic_unittest(4, 3, 4, string("d4"));
    success_count += coord_to_algebraic_unittest(5, 4, 3, string("e5"));
    success_count += coord_to_algebraic_unittest(6, 5, 5, string("f3"));
    success_count += coord_to_algebraic_unittest(7, 6, 1, string("g7"));
    success_count += coord_to_algebraic_unittest(8, 7, 0, string("h8"));

    return success_count;
}

#pragma endregion coord_to_algebraic

int unittests_ChessEngine()
{
    int success_count = 0;

    success_count += algebraic_to_coord_unittestLauncher();
    success_count += coord_to_algebraic_unittestLauncher();

    return success_count;
}
