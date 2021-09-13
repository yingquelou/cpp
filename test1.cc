#include <iostream>
#include <cmath>
using namespace std;
using mytype = long double;
mytype mysqrt(const mytype &num, const mytype &width)
{
    mytype left = (int)sqrt(num),
           right = left + 1, mid;
    while (1)
    {
        mid = (left + right) / 2;
        if (mid * mid < num)
        {
            left = mid;
        }
        else
        {
            right = mid;
        }
        if (abs(mid * mid - num) < width)
            break;
    }
    return mid;
}
int main(void)
{
    mytype tmp;
    for (mytype i = 1; i > 1e-15; i /= 10)
    {
        tmp = mysqrt(10.0, i);
        printf("%.50llf %53.50llf\n", tmp, tmp * tmp);
    }
    return 0;
}