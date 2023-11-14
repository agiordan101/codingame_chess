#include <iostream>
using namespace std;

int mainTestBoard();
int unittests_Move();

int main()
{
    int successCount = 0;

    cerr << "[UNITTEST] Start ..." << endl;

    // successCount += mainTestBoard();
    successCount += unittests_Move();

    cerr << "[UNITTEST] End: " << successCount << " tests were successfull !" << endl;
}