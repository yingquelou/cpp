#include "Line.h"
#include <stdexcept>
using namespace std;
Line::Line(const double &a, const double &b, const double &c)
{
    if (!a && !b)
    {
        cerr << "The a and the b can't all be zero" << endl;
        exit(EXIT_FAILURE);
    }
    A = a;
    B = b;
    C = c;
}
bool Line::IsLineExist()
{
    if (!A && !B)
        return false;
    return true;
}
