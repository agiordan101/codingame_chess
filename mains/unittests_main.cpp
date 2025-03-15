#include <iostream>
using namespace std;

int mainTestBoard();
int unittests_Move();
int unittests_ChessEngine();

int main()
{
    int successCount = 0;

    cerr << "[UNITTEST] Start ..." << endl;

    successCount += mainTestBoard();
    successCount += unittests_Move();
    successCount += unittests_ChessEngine();

    cerr << "[UNITTEST] End: " << successCount << "/199 tests were successfull !" << endl;
}