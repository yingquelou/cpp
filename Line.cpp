#include "Line.h"
#include <stdexcept>
using namespace std;
Line::Line(const double &a, const double &b, const double &c)
{
    try
    {
        if (!a && !b)
            throw invalid_argument("The a and the b can't all be zero");
        A = a;
        B = b;
        C = c;
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
bool Line::IsLineExist()
{
    if (!A && !B)
        return false;
    return true;
}
