#include <stdexcpt.h>
//C实现伪命名空间
#include "file.h"
int main(void)
{
    init();
    printf("%lf\n", Test.Sum(1, 23));
    printf("%lf\n", Test.Sub(1, 23));
    printf("%lf\n", Test.Div(46, 23));
    printf("%lf\n", Test.Mul(2, 3));
    return 0;
}