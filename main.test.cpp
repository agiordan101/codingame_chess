#include <iostream>
using namespace std;

int mainTestBoard();

int main()
{
    int successCount = 0;

    cerr << "[UNITTEST] Start ..." << endl;

    successCount += mainTestBoard();

    cerr << "[UNITTEST] End: " << successCount << " tests were successfull !" << endl;
}