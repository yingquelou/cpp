#include "Oval.h"
int main(void)
{
    // Oval t;
    Point A(1, -1), B(1, 1);
    cout << "This point is " << A << endl;
    A--;
    cout << "This point is " << A << endl;
    cout << "This point is " << A - B << endl;
    // cout << A * B << endl;
    return 0;
}
