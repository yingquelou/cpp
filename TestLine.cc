#include "Line.h"
#include "Line.cpp"
using namespace std;
int main(void)
{
    Line L1{{1, 2}, VectorOfMath(1, 2), 1},
        L2{{0, 0}, {2, 4}, 1},
        L3{1.0, -1.0, 0.0},
        L4{0.0, 1.0, 0.0};
    Point P;
    // L1.KnowX(-1.0, P);
    cout << L1.CrossoverPoint(L2, P) << endl
         << P << endl
         << L3.AngleOfLine(L4) << endl;
    return 0;
}