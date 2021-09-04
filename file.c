#include <stdio.h>
//C实现伪命名空间
static struct
{
    double (*Sum)(const double, const double);
    double (*Sub)(const double, const double);
} Test;
double sum(const double x, const double y)
{
    return x + y;
}
double sub(const double x, const double y)
{
    return x - y;
}
void init(void)
{
    Test.Sum = sum;
    Test.Sub = sub;
}
int main(void)
{
    init();
    printf("%lf\n", Test.Sum(1, 23));
    printf("%lf\n", Test.Sub(1, 23));
    /* printf("%p %p\n", &a.Sum, a.Sum);
    a.Sum = &sum;
    printf("%p %p\n", &a.Sum, a.Sum);
    printf("%lf\n", a.Sum(1, 2)); */
    return 0;
}